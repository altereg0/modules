#pragma once

<<<<<<< HEAD
class IPin {
public:
    virtual void init() = 0;
    virtual bool read() = 0;
};

template<board::DigitalPin attached_pin>
class atm_pin_helper: public IPin {
public:
    void set() { pin.set(); };

    bool read() override {
        return pin.value();
    }

    void clear() { pin.clear(); }


protected:
    gpio::FAST_PIN<attached_pin> pin;
};

template<board::DigitalPin attached_pin>
class atm_btn_helper: public atm_pin_helper<attached_pin> {
public:

    void init() override {
        pin.set_mode(gpio::PinMode::INPUT_PULLUP);
    }

private:
    gpio::FAST_PIN<attached_pin> pin;
=======
template<board::DigitalPin attached_pin>
class atm_pin_helper {
 public:
  atm_pin_helper(const gpio::FAST_PIN<attached_pin> &pin) : pin(pin) {}
  void set() { pin.set(); };
  void clear() { pin.clear(); }
 private:
  gpio::FAST_PIN<attached_pin> pin;
>>>>>>> master
};
