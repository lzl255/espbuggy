#include "motor.hpp"

#include "ports.hpp"
#include <cmath>

Motor::Motor(PinName pin_direction, PinName pin_pwm, PinName pin_bipolar,
             bool direction_reversed, float initial_power)
    : direction(pin_direction), pwm(pin_pwm), bipolar(pin_bipolar), power(0.f),
      direction_reversed(direction_reversed) {
  this->bipolar.write(false);
  this->pwm.period(0.005f);
  this->set_power(initial_power);
}

Motor Motor::left() {
  return Motor(ports::DIRECTION_2, ports::PWM_2, ports::BIPOLAR_2, true);
}

Motor Motor::right() {
  // For some reason direction is reversed.
  return Motor(ports::DIRECTION_1, ports::PWM_1, ports::BIPOLAR_1, true);
}

int Motor::forward_direction() const {
  return this->direction_reversed ? 0 : 1;
}

int Motor::backward_direction() const {
  return this->direction_reversed ? 1 : 0;
}

void Motor::set_power(float power) {
  float old_power = this->power;
  this->power = power;

  // Change direction on sign change.
  if (old_power <= 0 && power > 0)
    this->direction.write(this->forward_direction());
  else if (old_power >= 0 && power < 0)
    this->direction.write(this->backward_direction());

  float power_abs = (power > 0) ? power : -power;
  this->pwm.write(1.f - power_abs);
}

float Motor::get_power() const { return this->power; }
