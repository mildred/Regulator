#ifndef __settings_h__
#define __settings_h__

#include <EEPROM.h>

class Settings {
public:

  int  targetTemperature();
  void targetTemperature(int temp);
  
  int  preference();
  void preference(int p);

};

#endif
