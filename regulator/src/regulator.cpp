#include "regulator.h"
#include "settings.h"

Regulator::Regulator(Settings &settings) : settings(settings) {
}

void Regulator::refresh(double t_ltc, double t_ballon, bool &act_ballon){
  act_ballon = t_ltc >= settings.targetTemperature() && t_ltc < 95;
}

