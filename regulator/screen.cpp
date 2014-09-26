#include <LiquidCrystal.h>
#include "screen.h"

static byte char_deg[8] = {
    B01000,
    B10100,
    B01000,
    B00000,
    B00000,
    B00000,
    B00000
};

static byte char_up[8] = {
    B00000,
    B00100,
    B01010,
    B01010,
    B10001,
    B10001,
    B00000
};

static byte char_dn[8] = {
    B00000,
    B10001,
    B10001,
    B01010,
    B01010,
    B00100,
    B00000
};

static byte char_updn[8] = {
    B00100,
    B01010,
    B10001,
    B00000,
    B10001,
    B01010,
    B00100
};

static byte char_diamond[8] = {
    B00000,
    B00100,
    B01010,
    B10001,
    B01010,
    B00100,
    B00000
};

static byte char_diamond2[8] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B01110,
    B00100,
    B00000
};

#define CHAR_DEG byte(0)
#define CHAR_UP  1
#define CHAR_DN  2
#define CHAR_UPDN  3
#define CHAR_DIAMOND 4
#define CHAR_DIAMOND2 5

Screen::Screen(LiquidCrystal &printer) : printer(printer) {
  printer.createChar(CHAR_DEG, char_deg);
  printer.createChar(CHAR_UP,  char_up);
  printer.createChar(CHAR_DN,  char_dn);
  printer.createChar(CHAR_UPDN,  char_updn);
  printer.createChar(CHAR_DIAMOND,  char_diamond);
  printer.createChar(CHAR_DIAMOND2,  char_diamond2);
  printer.begin(16, 2);
}

void Screen::print_welcome(int t_ltc, int t_ballon, bool ballon, bool radiateur){
  printer.setCursor(0, 0);
  printer.print("LTC    ");
  printf(t_ltc, 3);
  printer.write(CHAR_DEG);
  printer.print("C  ");
  printer.print(ballon    ? "B" : " ");
  printer.print(radiateur ? "R" : " ");
  printer.setCursor(0, 1);
  printer.print("Ballon ");
  printf(t_ballon, 3);
  printer.write(CHAR_DEG);
  printer.print("C <");
  printer.write(CHAR_DIAMOND2);
  printer.print(">");
}

void Screen::print_valves(bool ballon, bool radiateur) {
  printer.setCursor(0, 0);
  printer.print("Ballon    ");
  printer.print(ballon ? "ON " : "OFF");
  printer.print("   ");
  printer.setCursor(0, 1);
  printer.print("Radiateur ");
  printer.print(radiateur ? "ON " : "OFF");
  printer.print("<");
  printer.write(CHAR_DIAMOND2);
  printer.print(">");
}

void Screen::print_string(const char *nom, const char *value, bool modifiable) {
  printer.clear();
  printer.setCursor(0, 0);
  printer.print(nom);
  printer.print(" :");
  printer.setCursor(0, 1);
  if (modifiable) {
    printer.write(CHAR_UPDN);
    printer.setCursor(2, 1);
  }
  printer.print(value);
  printer.setCursor(13, 1);
  printer.print("<");
  printer.write(CHAR_DIAMOND);
  printer.print(">");
}

void Screen::print_temperature(const char *nom, int temp, bool modifiable) {
  printer.clear();
  printer.setCursor(0, 0);
  printer.print("T");
  printer.write(CHAR_DEG);
  printer.setCursor(3, 0);
  printer.print(nom);
  printer.print(" :");
  printer.setCursor(0, 1);
  if (modifiable) {
    printer.write(CHAR_UPDN);
    printer.setCursor(2, 1);
  }
  printer.print(temp);
  printer.write(CHAR_DEG);
  printer.print("C   ");
  printer.setCursor(13, 1);
  printer.print("<");
  printer.write(CHAR_DIAMOND);
  printer.print(">");
}

void Screen::print_menu(const char* cur, const char* sub) {
  printer.setCursor(0, 0);
  printer.print("****************");
  printer.setCursor(2, 0);
  printer.print(" ");
  printer.print(cur);
  printer.print(" ");
  printer.setCursor(0, 1);
  printer.write(CHAR_UP);
  printer.print("            <");
  printer.write(CHAR_DIAMOND2);
  printer.print(">");
  printer.setCursor(3, 1);
  printer.print(sub);
}

void Screen::print_sub_menu(const char* cur, const char* parent) {
  printer.setCursor(0, 0);
  printer.print("****************");
  printer.setCursor(2, 0);
  printer.print(" ");
  printer.print(cur);
  printer.print(" ");
  printer.setCursor(0, 1);
  printer.write(CHAR_DN);
  printer.print("            <");
  printer.write(CHAR_DIAMOND);
  printer.print(">");
  printer.setCursor(3, 1);
  printer.print(parent);
}

void Screen::print_error(const char *error) {
  printer.clear();
  printer.print("Error: ");
  printer.setCursor(0, 1);
  printer.print(error);
}

void Screen::printf(int num, int length){
  unsigned sz = 0;
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  do {
    unsigned long m = num;
    num /= 10;
    *--str = '0' + m - 10 * num;
    sz++;
  } while(num && sz < length);

  while (sz < length) {
    *--str = ' ';
    sz++;
  }
  
  if (num) {
    *str = '*';
  }
  
  printer.write(str);
}
