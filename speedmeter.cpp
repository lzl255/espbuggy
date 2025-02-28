#include "speedmeter.hpp"

static inline float ticks_to_distance(int delta_ticks) {
  return delta_ticks * WHEEL_DIAMETER * PI / (512.f * GEAR_RATIO);
}

void SpeedMeter::ticker_callback() {
  int ticks = this->qei.getPulses();
  float distance = ticks_to_distance(ticks - this->previous_ticks);
  this->previous_ticks = ticks;
  if (distance != 0.f) // to avoid having negative zero
    distance *= this->speed_correction_factor;
  this->distance += distance;
  this->speed = distance / this->sampling_period;
}

SpeedMeter::SpeedMeter(PinName channel_a, PinName channel_b,
                       float sampling_period, float correction_factor)
    : qei(channel_a, channel_b, NC, COUNTERS_PER_REVOLUTION_X2,
          QEI::X2_ENCODING),
      speed(0.f), ticker(), previous_ticks(0), sampling_period(sampling_period),
      speed_correction_factor(correction_factor) {
  this->qei.reset();
  this->previous_ticks = this->qei.getPulses();
  this->ticker.attach(callback(this, &SpeedMeter::ticker_callback),
                      sampling_period);
}

SpeedMeter *SpeedMeter::for_encoder_a(float sampling_period) {
  SpeedMeter *this_ = new SpeedMeter(ports::ENCODER_A_CHA, ports::ENCODER_A_CHB,
                                     sampling_period, -1.f);
  return this_;
}

SpeedMeter *SpeedMeter::for_encoder_b(float sampling_period) {
  return new SpeedMeter(ports::ENCODER_B_CHA, ports::ENCODER_B_CHB,
                        sampling_period, 1.f);
}

float SpeedMeter::get_speed() const { return this->speed; }

float SpeedMeter::get_distance() const { return this->distance; }

void SpeedMeter::reset_distance() { this->distance = 0.f; }
