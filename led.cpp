#include "led.hpp"
#include "utils.hpp"
#include <cstdint>

Rgb::Rgb() {}

Rgb::Rgb(float r, float g, float b) : r(r), g(g), b(b) {}

Rgb Rgb::black() { return Rgb(0.f, 0.f, 0.f); }
Rgb Rgb::dark_gray() { return Rgb(.33f, .33f, .33f); }
Rgb Rgb::gray() { return Rgb(.67f, .67f, .67f); }
Rgb Rgb::white() { return Rgb(1.f, 1.f, 1.f); }
Rgb Rgb::red() { return Rgb(1.f, 0.f, 0.f); }
Rgb Rgb::green() { return Rgb(0.f, 1.f, 0.f); }
Rgb Rgb::blue() { return Rgb(0.f, 0.f, 1.f); }
Rgb Rgb::cyan() { return Rgb(0.f, 1.f, 1.f); }
Rgb Rgb::yellow() { return Rgb(1.f, 1.f, 0.f); }
Rgb Rgb::magenta() { return Rgb(1.f, 0.f, 1.f); }
Rgb Rgb::bright_red() { return Rgb(1.f, 0.f, 0.f); }
Rgb Rgb::bright_green() { return Rgb(.67f, 1.f, .67f); }
Rgb Rgb::bright_blue() { return Rgb(.67f, .67f, 1.f); }
Rgb Rgb::bright_cyan() { return Rgb(.67f, 1.f, 1.f); }
Rgb Rgb::bright_yellow() { return Rgb(1.f, 1.f, .67f); }
Rgb Rgb::bright_magenta() { return Rgb(1.f, .67f, 1.f); }

Rgb Rgb::ansi_fg(uint8_t ansi_code) {
  switch (ansi_code) {
  case 30:
    return Rgb::black();
  case 31:
    return Rgb::red();
  case 32:
    return Rgb::green();
  case 33:
    return Rgb::yellow();
  case 34:
    return Rgb::blue();
  case 35:
    return Rgb::magenta();
  case 36:
    return Rgb::cyan();
  case 37:
    return Rgb::gray();
  case 90:
    return Rgb::dark_gray();
  case 91:
    return Rgb::bright_red();
  case 92:
    return Rgb::bright_green();
  case 93:
    return Rgb::bright_yellow();
  case 94:
    return Rgb::bright_blue();
  case 95:
    return Rgb::bright_magenta();
  case 96:
    return Rgb::bright_cyan();
  case 97:
    return Rgb::white();
  default:
    return Rgb::black();
  }
}

Rgb Rgb::ansi_bg(uint8_t ansi_code) {
  return Rgb::ansi_fg(ansi_code - 10);
}

Led::Led(PinName pin_red, PinName pin_green, PinName pin_blue)
    : is_on(false), color(Rgb()), red(pin_red), green(pin_green),
      blue(pin_blue) {
  float hz = 60.f;
  this->red.period(1.f / hz);
  this->green.period(1.f / hz);
  this->blue.period(1.f / hz);
}

void Led::update_pins() {
  this->red.write(1.f - this->color.r);
  this->green.write(1.f - this->color.g);
  this->blue.write(1.f - this->color.b);
}

void Led::turn_on() {
  if (!this->is_on) {
    this->is_on = true;
    this->update_pins();
  }
}

void Led::turn_off() {
  if (this->is_on) {
    this->is_on = false;
    this->red.write(1.f);
    this->green.write(1.f);
    this->blue.write(1.f);
  }
}

bool Led::get_is_on() const { return this->is_on; }

void Led::set_is_on(bool is_on) {
  if (is_on)
    this->turn_on();
  else
    this->turn_off();
}

Rgb Led::get_color() const { return this->color; }

void Led::set_color(Rgb color) {
  this->color = color;
  this->update_pins();
}
