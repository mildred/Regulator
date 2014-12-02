#ifndef __app_h__
#define __app_h__

#include <Arduino.h>
#include "task.h"
#include "math.h"
#include "tempreader.h"

class ButtonReader;
class Screen;

class App : public Task {
  ButtonReader  &btns;
  TempReader    &temp;
  Screen        &scr;
  uint8_t        state;
  float          t_ltc;
  float          t_ballon;
  bool           act_bypass_ballon;
  long           last_act_time;
  bool           blink;
  ballon_mode_t  ballon_mode;
  dpin_t         ballon_bypass_relay_pin;

public:

  enum {
    S_INIT,
    S_WELCOME,
    S_BALLON_MODE,
    S_CALIBRATE
  };

  App(ButtonReader &btns, TempReader &temp, Screen &scr, dpin_t ballon_bypass_relay_pin) :
    btns(btns), temp(temp), scr(scr), state(S_WELCOME),
    t_ltc(NAN), t_ballon(NAN), act_bypass_ballon(false),
    last_act_time(0), blink(false), ballon_mode(BALLON_MODE_AUTO),
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
  return btns.button != ButtonReader::NONE   ||
         t_ltc       != temp.temperatures[0] ||
         t_ballon    != temp.temperatures[1] ;
}

ulong_t App::loop(ulong_t ms){
  t_ltc    = temp.temperatures[0];
  t_ballon = temp.temperatures[1];
  
  // Compute action to take every 10 seconds only
  if(last_act_time + 10000 < ms || last_act_time > ms) {
    bool new_act_bypass_ballon = last_act_time;  
    switch(ballon_mode) {
    default:
    case BALLON_MODE_AUTO:
      if(t_ltc < 30.0) {
        // Si la température LTC est basse (poele pas en fonction)
        // Mettre au repos la vanne
        // TODO: vérifier si la température descend depuis 15 minutes, mettre au
        // repos la vanne, si la température monte, revenir a un fonctionnement
        // normal.
        new_act_bypass_ballon = false;
      } else if(!act_bypass_ballon) {
        // lorsque on chauffe le ballon avec le poele:
        // - si le poele est moins chaud que le ballon, on coupe le ballon
        // - si le poele a une température supérieure à 95°C, on coupe le ballon
        new_act_bypass_ballon = t_ballon >= t_ltc || t_ltc > 95.0;
      } else {
        // lorsque on ne chuffe pas le ballon avec le poele:
        // - si le poele est plus chaud de 5°C que le ballon
        // - et que le poele a une température inférieure à 95°C, on chauffe le ballon
        new_act_bypass_ballon = ! (t_ltc > t_ballon + 4.0 && t_ltc < 95.0);
      }
      break;
    case BALLON_MODE_FORCE_ON:
      new_act_bypass_ballon = false;
      break;
    case BALLON_MODE_FORCE_OFF:
      new_act_bypass_ballon = true;
      break;
    }
    if(new_act_bypass_ballon != act_bypass_ballon) {
      act_bypass_ballon = new_act_bypass_ballon;
      digitalWrite(ballon_bypass_relay_pin, new_act_bypass_ballon ? HIGH : LOW);
      last_act_time = ms;
    }
  }
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
      if(btn == ButtonReader::UP)          ballon_mode = maxi(ballon_mode-1, BALLON_MODE_FIRST);
      if(btn == ButtonReader::DOWN)        ballon_mode = mini(ballon_mode+1, BALLON_MODE_LAST);
      if(btn == ButtonReader::RIGHT)       newstate = S_BALLON_MODE;
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

  }
  blink = !blink;
}

#endif
