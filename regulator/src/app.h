#ifndef __app_h__
#define __app_h__

#include <Arduino.h>
#include "task.h"
#include "math.h"
#include "tempreader.h"

class ButtonReader;
class Screen;

class App : public Task {
public:
  TempReader    &temp;
  bool           act_ballon;
  ballon_mode_t  ballon_mode;
  dpin_t         ballon_relay_pin;

public:

  App(TempReader &temp, dpin_t ballon_relay_pin) :
    temp(temp),
    act_ballon(false),
    ballon_mode(BALLON_MODE_AUTO),
    ballon_relay_pin(ballon_relay_pin)
  {
  }
  
  void setup(){
    temp.setup();
    pinMode(ballon_relay_pin, OUTPUT);
    write();
  }
  
  void    update(uint8_t btn);
  bool    wakeup();
  ulong_t loop(ulong_t ms);
  
  float t_ltc()    { return temp.temperatures[0]; }
  float t_ballon() { return temp.temperatures[1]; }
  
private:
  void write();
};

#include "buttonreader.h"
#include "screen.h"
#include "types.h"

bool App::wakeup(){
  return temp.changed;
}

ulong_t App::loop(ulong_t ms){
  // Compute action to take every 10 seconds only
  bool new_act_ballon = act_ballon;
  switch(ballon_mode) {
  default:
  case BALLON_MODE_AUTO:
    // Si le LTC est inférieur a 25°C (il ne tourne pas) on active le ballon
    // (électrovane au repos)
    if (t_ltc() < (act_ballon ? 26.0 : 25.0)) {
      new_act_ballon = 1;
    } else if (act_ballon) {
      // lorsque on chauffe le ballon avec le poele:
      // - si le poele est moins chaud que le ballon, on coupe le ballon
      // - si le ballon a une température supérieure à 95°C, on coupe le ballon
      new_act_ballon =
        t_ballon() < t_ltc() &&
        t_ballon() < 95.0;
    } else {
      // lorsque on ne chauffe pas le ballon avec le poele:
      // - si le poele est plus chaud de 1°C que le ballon
      // - et que le poele a une température inférieure à 95°C, on chauffe le ballon
      new_act_ballon =
        t_ltc() > t_ballon() + 1.0 &&
        t_ballon() < 95.0;
    }
    break;
  case BALLON_MODE_FORCE_ON:
    new_act_ballon = true;
    break;
  case BALLON_MODE_FORCE_OFF:
    new_act_ballon = false;
    break;
  }
  if(new_act_ballon != act_ballon) {
    act_ballon = new_act_ballon;
    write();
    changed = true;
  }
  return ms+500;
}

void App::write() {
  digitalWrite(ballon_relay_pin, act_ballon ? LOW : HIGH);  
}

#endif
