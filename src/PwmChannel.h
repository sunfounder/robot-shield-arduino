/**
 * @file PwmChannel.h
 * @brief Single PWM channel control — frequency, pulse width, and enable.
 *
 * Register layout (per channel ch = 0–11):
 *   CTRL   = 0x40 + ch       (1 byte, enable)
 *   PERIOD = 0x50 + ch * 2   (2 bytes, 16-bit LE, microseconds)
 *   PULSE  = 0x70 + ch * 2   (2 bytes, 16-bit LE, microseconds)
 *
 * Default period: 20000 us (50 Hz), suitable for standard servos.
 */

#ifndef PWM_CHANNEL_H
#define PWM_CHANNEL_H

#include <Arduino.h>

class PwmChannel {
public:
    /**
     * @brief Construct a PWM channel.
     * @param ch Channel number (0–11).
     */
    explicit PwmChannel(uint8_t ch);

    /**
     * @brief Initialise the channel with default period (20000 us / 50 Hz).
     */
    void begin();

    /**
     * @brief Set the PWM frequency. Period is computed as 1e6 / hz.
     * @param hz Frequency in Hz (must be > 0).
     */
    void setFreq(uint16_t hz);

    /**
     * @brief Set the PWM pulse width. Does not auto-enable the channel.
     * @param us Pulse width in microseconds (0–65535).
     */
    void setPulse(uint16_t us);

    /**
     * @brief Read the current PWM pulse width.
     * @return uint16_t Pulse width in microseconds.
     */
    uint16_t getPulse();

    /**
     * @brief Read the current PWM period.
     * @return uint16_t Period in microseconds.
     */
    uint16_t getPeriod();

    /**
     * @brief Enable or disable the PWM channel.
     * @param on true to enable, false to disable.
     */
    void enable(bool on);

    /**
     * @brief Get the channel number.
     * @return uint8_t Channel number (0–11).
     */
    uint8_t channel() const { return _ch; }

private:
    uint8_t _ch;  ///< PWM channel number (0–11).
};

#endif // PWM_CHANNEL_H
