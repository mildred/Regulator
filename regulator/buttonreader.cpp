#include "buttonreader.h"
#include <Arduino.h>

ButtonReader::ButtonReader(ButtonReaderCallback *cb, int port, unsigned wait_msecs) : cb(cb), port(port), wait_msecs(wait_msecs) {
}

void ButtonReader::loop() {
  int adc_key_in;
  int last_btn = read(adc_key_in);
  if(cb && last_btn != NONE) cb->buttonClicked(last_btn);
  ados()->Sleep(wait_msecs);
}

int ButtonReader::read(int &adc_key_in) {
  adc_key_in = analogRead(port);      // read the value from the sensor 

  // debounce and wait for button release
  do {
    ados()->Sleep(5);
  } while(abs(analogRead(port) - adc_key_in) < 5);
  
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return NONE; // We make this the 1st option for speed reasons since it will be the most likely result
  if (adc_key_in < 50)   return RIGHT;  
  if (adc_key_in < 195)  return UP; 
  if (adc_key_in < 380)  return DOWN; 
  if (adc_key_in < 555)  return LEFT; 
  if (adc_key_in < 790)  return SELECT;   
  return NONE;  // when all others fail, return this...
}
