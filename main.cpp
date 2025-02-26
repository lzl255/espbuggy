#include <stdint.h>

#include <C12832.h>
#include <QEI.h>
#include <mbed.h>

#include "led.hpp"
#include "motor.hpp"
#include "pid.hpp"
#include "ports.hpp"
#include "potentiometer.hpp"
#include "speedmeter.hpp"
#include "utils.hpp"
#include "wheel.hpp"

enum WheelSide { LEFT_WHEEL, RIGHT_WHEEL };

class Buggy {
  C12832 lcd;
  Led led;
  Ticker display_refresh_ticker;
  Wheel *wheel_left;
  Wheel *wheel_right;

public:
  Buggy()
      : lcd(D11, D13, D12, D7, D10), led(D5, D9, D8), display_refresh_ticker(),
        wheel_left(Wheel::left()), wheel_right(Wheel::right()) {
    wheel_left->set_power_cap(0.5f);
    wheel_right->set_power_cap(0.5f);
    display_refresh_ticker.attach(
        callback(this, &Buggy::display_refresh_callback), 1.f / 12.f);
  }

  ~Buggy() {
    delete this->wheel_left;
    delete this->wheel_right;
  }

  Led *get_led() { return &this->led; }

  const Led *get_led() const { return &this->led; }

  void display_refresh_callback() {
    this->lcd.cls();
    this->lcd.locate(0, 0);
    this->lcd.printf("D:%.3f/%.3f", this->wheel_left->get_distance(),
                     this->wheel_right->get_distance());
  }

  void both_wheels(float distance_left, float distance_right,
                   float speed = 0.03f, bool do_overshot_correction = false) {
    wheel_left->reset_distance();
    wheel_right->reset_distance();
    wheel_left->set_target_speed((distance_left < 0) ? -speed : speed);
    wheel_right->set_target_speed((distance_right < 0) ? -speed : speed);
    for (;;) {
      if (abs(wheel_left->get_distance()) >= abs(distance_left))
        wheel_left->set_target_speed(0.);
      if (abs(wheel_right->get_distance()) >= abs(distance_right))
        wheel_right->set_target_speed(0.);
      if (is_zero(wheel_left->get_target_speed()) &&
          is_zero(wheel_right->get_target_speed()))
        break;
    }

    // Compensate for under/overshooting.
    if (do_overshot_correction) {
      float overshot_distance_left = wheel_left->get_distance() - distance_left;
      float overshot_distance_right =
          wheel_right->get_distance() - distance_right;
      if (abs(overshot_distance_left) <= 0.0005f)
        overshot_distance_left = 0.f;
      if (abs(overshot_distance_right) <= 0.0005f)
        overshot_distance_right = 0.f;
      if (overshot_distance_left != 0.f && overshot_distance_right != 0.f) {
        this->get_led()->set_color(LedColor::red());
        this->both_wheels(-overshot_distance_left, -overshot_distance_right,
                          speed * 0.75f, false);
      }
    }
  }

  void one_wheel(WheelSide wheel_side, float distance, float speed = 0.03f,
                 bool do_overshot_correction = true) {
    Wheel *wheel;
    switch (wheel_side) {
    case LEFT_WHEEL:
      wheel = this->wheel_left;
      break;
    case RIGHT_WHEEL:
      wheel = this->wheel_right;
      break;
    }
    wheel->reset_distance();
    wheel->set_target_speed((distance < 0) ? -speed : speed);
    while (abs(wheel->get_distance()) < abs(distance))
      ;
    wheel->set_target_speed(0.f);

    if (do_overshot_correction) {
      float overshot_distance = wheel->get_distance() - distance;
      if (abs(overshot_distance) >= 0.0005f) {
        this->get_led()->set_color(LedColor::red());
        this->one_wheel(wheel_side, -overshot_distance, speed * 0.75f, false);
      }
    }
  }
};

int main() {
  // The enable pin of the drive board.
  DigitalOut drive_board_enable(ports::BOARD_ENABLE);
  drive_board_enable.write(true);

  const float TURN_90 = 0.0085f;
  const float TURN_180 = 0.0075f;
  const float HALF_METER = 0.01f;

  Buggy buggy;

  buggy.get_led()->turn_on();

  for (uint32_t i = 0; i < 4; ++i) {
    buggy.get_led()->set_color(LedColor::cyan());
    buggy.both_wheels(HALF_METER, HALF_METER);
    wait(.1f);
    if (i != 3) {
      buggy.get_led()->set_color(LedColor::magenta());
      buggy.one_wheel(RIGHT_WHEEL, TURN_90);
      wait(.1f);
    }
  }

  buggy.get_led()->set_color(LedColor::yellow());
  buggy.both_wheels(-TURN_180, TURN_180);
  wait(.1f);

  for (uint32_t i = 0; i < 4; ++i) {
    buggy.get_led()->set_color(LedColor::cyan());
    buggy.both_wheels(HALF_METER, HALF_METER);
    wait(.1f);
    if (i != 3) {
      buggy.get_led()->set_color(LedColor::magenta());
      buggy.one_wheel(LEFT_WHEEL, TURN_90);
      wait(.1f);
    }
  }

  buggy.get_led()->set_color(LedColor::green());
  drive_board_enable.write(false);
}
