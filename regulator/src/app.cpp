#include "app.h"
#include "screen.h"
//#include "buttons.h"
#include "settings.h"
#include "relay.h"
#include <math.h>

/*
#define INVALID_TEMP NAN
static double digitalLowPass(double last_smoothed, double new_value, double filterVal)
{
  if(isnan(last_smoothed)) return new_value;
  return (new_value * (1 - filterVal)) + (last_smoothed * filterVal);
}
*/

static int16_t digitalLowPass(int16_t last_smoothed, int16_t new_value, int16_t filterVal)
{
  if(last_smoothed == TEMP_INVALID) return new_value;
  return (new_value * (1 - filterVal)) + (last_smoothed * filterVal);
}

App::App(Screen &screen, Settings &settings, Relay &relay_ballon_bypass, ados_event_t *temp_event) :
  scr(screen), settings(settings), relay_ballon_bypass(relay_ballon_bypass), temp_event(temp_event), state(S_WELCOME), tempLTC(TEMP_INVALID), tempBallon(TEMP_INVALID), regulator(settings)
{
  refresh();
}

void App::setup(){
}

void App::loop() {
/*
  TempReader::temp_event_t* ev = (TempReader::temp_event_t*) ados()->EventWaitFor(temp_event);
  switch(ev->channel) {
  case 0: tempLTC    = digitalLowPass(tempLTC,    ev->temp, 90); break;
  case 1: tempBallon = digitalLowPass(tempBallon, ev->temp, 90); break;
  default: return;
  }
  bool act_ballon;
  regulator.refresh(tempLTC, tempBallon, act_ballon);
  relay_ballon_bypass.activate(act_ballon);
  refresh();
*/
}

int App::getState() const {
  return state;
}

void App::update(int btn){
  int newstate = -1;
  switch(state) {
    case S_WELCOME:
      if(btn == ButtonReader::LEFT)        newstate = S_WELCOME_VANNES;
      else if(btn == ButtonReader::SELECT) newstate = S_MENU_MODE;
      break;
    case S_WELCOME_VANNES:
      if(btn == ButtonReader::RIGHT)       newstate = S_WELCOME;
      else if(btn == ButtonReader::SELECT) newstate = S_MENU_MODE;
      break;
    case S_MENU_MODE:
      if(btn == ButtonReader::LEFT)        newstate = S_MENU_TEMP_BALLON;
      else if(btn == ButtonReader::UP)     settings.preference(settings.preference() + 1);
      else if(btn == ButtonReader::DOWN)   settings.preference(settings.preference() - 1);
      else if(btn == ButtonReader::SELECT) newstate = S_WELCOME;
      break;
    case S_MENU_TEMP_BALLON:
      if(btn == ButtonReader::RIGHT)       newstate = S_MENU_MODE;
      else if(btn == ButtonReader::UP)     settings.targetTemperature(settings.targetTemperature() + 1);
      else if(btn == ButtonReader::DOWN)   settings.targetTemperature(settings.targetTemperature() - 1);
      else if(btn == ButtonReader::SELECT) newstate = S_WELCOME;
      break;
  }
  if (newstate == -1) {
    if(btn == ButtonReader::LEFT)       newstate = state - 1;
    else if(btn == ButtonReader::RIGHT) newstate = state + 1;
  }
  if (newstate != -1 && newstate != state) setState(newstate);
  else refresh();
}

void App::setState(int s) {
  if (s == state) return;
  state = s;
  refresh();
}

void App::refresh() {
  switch(state) {
    case S_WELCOME:
      scr.print_welcome(tempLTC, tempBallon, relay_ballon_bypass.status());
      break;
    case S_WELCOME_VANNES:
      scr.print_valves(true, false);
      break;
    case S_MENU_MODE:
      scr.print_string("Preference", settings.preference() ? "Radiateur" : "Ballon", true);
      break;
    case S_MENU_TEMP_BALLON:
      scr.print_temperature("Ballon", settings.targetTemperature(), true);
      break;
    default:
      scr.print_error("Unknown screen");
  }
}


