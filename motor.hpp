#pragma once

#include <mbed.h>

/**
 * Abstraction of a motor.
 */
class Motor {
protected:
  /// The direction pin.
  DigitalOut direction;

  /// The PWM as abstracted by `PwmOut`.
  PwmOut pwm;

  /// The bipolar pin.
  DigitalOut bipolar;

  /**
   * The power level.
   * Should be a `float` in the range of `-1` to `1` inclusive.
   * @see Motor::set_power
   */
  float power;

  /**
   * Is the direction reversed?
   * If `false`, `1` on direction pin should result in motor driving forward.
   * If `true`, `1` on direction pin should result in motor driving backward.
   */
  bool direction_reversed;

  /// The direction bit that would result in motor driving forward.
  int forward_direction() const;

  /// The direction bit that would result in motor driving forward.
  int backward_direction() const;

public:
  Motor(PinName pin_direction, PinName pin_pwm, PinName pin_bipolar,
        bool direction_reversed, float initial_power = 0.f);

  /**
   * Constructs the left motor using port values in `ports`.
   * @returns the left motor instance.
   * @see ports
   */
  static Motor left();

  /**
   * Constructs the right motor using port values in `ports`.
   * @returns the right motor instance.
   * @see ports
   */
  static Motor right();

  /**
   * Set the power level of a motor.
   * @param power must be in range of -1 to 1 (inclusive). Greater value is
nnnnn   * faster speed. Don't forget to set enable pin of motor drive board.
   */
  void set_power(float power);

  /**
    * Get the current power level.
      @returns the current power.
  */
  float get_power() const;
};