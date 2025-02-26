#pragma once

#include <mbed.h>

/**
 * Abstraction of a potentiometer.
 */
class Potentiometer {
private:
  AnalogIn input_signal;
  /// The normalized reading of the latest sample.
  float last_sample_normalized;

public:
  Potentiometer(PinName pin);

  /** Get a new sample from the input signal.
   * @returns the sampled value in normalize form.
   */
  float sample();

  /**
    * @returns The latest sample in normalized form.
  */
  float get_last_sample() const;
};
