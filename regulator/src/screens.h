#ifndef __screens_h__
#define __screens_h__

#include <Arduino.h>
#include "task.h"
#include "math.h"

class ButtonReader;
class Screen;
class App;

class Screens : public Task {
  ButtonReader  &btns;
  App           &app;
  Screen        &scr;
  int            state;
  bool           blink;
  char           vernum;

public:

  enum {
    S_INIT,
    S_WELCOME,
    S_BALLON_MODE,
    S_VERSION
  };

  Screens(ButtonReader &btns, App &app, Screen &scr) :
    btns(btns), app(app), scr(scr), state(S_WELCOME), blink(false), vernum(0)
  {
    refresh();
  }
  
  void    update(uint8_t btn);
  bool    wakeup();
  ulong_t loop(ulong_t ms);
  
private:
  void refresh();
};

#include "buttonreader.h"
#include "screen.h"
#include "app.h"

bool Screens::wakeup(){
  return btns.button != ButtonReader::NONE   ||
         app.temp.changed                    ||
         app.changed                         ;
}

ulong_t Screens::loop(ulong_t ms){
  blink = ms % 1000 > 500;
  update(btns.take_button());
  refresh();
  return ms+500;
}

void Screens::update(uint8_t btn){
  int newstate = -1;
  switch(state) {
    case S_WELCOME:
      if(btn == ButtonReader::LEFT)  newstate = S_WELCOME;
      break;
    case S_BALLON_MODE:
      if(btn == ButtonReader::UP)    app.ballon_mode = maxi(app.ballon_mode-1, BALLON_MODE_FIRST);
      if(btn == ButtonReader::DOWN)  app.ballon_mode = mini(app.ballon_mode+1, BALLON_MODE_LAST);
      break;
    case S_VERSION:
      if(btn == ButtonReader::UP)    vernum = 0;
      if(btn == ButtonReader::DOWN)  vernum = 1;
      if(btn == ButtonReader::RIGHT) newstate = S_VERSION;
      break;
  }
  if (newstate == -1) {
    if(btn == ButtonReader::LEFT)       newstate = state - 1;
    else if(btn == ButtonReader::RIGHT) newstate = state + 1;
  }
  if (newstate != -1) state = newstate;
}

void Screens::refresh(){
  bool force = (app.ballon_mode != BALLON_MODE_AUTO);
  switch(state) {
  default:
  case S_WELCOME:
    scr.print_welcome(app.t_ltc(), app.t_ballon(), force, !app.act_ballon, blink);
    scr.print_status(force, !app.act_ballon, blink);
    break;
  case S_BALLON_MODE:
    scr.print_ballon_mode(app.ballon_mode, force, !app.act_ballon, blink);
    scr.print_status(force, !app.act_ballon, blink);
    break;
  case S_VERSION:
    scr.print_version(vernum);
    scr.print_status(force, !app.act_ballon, blink);
    break;
  }
  blink = !blink;
}

#endif
