#include "tempreader.h"
#include <Arduino.h>

TempReader::TempReader(int8_t dportA, int8_t dportB, int8_t aport, uint8_t numChannels, uint16_t wait_msecs) :
  numChannels(numChannels), wait_msecs(wait_msecs), rtd(dportA, dportB, aport)
{
}

void TempReader::setup(){
  //Serial.begin(9600);
}

void TempReader::loop() {
  temp_event_t evdata;
  for(evdata.channel = 0; evdata.channel < numChannels; ++evdata.channel) {
    rtd.setChannel(evdata.channel);
    ados()->Sleep(10);
    //delay(10);
    evdata.temp = rtd.getTemperature();
    int adc = rtd.getADC();
    /*
    Serial.print(evdata.channel);
    Serial.print(' ');
    Serial.print(adc);
    Serial.print(' ');
    Serial.print(evdata.temp);
    Serial.print(' ');
    */
    ados()->EventPulse(&temp_event, &evdata);
  }
  ados()->Sleep(wait_msecs);
  Serial.println(' ');
}

