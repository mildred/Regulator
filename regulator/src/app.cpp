#include "app.h"
#include "screen.h"
#include "buttons.h"
#include "settings.h"

App::App(Screen &screen, Settings &settings) : scr(screen), settings(settings), state(S_WELCOME) {
  refresh();
}

int App::getState() const {
  return state;
}

void App::update(int btn){
  int newstate = -1;
  switch(state) {
    case S_WELCOME:
      if(btn == ButtonReader::LEFT)  newstate = S_WELCOME_VANNES;
      else if(btn == ButtonReader::SELECT) newstate = S_MENU_MODE;
      break;
    case S_WELCOME_VANNES:
      if(btn == ButtonReader::RIGHT) newstate = S_WELCOME;
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
      scr.print_welcome(85, 75, true, true);
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
