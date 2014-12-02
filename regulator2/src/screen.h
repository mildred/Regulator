#ifndef __screen_h__
#define __screen_h__

#include <Arduino.h>
#include "types.h"

class LiquidCrystal;

class Screen {
  LiquidCrystal &printer;
public:

  Screen(LiquidCrystal &printer);
  
  void print_welcome(float t_ltc, float t_ballon, bool force, bool bypass_ballon, bool blink);
  void print_ballon_mode(int8_t mode, bool force, bool bypass_ballon, bool blink);
  void print_status(bool force, bool bypass_ballon, bool blink);
  void print_calibrate(int32_t t1, int32_t t2, int32_t t3, int32_t t4, bool blink);
  void print_valves(bool ballon, bool radiateur);
  void print_string(const char *nom, const char *string, bool modifiable);
  void print_temperature(const char *nom, double temp, bool modifiable);
  void print_menu(const char* cur, const char* sub);
  void print_sub_menu(const char* cur, const char* parent);
  void print_error(const char *error);
  
private:
  void print_temp(float temp, int length);
#if 0
  //void printf(int num, int length);
  //void print_temp(temp_t num, int length);
#endif
};

#endif
