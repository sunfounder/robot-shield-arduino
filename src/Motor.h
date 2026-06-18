/**
 * @file Motor.h
 * @brief DC motor control via dual-PWM H-bridge.
 *
 * Composes two PwmChannel instances for H-bridge drive:
 *   Forward → A = duty, B = 0
 *   Reverse → A = 0, B = duty
 *   Brake   → A = 0, B = 0 (disabled)
 *
 * Channel mapping: M0=PWM4/5, M1=PWM6/7, M2=PWM8/9, M3=PWM10/11.
 * PWM frequency: 100 Hz (10000 us period).
 */

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "PwmChannel.h"

class Motor {
public:
    static constexpr uint16_t PERIOD_US = 10000;  ///< PWM period (us), 10000 us = 100 Hz.

    /**
     * @brief Construct a Motor with dual PWM channels.
     * @param id   Motor identifier ("M0"–"M3").
     * @param ch_a A-phase PWM channel number.
     * @param ch_b B-phase PWM channel number.
     */
    Motor(const char* id, uint8_t ch_a, uint8_t ch_b);

    /**
     * @brief Initialise both PWM channels (writes 100 Hz period).
     */
    void begin();

    /**
     * @brief Set motor power level.
     * @param power Power value: -100 (full reverse) to +100 (full forward),
     *              0 = brake. Clamped if out of range.
     */
    void setPower(int16_t power);

    /**
     * @brief Get the motor identifier.
     * @return const char* Motor id ("M0"–"M3").
     */
    const char* id() const { return _id; }

private:
    const char* _id;     ///< Motor identifier ("M0"–"M3").
    PwmChannel  _pwm_a;  ///< A-phase PWM channel.
    PwmChannel  _pwm_b;  ///< B-phase PWM channel.
};

#endif // MOTOR_H
