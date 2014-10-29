#ifndef __regulator_h__
#define __regulator_h__

#include "temp.h"

class Settings;

class Regulator {
  Settings &settings;

public:
  Regulator(Settings &settings);
  
  void refresh(temp_t t_ltc, temp_t t_ballon, bool &act_ballon);

};

#endif
