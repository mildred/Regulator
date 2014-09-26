#ifndef __app_h__
#define __app_h__

#include <Arduino.h>
#include "buttonreader.h"

class Screen;
class Settings;

class App : public ButtonReaderCallback {
  Screen   &scr;
  Settings &settings;
  int       state;
public:

  enum {
    S_WELCOME,
    S_WELCOME_VANNES,
    S_MENU_MODE,
    S_MENU_TEMP_BALLON
  };

  App(Screen &screen, Settings &settings);
  
  int getState() const;
  void update(int btn);
  
  virtual void buttonClicked(int b) { update(b); }
  
private:
  void setState(int s);
  void refresh();
};

#endif
