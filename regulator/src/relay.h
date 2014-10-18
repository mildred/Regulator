#ifndef __relay_h__
#define __relay_h__

class Relay {
  int8_t pin    : 5;
  bool   output : 1;
  
public:
  Relay(int8_t pin);
  
  void activate(bool action);
  bool status();

};

#endif
