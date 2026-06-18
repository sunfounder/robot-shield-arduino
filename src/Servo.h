/**
 * @file Servo.h
 * @brief Servo angle control — composes PwmChannel for angle ↔ pulse mapping.
 *
 * Linear mapping: -90° → 500 us, 0° → 1500 us, +90° → 2500 us.
 * Pulse is written before enabling the channel to prevent jerk on power-up.
 */

#ifndef SERVO_H
#define SERVO_H

#include <Arduino.h>
#include "PwmChannel.h"

class Servo {
public:
    static constexpr int16_t  MIN_PULSE  = 500;    ///< Minimum pulse width (us) at -90°.
    static constexpr int16_t  MID_PULSE  = 1500;   ///< Mid-point pulse width (us) at 0°.
    static constexpr int16_t  MAX_PULSE  = 2500;   ///< Maximum pulse width (us) at +90°.
    static constexpr int16_t  ANGLE_MIN  = -90;     ///< Minimum servo angle.
    static constexpr int16_t  ANGLE_MAX  = 90;      ///< Maximum servo angle.
    static constexpr uint16_t PERIOD_US  = 20000;   ///< PWM period (us), 20000 us = 50 Hz.

    /**
     * @brief Construct a Servo on the given PWM channel.
     * @param ch PWM channel number (0–11).
     */
    explicit Servo(uint8_t ch);

    /**
     * @brief Initialise the servo PWM channel (writes 50 Hz period).
     */
    void begin();

    /**
     * @brief Set the servo angle. Pulse is written before enable to avoid jerk.
     * @param angle Target angle in degrees (-90 to +90). Clamped if out of range.
     */
    void setAngle(int16_t angle);

    /**
     * @brief Read the current servo angle from the hardware.
     * @return int16_t Current angle in degrees (-90 to +90).
     */
    int16_t getAngle();

private:
    PwmChannel _pwm;  ///< Underlying PWM channel.

    /**
     * @brief Convert angle to pulse width.
     * @param angle Angle in degrees (-90 to +90).
     * @return int16_t Pulse width in microseconds (500–2500).
     */
    static int16_t angleToPulse(int16_t angle);

    /**
     * @brief Convert pulse width to angle.
     * @param pulse Pulse width in microseconds.
     * @return int16_t Angle in degrees (-90 to +90).
     */
    static int16_t pulseToAngle(uint16_t pulse);
};

#endif // SERVO_H
