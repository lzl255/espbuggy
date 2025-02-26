#include "wheel.hpp"
#include "pid.hpp"
#include "speedmeter.hpp"
#include "utils.hpp"

static const float K_P = 7.0f;
static const float K_I = 1.f;
static const float K_D = 0.2f;
static const float I_LIMIT = 0.5f;

void Wheel::ticker_callback() {
  if (is_zero(this->get_target_speed()))
    return;
  float speed = this->get_current_speed();
  float pid_feedback = this->pid.feedback(speed, this->sampling_period);
  pid_feedback = clamp(pid_feedback, -this->power_cap, this->power_cap);
  this->motor.set_power(pid_feedback);
}

Wheel::Wheel(Motor motor, SpeedMeter *speedmeter, float sampling_period)
    : motor(motor), speedmeter(speedmeter),
      pid(PidController(0.f, K_P, K_I, K_D, I_LIMIT)), ticker(),
      sampling_period(sampling_period), power_cap(1.f) {
  this->ticker.attach(callback(this, &Wheel::ticker_callback), sampling_period);
}

Wheel *Wheel::left() {
  return new Wheel(Motor::left(), SpeedMeter::for_encoder_b());
}

Wheel *Wheel::right() {
  return new Wheel(Motor::right(), SpeedMeter::for_encoder_a());
}

void Wheel::set_target_speed(float speed) {
  if (is_zero(speed)) {
    this->motor.set_power(0.f);
    this->pid.reset();
  }
  this->pid.set_set_point(speed);
}

float Wheel::get_target_speed() const { return this->pid.get_set_point(); }

float Wheel::get_current_speed() const { return this->speedmeter->get_speed(); }

void Wheel::reset_distance() { this->speedmeter->reset_distance(); }

void Wheel::reset_pid() { this->pid.reset(); }

float Wheel::get_distance() const { return this->speedmeter->get_distance(); }

void Wheel::set_power_cap(float power_cap) {
  this->power_cap = clamp(power_cap, 0.f, 1.f);
}

float Wheel::get_power_cap() const { return this->power_cap; }

PidController *Wheel::get_pid() { return &this->pid; }
