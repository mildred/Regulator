#ifndef __screen_h__
#define __screen_h__

#include <Arduino.h>

class LiquidCrystal;

class Screen {
  LiquidCrystal &printer;
public:

  Screen(LiquidCrystal &printer);
  
  void print_welcome(double t_ltc, double t_ballon, bool bypass_ballon);
  void print_valves(bool ballon, bool radiateur);
  void print_string(const char *nom, const char *string, bool modifiable);
  void print_temperature(const char *nom, double temp, bool modifiable);
  void print_menu(const char* cur, const char* sub);
  void print_sub_menu(const char* cur, const char* parent);
  void print_error(const char *error);
  
private:
  void printf(int num, int length);
  void printf(double num, int length, int numDecimals);
};

#endif
