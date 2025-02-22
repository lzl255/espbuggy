#include "wheel.hpp"
#include "speedmeter.hpp"

const float K_P = 5.0f;
const float K_I = 0.3f;
const float K_D = 0.05f;

void Wheel::ticker_callback() {
  float speed = this->get_current_speed();
  float pid_feedback = this->pid.feedback(speed);
  if (pid_feedback < -1.f)
    pid_feedback = -1.f;
  if (pid_feedback > 1.f)
    pid_feedback = 1.f;
  this->motor.set_power(pid_feedback);
}

Wheel::Wheel(Motor motor, SpeedMeter *speedmeter, PidController pid)
    : motor(motor), speedmeter(speedmeter), pid(pid), ticker() {
  this->ticker.attach(callback(this, &Wheel::ticker_callback), 0.05f);
}

Wheel *Wheel::left() {
  return new Wheel(Motor::left(), SpeedMeter::for_encoder_b(),
                   PidController(0.0, K_P, K_I, K_D));
}

Wheel *Wheel::right() {
  return new Wheel(Motor::right(), SpeedMeter::for_encoder_a(),
                   PidController(0.0, K_P, K_I, K_D));
}

void Wheel::set_target_speed(float speed) { this->pid.set_set_point(speed); }

float Wheel::get_target_speed() const { return this->pid.get_set_point(); }

float Wheel::get_current_speed() const { return this->speedmeter->get_speed(); }
