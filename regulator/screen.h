#ifndef __screen_h__
#define __screen_h__

#include <Arduino.h>

class LiquidCrystal;

class Screen {
  LiquidCrystal &printer;
public:

  Screen(LiquidCrystal &printer);
  
  void print_welcome(int t_ltc, int t_ballon, bool ballon, bool radiateur);
  
private:
  void printf(int num, int length);
};

#endif
