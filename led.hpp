#pragma once

#include <mbed.h>

#include "utils.hpp"

class Rgb {
public:
  float r;
  float g;
  float b;

  Rgb();

  Rgb(float r, float g, float b);

  /// Convert from ANSI foreground color code.
  static Rgb ansi_fg(uint8_t ansi_code);
  /// Convert from ANSI background color code.
  static Rgb ansi_bg(uint8_t ansi_code);

  static Rgb black();
  /// ANSI calls it "bright black".
  static Rgb dark_gray();
  /// ANSI calls it "white", and "white" "bright white".
  static Rgb gray();
  /// ANSI calls it "bright white", and "gary" "white".
  static Rgb white();
  static Rgb red();
  static Rgb green();
  static Rgb blue();
  static Rgb cyan();
  static Rgb yellow();
  static Rgb magenta();
  static Rgb bright_red();
  static Rgb bright_green();
  static Rgb bright_blue();
  static Rgb bright_cyan();
  static Rgb bright_yellow();
  static Rgb bright_magenta();
};

class Led {
private:
  bool is_on;
  Rgb color;
  PwmOut red;
  PwmOut green;
  PwmOut blue;

  void update_pins();

public:
  Led(PinName pin_red, PinName pin_green, PinName pin_blue);

  void turn_on();

  void turn_off();

  bool get_is_on() const;

  void set_is_on(bool is_on);

  Rgb get_color() const;

  void set_color(Rgb color);
};
