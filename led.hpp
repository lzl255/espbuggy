#pragma once

#include <mbed.h>

class LedColor {
public:
  bool r;
  bool g;
  bool b;

  LedColor();

  LedColor(bool r, bool g, bool b);

  static LedColor red();
  static LedColor green();
  static LedColor blue();
  static LedColor cyan();
  static LedColor yellow();
  static LedColor magenta();
  static LedColor white();
};

class Led {
private:
  bool is_on;
  LedColor color;
  DigitalOut pin_red;
  DigitalOut pin_green;
  DigitalOut pin_blue;

public:
  Led(DigitalOut pin_red, DigitalOut pin_green, DigitalOut pin_blue);

  void turn_on();

  void turn_off();

  bool get_is_on() const;

  void set_is_on(bool is_on);

  LedColor get_color() const;

  void set_color(LedColor color);
};
