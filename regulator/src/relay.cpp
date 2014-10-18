#include "Arduino.h"
#include "relay.h"

Relay::Relay(int8_t pin) : pin(pin), output(false) {
  pinMode(pin, OUTPUT);
}

void Relay::activate(bool action) {
  output = action;
  digitalWrite(pin, action ? HIGH : LOW);
}

bool Relay::status() {
  return output;
}
