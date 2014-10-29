#ifndef __buttonreader_h__
#define __buttonreader_h__

#include <Arduino.h>
#include "task.h"
#include "types.h"

class ButtonReader : public Task {
public:

  apin_t  port;
  uint8_t last_btn;
  int     last_adc;

public:

  ButtonReader(apin_t input) : port(input), last_adc(0) {}

  enum {
    NONE   = 0,
    RIGHT  = 1,
    UP     = 2,
    DOWN   = 4,
    LEFT   = 8,
    SELECT = 16
  };
  
  uint8_t button;
  
  uint8_t take_button() {
    if(button & RIGHT ) { button &= ~RIGHT ; return RIGHT ; }
    if(button & UP    ) { button &= ~UP    ; return UP    ; }
    if(button & DOWN  ) { button &= ~DOWN  ; return DOWN  ; }
    if(button & LEFT  ) { button &= ~LEFT  ; return LEFT  ; }
    if(button & SELECT) { button &= ~SELECT; return SELECT; }
    return NONE;
  }
  
  ulong_t loop(ulong_t ms) {
    int adc = analogRead(port);
    if(last_btn == 0) {
      // read button and wait 5ms
      last_adc = adc;
      last_btn = analog_to_button(adc);
      return ms + 5;
    } else if(abs(adc - last_adc) < 5) {
      // wait 5ms for button release
      return ms + 5;
    } else {
      // button released, register, wait 50ms for next input
      button |= last_btn;
      last_btn = 0;
      return ms + 50;
    }
  }

private:

  uint8_t analog_to_button(int analog_value) {
    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    if (analog_value > 1000) return NONE; // We make this the 1st option for speed reasons since it will be the most likely result
    if (analog_value < 50)   return RIGHT;  
    if (analog_value < 195)  return UP; 
    if (analog_value < 380)  return DOWN; 
    if (analog_value < 555)  return LEFT; 
    if (analog_value < 790)  return SELECT;   
    return NONE;  // when all others fail, return this...
  }
};

#endif

