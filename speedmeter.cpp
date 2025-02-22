#include "speedmeter.hpp"

static inline float ticks_to_speed(int d_ticks, float seconds) {
  float tick_rate = (float)d_ticks / seconds;
  return tick_rate * WHEEL_DIAMETER * PI / (512 * GEAR_RATIO);
}

void SpeedMeter::ticker_callback() {
  int ticks = this->qei.getPulses();
  this->speed =
      ticks_to_speed(ticks - this->previous_ticks, this->sampling_period);
  if (this->speed != 0.f) // to avoid having negative zero.
    this->speed *= this->correction_factor;
  this->previous_ticks = ticks;
}

SpeedMeter::SpeedMeter(PinName channel_a, PinName channel_b,
                       float sampling_period, float correction_factor)
    : qei(channel_a, channel_b, NC, COUNTERS_PER_REVOLUTION_X2,
          QEI::X2_ENCODING),
      sampling_period(sampling_period), speed(0.f), ticker(), previous_ticks(0),
      correction_factor(correction_factor) {
  this->qei.reset();
  this->previous_ticks = this->qei.getPulses();
  this->ticker.attach(callback(this, &SpeedMeter::ticker_callback),
                      this->sampling_period);
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