#pragma once

#include <mbed.h>

namespace ports {
    static const PinName JP1A_1 = PB_1;
    static const PinName JP1A_2 = PA_14;
    static const PinName JP1A_3 = PA_15;
    static const PinName JP1A_4 = PB_15;
    static const PinName JP1A_5 = PB_2;
    static const PinName JP1A_6 = PB_7;
    static const PinName JP1A_7 = PB_12;

    static const PinName BOARD_ENABLE = JP1A_7;
    static const PinName DIRECTION_1 = JP1A_2;
    static const PinName PWM_1       = JP1A_3;
    static const PinName DIRECTION_2 = JP1A_5;
    static const PinName PWM_2       = JP1A_6;
    static const PinName BIPOLAR_1   = JP1A_1;
    static const PinName BIPOLAR_2   = JP1A_4;

    /// Encoder A, channel A.
    static const PinName ENCODER_A_CHA = PB_13;
    /// Encoder A, channel B.
    static const PinName ENCODER_A_CHB = PB_14;
    /// Encoder B, channel A.
    static const PinName ENCODER_B_CHA = PC_2;
    /// Encoder B, channel B.
    static const PinName ENCODER_B_CHB = PC_3;
} // ports
