#ifndef __app_h__
#define __app_h__

#include <Arduino.h>
#include "buttonreader.h"
#include "tempreader.h"
#include "regulator.h"
#include "ados.h"

class Screen;
class Settings;
class Relay;

class App : public ButtonReaderCallback, public AdOSTask2<512> {
  Screen       &scr;
  Settings     &settings;
  Relay        &relay_ballon_bypass;
  ados_event_t *temp_event;
  int           state;
  double        tempLTC;
  double        tempBallon;
  Regulator     regulator;
public:

  enum {
    S_WELCOME,
    S_WELCOME_VANNES,
    S_MENU_MODE,
    S_MENU_TEMP_BALLON
  };

  App(Screen &screen, Settings &settings, Relay &relay_ballon_bypass, ados_event_t *temp_event);
  
  void setup();
  void loop();
  
  int getState() const;
  void update(int btn);
  
  void buttonClicked(int b) { update(b); }
  
private:
  void setState(int s);
  void refresh();
};

#endif
