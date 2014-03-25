#ifndef __buttonreader_h__
#define __buttonreader_h__

#include "ados.h"

class ButtonReaderCallback {
  public:
  virtual void buttonClicked(int) = 0;
};

class ButtonReader : public AdOSTask2<128> { // 64 won't work
  ButtonReaderCallback *cb;
  int port;
  unsigned wait_msecs;
public:

  enum {
    NONE   = 0,
    RIGHT  = 1,
    UP     = 2,
    DOWN   = 3,
    LEFT   = 4,
    SELECT = 5
  };

  ButtonReader(ButtonReaderCallback *cb, int port, unsigned wait_msecs);
  
  void loop();

private:
  int read(int &adc_key_in);

};

#endif
