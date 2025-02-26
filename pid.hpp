#pragma once

class PidController {
private:
  float set_point;
  float k_p;
  float k_i;
  float k_d;
  /// Accumulation of errors since the beginning (t = 0).
  float accumulator;
  /// Limit of magnitude of `accumulator`.
  float integral_limit;
  /// The error value of the previous feedback.
  float previous_error;
  float previous_p;
  float previous_i;
  float previous_d;

public:
  PidController(float set_point, float k_p, float k_i, float k_d, float integral_limit = 100.f);

  float feedback(float input, float sampling_period);

  float get_previous_term_p() const;
  
  float get_previous_term_i() const;

  float get_previous_term_d() const;

  void set_set_point(float set_point);

  float get_set_point() const;

  void set_integral_limit(float integral_limit);

  float get_integral_limit() const;

  void reset();
};
