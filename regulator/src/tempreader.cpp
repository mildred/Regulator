#include "tempreader.h"
#include <Arduino.h>

TempReader::TempReader(int dportA, int dportB, int aport, unsigned char numChannels) :
  numChannels(numChannels), rtd(dportA, dportB, aport)
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
  Serial.println(' ');
}

