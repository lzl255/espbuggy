#include "led.hpp"

LedColor::LedColor() {}

LedColor::LedColor(bool r, bool g, bool b) : r(r), g(g), b(b) {}

LedColor LedColor::red() { return LedColor(1, 0, 0); }
LedColor LedColor::green() { return LedColor(0, 1, 0); }
LedColor LedColor::blue() { return LedColor(0, 0, 1); }
LedColor LedColor::cyan() { return LedColor(0, 1, 1); }
LedColor LedColor::yellow() { return LedColor(1, 1, 0); }
LedColor LedColor::magenta() { return LedColor(1, 0, 1); }
LedColor LedColor::white() { return LedColor(1, 1, 1); }

Led::Led(DigitalOut pin_red, DigitalOut pin_green, DigitalOut pin_blue)
    : is_on(false), color(LedColor()), pin_red(pin_red), pin_green(pin_green),
      pin_blue(pin_blue) {}

void Led::turn_on() {
  this->pin_red.write(!this->color.r);
  this->pin_green.write(!this->color.g);
  this->pin_blue.write(!this->color.b);
}

void Led::turn_off() {
  this->pin_red.write(1);
  this->pin_green.write(1);
  this->pin_blue.write(1);
}

bool Led::get_is_on() const {
    return this->is_on;
}

void Led::set_is_on(bool is_on) {
  this->is_on = is_on;
  if (is_on)
    this->turn_on();
  else
    this->turn_off();
}

LedColor Led::get_color() const {
    return this->color;
}

void Led::set_color(LedColor color) {
    this->color = color;
    this->turn_on();
}
