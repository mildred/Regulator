#ifndef __tempreader_h__
#define __tempreader_h__

#include "ados.h"
#include "RTDModule.h"

class TempReader : public AdOSTask2<96> {
  unsigned char numChannels;

public:
  TempReader(int dportA, int dportB, int aport, unsigned char numChannels);
  
  void setup();
  void loop();

  RTDModule    rtd;
  ados_event_t temp_event;
  
  struct temp_event_t {
    int channel;
    double temp;
  };
};

#endif
