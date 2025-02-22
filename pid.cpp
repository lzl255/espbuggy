#include "pid.hpp"

PidController::PidController(float set_point, float k_p, float k_i, float k_d)
    : set_point(set_point), k_p(k_p), k_i(k_i), k_d(k_d), accumulator(0.f),
      previous_error(0.f) {}

float PidController::feedback(float input) {
  float error = this->set_point - input;
  float p_term = this->k_p * error;
  this->accumulator += error;
  float i_term = this->k_i * this->accumulator;
  float d_term = this->k_d * (this->previous_error - error);
  this->previous_error = error;
  return p_term + i_term + d_term;
}

void PidController::set_set_point(float set_point) {
  this->set_point = set_point;
}

float PidController::get_set_point() const { return this->set_point; }