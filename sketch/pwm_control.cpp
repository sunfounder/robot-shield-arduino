/**
 * @file pwm_control.cpp
 * @brief PWM hardware abstraction — enable, period, and pulse-width over I2C.
 *
 * Register layout (per channel ch):
 *   CTRL   = 0x40 + ch        (1 byte,  enable)
 *   PERIOD = 0x50 + ch*2      (2 bytes, 16-bit period, little-endian)
 *   PULSE  = 0x70 + ch*2      (2 bytes, 16-bit pulse,  little-endian)
 *
 * Default period 0x4E20 = 20000 us → 50 Hz, suitable for standard servos.
 * Pulse range 500–2500 us maps to -90°–90° (handled by ServoControl).
 */

#include "pwm_control.h"
#include "reg_map.h"
#include "i2c.h"

//------------------------ Internal ------------------------//

/**
 * @brief Enable or disable a PWM channel via its control register.
 * @param ch      PWM channel (0–11).
 * @param enable  1 = on, 0 = off.
 */
void _pwm_write_ctrl(byte ch, byte enable)
{
    byte ctrl_reg = REG_PWM0_CTRL + ch;
    _i2c_write_reg(I2C_SLAVE_ADDR, ctrl_reg, enable);
}

/**
 * @brief Write the PWM period for a channel (16-bit, little-endian).
 * @param ch         PWM channel (0–11).
 * @param period_us  Period in microseconds.
 */
void _pwm_write_period(byte ch, uint16_t period_us)
{
    byte reg_l = REG_PWM0_PERIOD_L + ch * 2;
    _i2c_write_reg(I2C_SLAVE_ADDR, reg_l, period_us & 0xFF);
    _i2c_write_reg(I2C_SLAVE_ADDR, reg_l + 1, (period_us >> 8) & 0xFF);
}

/**
 * @brief Write the PWM pulse width for a channel (16-bit, little-endian).
 * @param ch        PWM channel (0–11).
 * @param pulse_us  Pulse width in microseconds.
 */
void _pwm_write_pulse(byte ch, uint16_t pulse_us)
{
    byte reg_l = REG_PWM0_PULSE_L + ch * 2;
    _i2c_write_reg(I2C_SLAVE_ADDR, reg_l, pulse_us & 0xFF);
    _i2c_write_reg(I2C_SLAVE_ADDR, reg_l + 1, (pulse_us >> 8) & 0xFF);
}

/**
 * @brief Read the current PWM pulse width from a channel.
 * @param ch  PWM channel (0–11).
 * @return    Pulse width in microseconds.
 */
uint16_t _pwm_read_pulse(byte ch)
{
    byte reg_l = REG_PWM0_PULSE_L + ch * 2;
    byte low = _i2c_read_reg(I2C_SLAVE_ADDR, reg_l, 1);
    byte high = _i2c_read_reg(I2C_SLAVE_ADDR, reg_l + 1, 1);
    return ((uint16_t)high << 8) | low;
}

/**
 * @brief Read the current PWM period from a channel.
 * @param ch  PWM channel (0–11).
 * @return    Period in microseconds.
 */
uint16_t _pwm_read_period(byte ch)
{
    byte reg_l = REG_PWM0_PERIOD_L + ch * 2;
    byte low = _i2c_read_reg(I2C_SLAVE_ADDR, reg_l, 1);
    byte high = _i2c_read_reg(I2C_SLAVE_ADDR, reg_l + 1, 1);
    return ((uint16_t)high << 8) | low;
}

//------------------------ Bridge API ------------------------//

/**
 * @brief Set PWM frequency for a channel. Converts Hz to period and writes hardware.
 * @param ch_str  PWM channel "0"–"11".
 * @param hz_str  Frequency in Hz (must be > 0).
 */
void pwm_set_freq(String ch_str, String hz_str)
{
    byte ch = (byte)ch_str.toInt();
    int hz = hz_str.toInt();
    if (hz <= 0) return;
    uint16_t period_us = (uint16_t)(1000000 / hz);
    _pwm_write_period(ch, period_us);
}

/**
 * @brief Set PWM pulse width. Auto-enables the channel after writing.
 * @param ch_str     PWM channel "0"–"11".
 * @param pulse_str  Pulse width in microseconds (0–65535, clamped).
 */
void pwm_set_pulse(String ch_str, String pulse_str)
{
    byte ch = (byte)ch_str.toInt();
    int pulse = pulse_str.toInt();
    pulse = (pulse < 0) ? 0 : (pulse > 65535) ? 65535 : pulse;
    _pwm_write_pulse(ch, (uint16_t)pulse);
    _pwm_write_ctrl(ch, 1);
}

/**
 * @brief Read current PWM pulse width from a channel.
 * @param ch_str  PWM channel "0"–"11".
 * @return        Pulse width in microseconds.
 */
int pwm_get_pulse(String ch_str)
{
    byte ch = (byte)ch_str.toInt();
    return (int)_pwm_read_pulse(ch);
}

/**
 * @brief Enable or disable a PWM channel.
 * @param ch_str  PWM channel "0"–"11".
 * @param en_str  "1" = enable, "0" = disable.
 * @return        0.
 */
int pwm_enable(String ch_str, String en_str)
{
    byte ch = (byte)ch_str.toInt();
    byte en = (byte)en_str.toInt();
    _pwm_write_ctrl(ch, en ? 1 : 0);
    return 0;
}
