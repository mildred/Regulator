#ifndef __tempreader_h__
#define __tempreader_h__

#include <OneWire.h>
#include <Arduino.h>
#include "task.h"
#include "types.h"
#include "config.h"

class TempReader : public Task {
private:
  typedef byte onewire_addr_t[8];
  onewire_addr_t devices[TEMP_NUM_DEVICES];
  uint8_t num_devices_found;
  OneWire &ds;
public:

  float temperatures[TEMP_NUM_DEVICES];

  TempReader(OneWire &bus) :
    num_devices_found(0),
    ds(bus)
  {
    for(int i = 0; i < TEMP_NUM_DEVICES; ++i) {
      changed = false;
      temperatures[i] = NAN;
    }
  }
  
  void setup() {
    int i = 0;
    ds.reset_search();
    
    while(i < TEMP_NUM_DEVICES && ds.search(devices[i])) {
      if(OneWire::crc8(devices[i], 7) != devices[i][7]) continue;
      if(devices[i][0] != 0x10 && devices[i][0] != 0x28) continue;
      i++;
    }

    num_devices_found = i;
  }
  
  ulong_t loop(ulong_t ms) {
    for(int i = 0; i < num_devices_found; ++i) {
      ds.reset();
      ds.select(devices[i]);
      ds.write(0x44); // Start conversion

      ds.reset();
      ds.select(devices[i]);
      ds.write(0xBE); // Read Scratchpad

      byte data[9];
      for (int j = 0; j < 9; j++) { // we need 9 bytes
        data[j] = ds.read();
      }

      byte MSB = data[1];
      byte LSB = data[0];

      float tempRead = ((MSB << 8) | LSB) / 16.0; //using two's compliment
      changed = changed || (tempRead != temperatures[i]);
      temperatures[i] = tempRead;
    }
    
    return ms + 1000;
  }
};

#endif

