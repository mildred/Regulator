#ifndef __app_h__
#define __app_h__

#include <Arduino.h>
#include "buttonreader.h"

class Screen;

class App : public ButtonReaderCallback {
  Screen &scr;
  int     state;
public:

  enum {
    S_WELCOME,
    S_WELCOME_VANNES
  };

  App(Screen &screen);
  
  int getState() const;
  void update(int btn);
  
  virtual void buttonClicked(int b) { update(b); }
  
private:
  void setState(int s);
  void refresh();
};

#endif
