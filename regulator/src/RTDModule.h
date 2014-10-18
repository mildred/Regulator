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

    RTDModule(int _dpinA, int _dpinB, int _analogInPin);
    void calibration(int channel, double _calA, double _calB);
    double getTemperature();
    int getADC();
    void setChannel(int channel);

  private:


    int analogInPin;
    int dpinA;
    int dpinB;
    short  currentChannel;
    double scale[4];
    double offset[4];

};

#endif
