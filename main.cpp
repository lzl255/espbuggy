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

class TickerTest {
private:
  DigitalOut red_led;
  Ticker ticker;

private:
  void ticker_callback() {
    if (this->red_led.read() == 0)
      this->red_led.write(1);
    else
      this->red_led.write(0);
  }

public:
  TickerTest() : red_led(D5), ticker() {
    this->ticker.attach(callback(this, &TickerTest::ticker_callback), 0.5f);
  }
};

static inline float ticks_to_speed(int d_ticks, float seconds) {
  float tick_rate = (float)d_ticks / seconds;
  return tick_rate * WHEEL_DIAMETER * PI / (512 * GEAR_RATIO);
}

int main() {
  // Indicator for code is running.
  PwmOut green_led(D9); // D9 -> mbed application shield LED green
  green_led.period(.4f);
  green_led.write(.5f);

  C12832 lcd(D11, D13, D12, D7, D10);

  // The enable pin of the drive board.
  DigitalOut drive_board_enable(ports::BOARD_ENABLE);
  drive_board_enable.write(true);

  Wheel *wheel_left = Wheel::left();
  Wheel *wheel_right = Wheel::right();
  wheel_left->set_target_speed(0.04f);
  wheel_right->set_target_speed(0.04f);

  float wait_period = 1.f / 24.f; // 24 Hz

  for (;;) {
    wait(wait_period);

    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("V_L:%.4f V_R:%.4f", wheel_left->get_current_speed(),
               wheel_right->get_current_speed());
  }

  drive_board_enable.write(false);
}