/*
  RTDSensor.cpp - Library for openenergymonitor
  Created by Trystan Lea, April 27 2010
  Licenced under GPL
*/

#include "Arduino.h"
#include "RTDModule.h"

//-----------------------------------------------------------------------
// Set module pins
//-----------------------------------------------------------------------
RTDModule::RTDModule(int _dpinA, int _dpinB, int _analogInPin)
{
    analogInPin = _analogInPin;
    dpinA = _dpinA;
    dpinB = _dpinB;

    pinMode(dpinA, OUTPUT);  
    pinMode(dpinB, OUTPUT);  

    currentChannel = 0;
   
    for(int i = 0; i < 4; ++i) {
        scale[i]  = 1.0;
        offset[i] = 1.0;
    }
}

//-----------------------------------------------------------------------
// Set calibration values
//
// Calibration needs to be set for every RTD input on the multiplexor
//-----------------------------------------------------------------------
void RTDModule::calibration(int channel, double _calA, double _calB)
{
    scale[channel] = _calA;
    offset[channel] = _calB;
}

//-----------------------------------------------------------------------
// Read temperature
//-----------------------------------------------------------------------
double RTDModule::getTemperature()
{
    double temperature = scale[currentChannel] * analogRead(analogInPin) + offset[currentChannel]; 
    return temperature;
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// RAW analogread
//-----------------------------------------------------------------------
int RTDModule::getADC()
{
    return analogRead(analogInPin);
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// setChannel is used to select the multiplexor channel to be read from
//-----------------------------------------------------------------------
void RTDModule::setChannel(int channel)
{
    int pinAval, pinBval;

    currentChannel = channel;

    switch(channel) {
    default:
    case 0: pinAval = LOW;  pinBval = LOW;  break;
    case 1: pinAval = HIGH; pinBval = LOW;  break;
    case 2: pinAval = LOW;  pinBval = HIGH; break;
    case 3: pinAval = HIGH; pinBval = HIGH; break;
    }
    
    digitalWrite(dpinA, pinAval);
    digitalWrite(dpinB, pinBval);
}
//-----------------------------------------------------------------------
