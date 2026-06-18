/**
 * @file Servo.cpp
 * @brief Servo implementation — angle ↔ pulse conversion + PwmChannel delegation.
 */

#include "Servo.h"

Servo::Servo(uint8_t ch)
    : _pwm(ch)
{}

void Servo::begin()
{
    _pwm.setFreq(1000000 / PERIOD_US);
}

void Servo::setAngle(int16_t angle)
{
    int16_t pulse = angleToPulse(angle);
    _pwm.setPulse((uint16_t)pulse);
    _pwm.enable(true);  // pulse written before enable — avoids power-on jerk
}

int16_t Servo::getAngle()
{
    uint16_t pulse = _pwm.getPulse();
    return pulseToAngle(pulse);
}

int16_t Servo::angleToPulse(int16_t angle)
{
    if (angle < ANGLE_MIN) angle = ANGLE_MIN;
    if (angle > ANGLE_MAX) angle = ANGLE_MAX;
    return map(angle, ANGLE_MIN, ANGLE_MAX, MIN_PULSE, MAX_PULSE);
}

int16_t Servo::pulseToAngle(uint16_t pulse)
{
    if (pulse < MIN_PULSE) pulse = MIN_PULSE;
    if (pulse > MAX_PULSE) pulse = MAX_PULSE;
    return (int16_t)map(pulse, MIN_PULSE, MAX_PULSE, ANGLE_MIN, ANGLE_MAX);
}
