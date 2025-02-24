#include <stdint.h>

#include <C12832.h>
#include <QEI.h>
#include <mbed.h>

#include "motor.hpp"
#include "pid.hpp"
#include "ports.hpp"
#include "potentiometer.hpp"
#include "speedmeter.hpp"
#include "wheel.hpp"

template <typename T> T min(T x, T y) {
  if (x > y)
    return y;
  else
    return x;
}

template <typename T> T max(T x, T y) {
  if (x > y)
    return x;
  else
    return y;
}

template <typename T> T abs(T x) {
  if (x >= 0)
    return x;
  else
    return -x;
}

enum WheelSide {
  LEFT_WHEEL,
  RIGHT_WHEEL,
};

class Buggy {
  C12832 lcd;
  Ticker display_refresh_ticker;
  Wheel *wheel_left;
  Wheel *wheel_right;

public:
  Buggy()
      : lcd(D11, D13, D12, D7, D10), display_refresh_ticker(),
        wheel_left(Wheel::left()), wheel_right(Wheel::right()) {
    wheel_left->set_power_cap(0.5f);
    wheel_right->set_power_cap(0.5f);
    wheel_left->set_target_speed(0.04f);
    wheel_right->set_target_speed(0.04f);
    wheel_left->reset_distance();
    wheel_right->reset_distance();
    display_refresh_ticker.attach(
        callback(this, &Buggy::display_refresh_callback), 1.f / 12.f);
  }

  void display_refresh_callback() {
    this->lcd.cls();
    this->lcd.locate(0, 0);
    this->lcd.printf("D:%.3f/%.3f", this->wheel_left->get_distance(),
                     this->wheel_right->get_distance());
  }

  void both_wheels(float distance_left, float distance_right) {
      wheel_left->clear_pid();
      wheel_right->clear_pid();
    wheel_left->reset_distance();
    wheel_right->reset_distance();
    wheel_left->set_target_speed((distance_left < 0) ? -0.04f : 0.04f);
    wheel_right->set_target_speed((distance_right < 0) ? -0.04f : 0.04f);
    for (;;) {
      if (abs(wheel_left->get_distance()) >= abs(distance_left))
        wheel_left->set_target_speed(0.);
      if (abs(wheel_right->get_distance()) >= abs(distance_right))
        wheel_right->set_target_speed(0.);
      if (wheel_left->get_target_speed() == 0. &&
          wheel_right->get_target_speed() == 0.) {
        break;
      }
    }
  }

  void one_wheel(WheelSide wheel_side, float distance) {
    Wheel *wheel;
    switch (wheel_side) {
    case LEFT_WHEEL:
      wheel = this->wheel_left;
      break;
    case RIGHT_WHEEL:
      wheel = this->wheel_right;
      break;
    }
    wheel->clear_pid();
    wheel->reset_distance();
    wheel->set_target_speed((distance > 0) ? 0.04f : -0.04f);
    while (abs(wheel->get_distance()) < abs(distance))
      ;
    wheel->set_target_speed(0.f);
  }
};

int main() {
  // Indicator for code is running.
  PwmOut green_led(D9); // D9 -> mbed application shield LED green
  green_led.period(.4f);
  green_led.write(.5f);

  C12832 lcd(D11, D13, D12, D7, D10);

  // The enable pin of the drive board.
  DigitalOut drive_board_enable(ports::BOARD_ENABLE);
  drive_board_enable.write(true);

  const float TURN_90 = 0.0073;

  Buggy buggy;
  buggy.both_wheels(0.01f, 0.01f);
  wait(0.2f);
  buggy.one_wheel(RIGHT_WHEEL, TURN_90);
  wait(0.2f);
  buggy.both_wheels(0.01f, 0.01f);
  wait(0.2f);
  buggy.one_wheel(RIGHT_WHEEL, TURN_90);
  wait(0.2f);
  buggy.both_wheels(0.01f, 0.01f);
  wait(0.2f);
  buggy.one_wheel(RIGHT_WHEEL, TURN_90);
  wait(0.2f);
  buggy.both_wheels(0.01f, 0.01f);

  wait(0.2f);
  buggy.both_wheels(-TURN_90, TURN_90);

  wait(0.2f);
  buggy.both_wheels(0.01f, 0.01f);
  wait(0.2f);
  buggy.one_wheel(LEFT_WHEEL, TURN_90);
  wait(0.2f);
  buggy.both_wheels(0.01f, 0.01f);
  wait(0.2f);
  buggy.one_wheel(LEFT_WHEEL, TURN_90);
  wait(0.2f);
  buggy.both_wheels(0.01f, 0.01f);
  wait(0.2f);
  buggy.one_wheel(LEFT_WHEEL, TURN_90);
  wait(0.2f);
  buggy.both_wheels(0.01f, 0.01f);

  drive_board_enable.write(false);
}