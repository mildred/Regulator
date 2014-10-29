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
RTDModule::RTDModule(int8_t _dpinA, int8_t _dpinB, int8_t _analogInPin)
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
void RTDModule::calibration(int channel, int16_t _calA, int16_t _calB)
{
    scale[channel]  = _calA;
    offset[channel] = _calB;
}

//-----------------------------------------------------------------------
// Read temperature
//-----------------------------------------------------------------------
int16_t RTDModule::getTemperature()
{
    return (static_cast<int32_t>(scale[currentChannel]) * static_cast<int32_t>(analogRead(analogInPin)))
           + offset[currentChannel]; 
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
    currentChannel = channel;

    switch(channel) {
    default:
    case 0: digitalWrite(dpinA, LOW);  digitalWrite(dpinB, LOW);  break;
    case 1: digitalWrite(dpinA, HIGH); digitalWrite(dpinB, LOW);  break;
    case 2: digitalWrite(dpinA, LOW);  digitalWrite(dpinB, HIGH); break;
    case 3: digitalWrite(dpinA, HIGH); digitalWrite(dpinB, HIGH); break;
    }
}
//-----------------------------------------------------------------------
