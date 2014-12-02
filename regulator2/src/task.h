#ifndef __task_h__
#define __task_h__

#include <Arduino.h>
#include "types.h"

class Task {

public:

  ulong_t last_run;
  ulong_t next_run;

public:

  bool changed;

  Task() : last_run(0), next_run(0), changed(false) {}

  virtual ulong_t loop(ulong_t ms) = 0;
  virtual bool    wakeup(){ return false; };
  
  virtual bool ready(ulong_t ms) {
    if(wakeup()) return true;
    if(next_run >= last_run) {
      // nominal case, wait for next time
      return ms >= next_run;
    } else {
      // next_run overflowed, wait for ms to overflow
      return ms < last_run && ms >= next_run;
    }
  }
  
  virtual void tick(ulong_t ms = millis()) {
    changed = false;
    if(ready(ms)) {
      next_run = loop(ms);
      last_run = ms;
    }
  }
  
};

#endif

