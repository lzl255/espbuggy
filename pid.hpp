#pragma once

class PidController {
private:
  float set_point;
  float k_p;
  float k_i;
  float k_d;
  /// Accumulation of errors since the beginning (t = 0).
  float accumulator;
  /// The error value of the previous feedback.
  float previous_error;

public:
  PidController(float set_point, float k_p, float k_i, float k_d);

  float feedback(float input);

  void set_set_point(float set_point);

  float get_set_point() const;
};