#include "regulator.h"
#include "settings.h"

Regulator::Regulator(Settings &settings) : settings(settings) {
}

void Regulator::refresh(temp_t t_ltc, temp_t t_ballon, bool &act_ballon){
  act_ballon = t_ltc >= settings.targetTemperature() && t_ltc < TEMP_95C;
}

