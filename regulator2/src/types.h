#ifndef __types_h__
#define __types_h__

#include <Arduino.h>

typedef uint8_t apin_t;
typedef uint8_t dpin_t;
typedef unsigned long ulong_t;
//typedef int16_t temp_t;

#define BALLON_MODE_FIRST     0
#define BALLON_MODE_AUTO      0
#define BALLON_MODE_FORCE_ON  1
#define BALLON_MODE_FORCE_OFF 2
#define BALLON_MODE_LAST      2
typedef int ballon_mode_t;

#endif

