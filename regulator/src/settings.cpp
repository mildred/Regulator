#include "settings.h"


int  Settings::targetTemperature() {
  int val = EEPROM.read(0);
  return val;
}

void Settings::targetTemperature(int temp) {
  EEPROM.write(0, temp);
}

int  Settings::preference() {
  int val = EEPROM.read(1);
  return val % 2;
}

void Settings::preference(int p) {
  EEPROM.write(1, p);
}
