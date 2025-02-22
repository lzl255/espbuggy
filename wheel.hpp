#pragma once

#include "motor.hpp"
#include "pid.hpp"
#include "speedmeter.hpp"

class Wheel {
protected:
  Motor motor;
  SpeedMeter *speedmeter;
  PidController pid;
  Ticker ticker;

private:
  void ticker_callback();

public:
  Wheel(Motor motor, SpeedMeter *speedmeter, PidController pid);

  /// Constructs the left wheel's abstraction using values in `ports`.
  /// @returns the wheel class abstracting the left wheel.
  /// @see ports
  static Wheel *left();

  /// Constructs the left wheel's abstraction using values in `ports`.
  /// @returns the wheel class abstracting the left wheel.
  /// @see ports
  static Wheel *right();

  /// Sets the target speed.
  /// @param speed the target speed in meters per second.
  void set_target_speed(float speed);

  /// Sets the target speed.
  /// @returns the target speed in meters per second.
  float get_target_speed() const;

  /// Get the current speed.
  /// @returns the current speed in meters per second.
  float get_current_speed() const;
};