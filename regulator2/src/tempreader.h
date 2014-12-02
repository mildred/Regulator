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
  uint8_t next_device;
  OneWire &ds;
public:

  float temperatures[TEMP_NUM_DEVICES];

  TempReader(OneWire &bus) :
    num_devices_found(0),
    next_device(0),
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
    temperatures[next_device] = getTemperature(devices[next_device]);
    next_device = (next_device + 1) % num_devices_found;
    //fetchTemperatures();
    //startConversion();
    return ms + 1000;
  }

private:

  void startConversion() {
    ds.reset();
    for(int i = 0; i < num_devices_found; ++i) {
      ds.select(devices[i]);
      ds.write(0x44); // Start conversion
    }
  }
  
  void fetchTemperatures() {
    byte data[9];
    for(int i = 0; i < num_devices_found; ++i) {
      ds.reset();
      ds.select(devices[i]);
      ds.write(0xBE); // Read Scratchpad

      for (int i = 0; i < 9; i++) { // we need 9 bytes
        data[i] = ds.read();
      }

      byte MSB = data[1];
      byte LSB = data[0];

      float tempRead = ((MSB << 8) | LSB); //using two's compliment
      temperatures[i] = tempRead / 16;
    }
  }

  float getTemperature(onewire_addr_t addr) {
    byte data[9];
    
    ds.reset();
    ds.select(addr);
    ds.write(0x44); // Start conversion

    ds.reset();
    ds.select(addr);
    ds.write(0xBE); // Read Scratchpad

    for (int i = 0; i < 9; i++) { // we need 9 bytes
      data[i] = ds.read();
    }

    byte MSB = data[1];
    byte LSB = data[0];

    float tempRead = ((MSB << 8) | LSB); //using two's compliment
    return tempRead / 16;
  }
};

#endif

