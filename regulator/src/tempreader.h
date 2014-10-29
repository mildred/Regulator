#ifndef __tempreader_h__
#define __tempreader_h__

#include "ados.h"
#include "RTDModule.h"

class TempReader : public AdOSTask2<96> {
  uint8_t  numChannels;
  uint16_t wait_msecs;

public:
  TempReader(int8_t dportA, int8_t dportB, int8_t aport, uint8_t numChannels, uint16_t wait_msecs);
  
  void setup();
  void loop();

  RTDModule    rtd;
  ados_event_t temp_event;
  
  struct temp_event_t {
    uint8_t channel;
    int16_t temp;
  };
};

#endif
