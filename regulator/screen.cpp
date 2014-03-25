#include <LiquidCrystal.h>
#include "screen.h"

Screen::Screen(LiquidCrystal &printer) : printer(printer) {
  printer.begin(16, 2);
}

void Screen::print_welcome(int t_ltc, int t_ballon, bool ballon, bool radiateur){
  printer.setCursor(0, 0);
  printer.print("LTC:    ");
  printf(t_ltc, 3);
  printer.print("C  ");
  printer.print(ballon    ? "B" : " ");
  printer.print(radiateur ? "R" : " ");
  printer.setCursor(0, 1);
  printer.print("Ballon: ");
  printf(t_ballon, 3);
  printer.print("C    ");
}

void Screen::printf(int num, int length){
  int n = printer.print(num);
  for(int i = 0; i < length - n; ++i) printer.print(" ");
  return;
  
  char str[length+16];
  unsigned i = length;
  
  if (num < 0) {
    str[i++] = '-';
    num = -num;
  }
  
  while (num != 0) {
    int mod = num % 10;
    str[i++] = '0' + mod;
    num = num - mod;
  }
  
  str[i] = 0;
  
  if (i - length < length) {
    for(int j = 0; j < length; ++j) str[j] = ' ';
    printer.print(& str[i - length]);
  } else {
    for(i = 0; i < length; ++i) str[i] = '*';
    str[length] = 0;
    printer.print(str);
  }
}
