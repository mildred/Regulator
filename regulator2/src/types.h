#ifndef __types_h__
#define __types_h__

#include <Arduino.h>

typedef uint8_t apin_t;
typedef uint8_t dpin_t;
typedef unsigned long ulong_t;
typedef int16_t temp_t;

enum ballon_mode_t {
  BALLON_MODE_AUTO,
  BALLON_MODE_FORCE_ON,
  BALLON_MODE_FORCE_OFF
};

#define TEMP(x)     ((x) * 100)
#define TEMP1(x, y) ((x) * 100 + (y) * 10)
#define TEMP2(x, y) ((x) * 100 + (y))
#define TEMP_INVALID TEMP2(-101,01)

#endif

