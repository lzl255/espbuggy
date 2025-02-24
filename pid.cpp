#include "pid.hpp"

PidController::PidController(float set_point, float k_p, float k_i, float k_d)
    : set_point(set_point), k_p(k_p), k_i(k_i), k_d(k_d), accumulator(0.f),
      previous_error(set_point) {}

float PidController::feedback(float input, float sampling_period) {
  float error = this->set_point - input;
  float p_term = this->k_p * error;
  this->accumulator += error;
  float i_term = this->k_i * this->accumulator * sampling_period;
  float d_term =
      this->k_d * (error - this->previous_error) * (1.f / sampling_period);
  this->previous_error = error;
  this->previous_p = p_term;
  this->previous_i = i_term;
  this->previous_d = d_term;
  return p_term + i_term + d_term;
}

float PidController::get_previous_term_p() const { return this->previous_p; }

float PidController::get_previous_term_i() const { return this->previous_i; }

float PidController::get_previous_term_d() const { return this->previous_d; }

void PidController::set_set_point(float set_point) {
  this->set_point = set_point;
}

float PidController::get_set_point() const { return this->set_point; }

void PidController::clear_integral() { this->accumulator = 0; }