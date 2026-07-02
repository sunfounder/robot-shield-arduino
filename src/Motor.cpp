/**
 * @file Motor.cpp
 * @brief DC motor H-bridge implementation — dual PWM forward/reverse/brake.
 *
 * H-bridge truth table:
 *   Forward → A = duty, B = 0
 *   Reverse → A = 0, B = duty
 *   Brake   → A = 0, B = 0 (disabled)
 */

#include "Motor.h"

Motor::Motor(const char* id, uint8_t ch_a, uint8_t ch_b)
    : _id(id), _pwm_a(ch_a), _pwm_b(ch_b)
{}

void Motor::begin()
{
    _pwm_a.setFreq(1000000 / PERIOD_US);
    _pwm_b.setFreq(1000000 / PERIOD_US);
}

void Motor::setPower(int16_t power)
{
    if (power < -100) power = -100;
    if (power > 100)  power = 100;

    uint16_t duty;
    if (power > 0) {
        // Forward: A = duty, B = 0
        duty = (uint16_t)map(power, 0, 100, 0, PERIOD_US);
        _pwm_a.setPulse(duty);
        _pwm_b.setPulse(0);
        _pwm_a.setEnable(true);
        _pwm_b.setEnable(true);
    } else if (power < 0) {
        // Reverse: A = 0, B = duty
        duty = (uint16_t)map(-power, 0, 100, 0, PERIOD_US);
        _pwm_a.setPulse(0);
        _pwm_b.setPulse(duty);
        _pwm_a.setEnable(true);
        _pwm_b.setEnable(true);
    } else {
        // Brake: both low, both disabled
        _pwm_a.setPulse(0);
        _pwm_b.setPulse(0);
        _pwm_a.setEnable(false);
        _pwm_b.setEnable(false);
    }
}
