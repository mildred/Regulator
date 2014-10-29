/*
  RTDSensor.h - Library for openenergymonitor
  Created by Trystan Lea, May 16 2010
  Licenced under GPL
*/

#ifndef RTDModule_h
#define RTDModule_h

#include "Arduino.h"

class RTDModule
{
  public:

    RTDModule(int8_t _dpinA, int8_t _dpinB, int8_t _analogInPin);
    void calibration(int channel, int16_t _calA, int16_t _calB);
    int16_t getTemperature();
    int16_t getTemperatureMili();
    int getADC();
    void setChannel(int channel);

  private:


    int8_t analogInPin;
    int8_t dpinA;
    int8_t dpinB;
    int8_t currentChannel;
    int16_t scale[4];
    int16_t offset[4];

};

#endif
