#if 0
#ifndef __buttons_h__
#define __buttons_h__

#include <Arduino.h>

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

class Buttons {
public:

  enum {
    RIGHT  = 0,
    UP     = 1,
    DOWN   = 2,
    LEFT   = 3,
    SELECT = 4,
    NONE   = 5
  };

  Buttons();
  
  static int read_LCD_buttons();
  static int read_LCD_buttons(int &adc_key_in);
};

#endif
#endif
