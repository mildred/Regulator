#include "app.h"
#include "screen.h"
#include "buttons.h"

App::App(Screen &screen) : scr(screen), state(S_WELCOME) {
  refresh();
}

int App::getState() const {
  return state;
}

void App::update(int btn){
  switch(state) {
    case S_WELCOME:
      if(btn == ButtonReader::LEFT || btn == ButtonReader::RIGHT) setState(S_WELCOME_VANNES);
      break;
    case S_WELCOME_VANNES:
      if(btn == ButtonReader::LEFT || btn == ButtonReader::RIGHT) setState(S_WELCOME);
      break;
  }
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
      scr.print_welcome(80, 70, false, false);
      break;
  }
}
