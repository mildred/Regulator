#ifndef __app_h__
#define __app_h__

#include <Arduino.h>
#include "task.h"
#include "math.h"

class ButtonReader;
class Screen;
class TempReader;

class App : public Task {
  ButtonReader &btns;
  TempReader   &temp;
  Screen       &scr;
  uint8_t       state;
  temp_t        t_ltc;
  temp_t        t_ballon;
  bool          act_bypass_ballon;
  bool          blink;
  ballon_mode_t ballon_mode;
  dpin_t        ballon_bypass_relay_pin;

public:

  enum {
    S_INIT,
    S_WELCOME,
    S_BALLON_MODE,
    S_CALIBRATE
  };

  App(ButtonReader &btns, TempReader &temp, Screen &scr, dpin_t ballon_bypass_relay_pin) :
    btns(btns), temp(temp), scr(scr), state(S_WELCOME),
    t_ltc(TEMP_INVALID), t_ballon(TEMP_INVALID), act_bypass_ballon(false),
    blink(false), ballon_mode(BALLON_MODE_AUTO),
    ballon_bypass_relay_pin(ballon_bypass_relay_pin)
  {
    refresh();
  }
  
  void setup(){
    temp.setup();
    pinMode(ballon_bypass_relay_pin, OUTPUT);
  }
  
  void    update(uint8_t btn);
  bool    wakeup();
  ulong_t loop(ulong_t ms);
  
private:
  void refresh();
};

#include "buttonreader.h"
#include "screen.h"
#include "types.h"

bool App::wakeup(){
  return btns.button != ButtonReader::NONE ||
         temp.temperatures[0] != TEMP_INVALID ||
         temp.temperatures[1] != TEMP_INVALID;
}

ulong_t App::loop(ulong_t ms){
  temp.take_temp(0, t_ltc);
  temp.take_temp(1, t_ballon);
  switch(ballon_mode) {
  default:
  case BALLON_MODE_AUTO:
    if(!act_bypass_ballon) {
      // lorsque on chauffe le ballon avec le poele:
      // - si le poele est moins chaud que le ballon, on coupe le ballon
      // - si le poele a une température supérieure à 95°C, on coupe le ballon
      act_bypass_ballon = t_ballon >= t_ltc || t_ltc > TEMP(95);
    } else {
      // lorsque on ne chuffe pas le ballon avec le poele:
      // - si le poele est plus chaud de 2,5°C que le ballon
      // - et que le poele a une température inférieure à 95°C, on chauffe le ballon
      act_bypass_ballon = ! (t_ltc > t_ballon + TEMP1(2,5) && t_ltc < TEMP(95));
    }
    break;
  case BALLON_MODE_FORCE_ON:
    act_bypass_ballon = false;
    break;
  case BALLON_MODE_FORCE_OFF:
    act_bypass_ballon = true;
    break;
  }
  digitalWrite(ballon_bypass_relay_pin, act_bypass_ballon ? HIGH : LOW);
  blink = ms % 1000 > 500;
  update(btns.take_button());
  refresh();
  return ms+500;
}

void App::update(uint8_t btn){
  int newstate = -1;
  switch(state) {
    case S_WELCOME:
      if(btn == ButtonReader::LEFT)        newstate = S_WELCOME;
      break;
    case S_BALLON_MODE:
      if(btn == ButtonReader::UP)          ballon_mode = maxi(static_cast<ballon_mode_t>(ballon_mode-1), BALLON_MODE_AUTO);
      if(btn == ButtonReader::DOWN)        ballon_mode = mini(static_cast<ballon_mode_t>(ballon_mode+1), BALLON_MODE_FORCE_OFF);
      break;
    case S_CALIBRATE:
      if(btn == ButtonReader::RIGHT)       newstate = S_CALIBRATE;
      break;
  }
  if (newstate == -1) {
    if(btn == ButtonReader::LEFT)       newstate = state - 1;
    else if(btn == ButtonReader::RIGHT) newstate = state + 1;
  }
  if (newstate != -1) state = newstate;
}

void App::refresh(){
  bool force = (ballon_mode != BALLON_MODE_AUTO);
  switch(state) {
  default:
    //scr.print_calibrate(btns.last_adc, btns.last_btn, blink);
    //scr.print_calibrate(btns.last_run, btns.next_run, blink);
    //break;
  case S_WELCOME:
    scr.print_welcome(t_ltc, t_ballon, force, act_bypass_ballon, blink);
    scr.print_status(force, act_bypass_ballon, blink);
    break;
  case S_BALLON_MODE:
    scr.print_ballon_mode(ballon_mode, force, act_bypass_ballon, blink);
    scr.print_status(force, act_bypass_ballon, blink);
    break;
  case S_CALIBRATE:
    scr.print_calibrate(temp.adcvalues[0], temp.adcvalues[1], temp.adcvalues[3], temp.adcvalues[4], blink);
    break;
  }
  blink = !blink;
}

#endif
