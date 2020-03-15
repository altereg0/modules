#pragma once

template<board::DigitalPin attached_pin>
class atm_pin_helper {
 public:
  atm_pin_helper(const gpio::FAST_PIN<attached_pin> &pin) : pin(pin) {}
  void set() { pin.set(); };
  void clear() { pin.clear(); }
 private:
  gpio::FAST_PIN<attached_pin> pin;
};
