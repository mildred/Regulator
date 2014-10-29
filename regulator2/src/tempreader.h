#ifndef __tempreader_h__
#define __tempreader_h__

#include <Arduino.h>
#include "task.h"
#include "types.h"

#define TEMP_CAL_MIN_DEG TEMP1(19,8)
#define TEMP_CAL_MAX_DEG TEMP1(88,8)
#define TEMP_CAL_MIN_VAL 354
#define TEMP_CAL_MAX_VAL 565

//#define TEMP_PRECISION ((TEMP_CAL_MAX_DEG-TEMP_CAL_MIN_DEG) / (TEMP_CAL_MAX_VAL-TEMP_CAL_MIN_VAL))
//#define TEMP_ZERO      (TEMP_CAL_MIN_DEG - TEMP_CAL_MIN_VAL * TEMP_PRECISION)

#define TEMP_PRECISION TEMP2(0,32)
#define TEMP_ZERO      TEMP2(-93,23)


class TempReader : public Task {
  dpin_t  out_a;
  dpin_t  out_b;
  apin_t  input;
  int8_t  cur_chan;
  int8_t  cur_sample;
  temp_t  samples[16];
public:

  TempReader(dpin_t out_a, dpin_t out_b, apin_t input) :
    out_a(out_a), out_b(out_b), input(input), cur_chan(-1), cur_sample(0)
  {
    temperatures[0] = TEMP_INVALID;
    temperatures[1] = TEMP_INVALID;
    temperatures[2] = TEMP_INVALID;
    temperatures[3] = TEMP_INVALID;
  }

  int32_t adcvalues[4];
  temp_t temperatures[4];
  
  void setup(){
    pinMode(out_a, OUTPUT);
    pinMode(out_b, OUTPUT);
  }
  
  void take_temp(uint8_t channel, temp_t &tempvar) {
    if(temperatures[channel] != TEMP_INVALID) {
      tempvar = temperatures[channel];
      temperatures[channel] = TEMP_INVALID;
    }
  }
  
  ulong_t loop(ulong_t ms) {
    if(cur_chan >= 0 && cur_chan < 4) {
      if(cur_sample < 16) {
        samples[cur_sample++] = read_adc();
      } else {
        temperatures[cur_chan] = read_temperature(adcvalues[cur_chan]);
        cur_sample = 0;
        cur_chan = (cur_chan+1) % 4;
      }
    } else {
      cur_chan = 0;
      cur_sample = 0;
    }
    write_channel();
    if(cur_sample == 0)
      return ms + 50;
    else
      return ms + 5;
  }

private:

  temp_t read_adc() {
    return analogRead(input);
  }

  temp_t read_temperature(int32_t &rawval) {
    int32_t avg = 0;
    for(int i = 0; i < 16; ++i) {
      avg += samples[i];
    }
    avg = avg / 16;
    rawval = avg;
    return avg * static_cast<int32_t>(TEMP_PRECISION) + TEMP_ZERO;
  }

  void write_channel() {
    //digitalWrite(out_a, (cur_chan%2 == 0) ? LOW : HIGH); delay(5); digitalWrite(out_b, LOW); delay(5);
    //return;
    switch(cur_chan) {
    default:
    case 0: digitalWrite(out_a, LOW);  digitalWrite(out_b, LOW);  break;
    case 1: digitalWrite(out_a, HIGH); digitalWrite(out_b, LOW);  break;
    case 2: digitalWrite(out_a, LOW);  digitalWrite(out_b, HIGH); break;
    case 3: digitalWrite(out_a, HIGH); digitalWrite(out_b, HIGH); break;
    }
  }
};

#endif

