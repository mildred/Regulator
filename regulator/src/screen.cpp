#include <LiquidCrystal.h>
#include "screen.h"
#include "version.h"

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

static byte char_up_small[8] = {
    B00100,
    B01010,
    B10001,
    B00000,
    B00000,
    B00000,
    B00000
};

static byte char_dn_small[8] = {
    B00000,
    B00000,
    B00000,
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

static byte char_ballon[8] = {
    B01110,
    B10001,
    B11111,
    B10001,
    B10001,
    B10001,
    B01110
};

static byte char_radiateur[8] = {
    B11111,
    B10001,
    B11111,
    B10001,
    B11111,
    B10001,
    B11111
};

static byte char_ok[8] = {
    B00000,
    B00000,
    B00001,
    B00010,
    B10100,
    B01000,
    B00000
};

static byte char_ok_blink[8] = {
    B00001,
    B00000,
    B00001,
    B00010,
    B10100,
    B01000,
    B00000
};

static byte char_force[8] = {
    B00000,
    B01110,
    B01000,
    B01100,
    B01000,
    B01000,
    B00000
};

static byte char_force_blink[8] = {
    B00001,
    B01110,
    B01000,
    B01100,
    B01000,
    B01000,
    B00000
};

#define CHAR_DEG byte(0)
#define CHAR_UP  1
#define CHAR_DN  2
#define CHAR_UPDN  3
#define CHAR_DIAMOND 4
#define CHAR_DIAMOND2 5
#define CHAR_BALLON 1
#define CHAR_RADIATEUR 2

Screen::Screen(LiquidCrystal &printer) : printer(printer) {
  printer.createChar(CHAR_DEG, char_deg);
  printer.createChar(CHAR_UP,  char_up);
  printer.createChar(CHAR_DN,  char_dn);
  printer.createChar(CHAR_BALLON,  char_ballon);
  printer.createChar(CHAR_RADIATEUR,  char_radiateur);
  printer.createChar(CHAR_UPDN,  char_updn);
  printer.createChar(CHAR_DIAMOND,  char_diamond);
  printer.createChar(CHAR_DIAMOND2,  char_diamond2);
  printer.begin(16, 2);
}

void Screen::print_welcome(float t_ltc, float t_ballon, bool force, bool bypass_ballon, bool blink){
  printer.createChar(0, char_deg);
  printer.createChar(1, blink ? char_diamond : char_diamond2);
  printer.createChar(2, char_ballon);
  printer.createChar(3, char_radiateur);
  printer.clear();
  printer.setCursor(0, 0);
  printer.print("LTC ");
  print_temp(t_ltc, 5);
  printer.write(CHAR_DEG);
  printer.print("C");
  printer.setCursor(0, 1);
  printer.print("Bal ");
  print_temp(t_ballon, 5);
  printer.write(CHAR_DEG);
  printer.print("C");
  printer.setCursor(14, 1);
  printer.print("<>");
}

void Screen::print_ballon_mode(int8_t mode, bool force, bool bypass_ballon, bool blink) {
  printer.createChar(5, char_updn);
  printer.setCursor(0, 0);
  printer.print("Mode Ballon  ");
  printer.setCursor(0, 1);
  printer.write(5);
  printer.print(' ');
  if(mode == BALLON_MODE_AUTO) {
    printer.print("Auto     ");
  } else if(mode == BALLON_MODE_FORCE_ON) {
    printer.print("Force ON ");
  } else if(mode == BALLON_MODE_FORCE_OFF) {
    printer.print("Force OFF");
  }
  printer.setCursor(14, 1);
  printer.print("<>");
}

void Screen::print_status(bool force, bool bypass_ballon, bool blink) {
  printer.createChar(1, force ? (blink ? char_force_blink : char_force) :
                                (blink ? char_ok_blink    : char_ok   ));
  printer.createChar(2, char_ballon);
  printer.createChar(3, char_radiateur);
  printer.setCursor(13, 0);
  printer.print(bypass_ballon ? " \3\1" : "\2\3\1");
}

void Screen::print_calibrate(int32_t t1, int32_t t2, int32_t t3, int32_t t4, bool blink) {
  printer.createChar(1, blink ? char_diamond : char_diamond2);
  printer.setCursor(0, 0);
  printer.print("                 ");
  printer.setCursor(0, 1);
  printer.print("Calibration  <\1>");
  printer.setCursor(0, 0);
  printer.print(t1);
  printer.print(' ');
  printer.print(t2);
  printer.print(' ');
  printer.print(t3);
  printer.print(' ');
  printer.print(t4);
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

void Screen::print_temperature(const char *nom, double temp, bool modifiable) {
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

const char *stars = "****************";

void Screen::print_menu(const char* cur, const char* sub) {
  printer.setCursor(0, 0);
  printer.print(stars);
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
  printer.print(stars);
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

void Screen::print_version(char vernum) {
  printer.createChar(5, char_up_small);
  printer.createChar(4, char_dn_small);
  printer.clear();
  printer.print("Version:");
  printer.setCursor(0, 1);
  printer.write(vernum == 0 ? 4 : 5);
  printer.print(" ");
  printer.print(vernum == 0 ? VERSION1 : VERSION2);
}

#if 0
void Screen::printf(int num, int length){
  unsigned sz = 0;
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];
  
  bool negative = false;
  if(num < 0) {
    num = -num;
    negative = true;
  }

  *str = '\0';

  do {
    unsigned long m = num;
    num /= 10;
    *--str = '0' + m - 10 * num;
    sz++;
  } while(num && sz < length);
  
  if(negative && sz < length) {
    *--str = '-';
    sz++;
    negative = false;
  }

  while (sz < length) {
    *--str = ' ';
    sz++;
  }
  
  if (num || negative) {
    *str = '*';
  }
  
  printer.write(str);
}

void Screen::print_temp(temp_t num, int length)
{
  int8_t decimals = num % 100;
  temp_t integer  = (num - decimals) / 100;
  printf(integer, length-2);
  int8_t d1 = num % 10;
  int8_t d0 = (decimals - d1) / 100;
  if(d1 >= 5) d0++;
  printer.write('.');
  printer.write('0' + d0);
}
#endif

void Screen::print_temp(float num, int length)
{
  char str[length+4];
  dtostrf(num, length, 1, str);
  str[length] = 0;
  printer.write(str);
}
