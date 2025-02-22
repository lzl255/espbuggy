#include "potentiometer.hpp"

Potentiometer::Potentiometer(PinName pin, float vdd)
    : input_signal(pin) {}

float Potentiometer::sample() {
  this->last_sample_normalized = this->input_signal.read();
  return this->last_sample_normalized;
}

float Potentiometer::get_last_sample() const {
  return this->last_sample_normalized;
}