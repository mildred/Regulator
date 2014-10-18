#ifndef __regulator_h__
#define __regulator_h__

class Settings;

class Regulator {
  Settings &settings;

public:
  Regulator(Settings &settings);
  
  void refresh(double t_ltc, double t_ballon, bool &act_ballon);

};

#endif
