#include "wheel.hpp"
#include "pid.hpp"
#include "speedmeter.hpp"

const float K_P = 7.0f;
const float K_I = 1.f;
const float K_D = 0.f;

void Wheel::ticker_callback() {
  if (this->get_target_speed() == 0.f && this->get_target_speed() == -0.f)
    return;
  float speed = this->get_current_speed();
  float pid_feedback = this->pid.feedback(speed, this->sampling_period);
  if (pid_feedback < -this->power_cap)
    pid_feedback = -this->power_cap;
  if (pid_feedback > this->power_cap)
    pid_feedback = this->power_cap;
  this->motor.set_power(pid_feedback);
}

Wheel::Wheel(Motor motor, SpeedMeter *speedmeter, PidController pid,
             float sampling_period)
    : motor(motor), speedmeter(speedmeter), pid(pid), ticker(),
      sampling_period(sampling_period), power_cap(1.f) {
  this->ticker.attach(callback(this, &Wheel::ticker_callback), sampling_period);
}

Wheel *Wheel::left() {
  return new Wheel(Motor::left(), SpeedMeter::for_encoder_b(),
                   PidController(0.0, K_P, K_I, K_D));
}

Wheel *Wheel::right() {
  return new Wheel(Motor::right(), SpeedMeter::for_encoder_a(),
                   PidController(0.0, K_P, K_I, K_D));
}

void Wheel::set_target_speed(float speed) {
  if (speed == 0.f && speed == -0.f) {
    this->motor.set_power(0.f);
  }
  this->pid.set_set_point(speed);
}

float Wheel::get_target_speed() const { return this->pid.get_set_point(); }

float Wheel::get_current_speed() const { return this->speedmeter->get_speed(); }

void Wheel::reset_distance() { this->speedmeter->reset_distance(); }

void Wheel::clear_pid() {
    this->pid.clear_integral();
}

float Wheel::get_distance() const { return this->speedmeter->get_distance(); }

void Wheel::set_power_cap(float power_cap) {
  if (power_cap < 0.f)
    power_cap = 0.f;
  else if (power_cap > 1.f)
    power_cap = 1.f;
  this->power_cap = power_cap;
}

float Wheel::get_power_cap() const { return this->power_cap; }

PidController *Wheel::get_pid() { return &this->pid; }