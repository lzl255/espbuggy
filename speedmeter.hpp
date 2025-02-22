#pragma once

#include <QEI.h>
#include <mbed.h>

#include "ports.hpp"

const float COUNTERS_PER_REVOLUTION_X2 = 512.f;
const float COUNTERS_PER_REVOLUTION_X4 = 1024.f;
const float GEAR_RATIO = 15.f;
const float WHEEL_DIAMETER = 80.4f / 1000.f;
const float PI = 3.141592653589;

/**
 * Measures speed with encoder on the wheel motors.
 */
class SpeedMeter {
protected:
  QEI qei;
  float speed;
  Ticker ticker;
  int previous_ticks;
  float sampling_period;
  float correction_factor;

private:
  void ticker_callback();

public:
  SpeedMeter(PinName channel_a, PinName channel_b, float sampling_period,
             float correction_factor = 1.f);

  /**
      Makes an instance of speed calculator with encoder A.
      Uses port numbers as defined in `ports`.
      @param sampling_period the sampling period (in seconds)
      @returns the instance of speed calculator
  */
  static SpeedMeter *for_encoder_a(float sampling_period = 0.04f);

  /**
      Makes an instance of speed calculator with encoder B.
      Uses port numbers as defined in `ports`.
      @param sampling_period the sampling period (in seconds)
      @returns the instance of speed calculator
  */
  static SpeedMeter *for_encoder_b(float sampling_period = 0.04f);

  /**
      Get the current speed.
      @returns the speed, in m/s.
  */
  float get_speed() const;
};