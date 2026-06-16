/**
 * @file servo_control.cpp
 * @brief Servo control layer — angle/pulse conversion plus Bridge API.
 *
 * Sits on top of PWMControl. Converts human-facing angle values (-90°–90°)
 * to PWM pulse widths (500–2500 us) and vice versa.
 */

#include "servo_control.h"
#include "pwm_control.h"

//------------------------ Internal (static) ------------------------//

static void _servo_move(byte ch, byte enable, int pulse_us);
static int  _servo_read_pulse(byte ch);
static int  _angle_to_pulse(int angle);
static int  _pulse_to_angle(int pulse);

//------------------------ Public API ------------------------//

/**
 * @brief Set a servo channel to the given angle. Converts angle to pulse, writes hardware.
 * @param ch_str     Servo channel "0"–"11".
 * @param angle_str  Target angle "-90"–"90".
 */
void servo_set_angle(String ch_str, String angle_str)
{
    byte ch = (byte)ch_str.toInt();
    int angle = angle_str.toInt();
    int pulse = _angle_to_pulse(angle);
    _servo_move(ch, 1, pulse);
}

/**
 * @brief Read the current angle of a servo channel from hardware.
 * @param ch_str  Servo channel "0"–"11".
 * @return        Current angle in degrees (-90–90).
 */
int servo_get_angle(String ch_str)
{
    byte ch = (byte)ch_str.toInt();
    int pulse = _servo_read_pulse(ch);
    return _pulse_to_angle(pulse);
}

//------------------------ Internal ------------------------//

/**
 * @brief Write pulse width then enable the channel.
 *
 * Pulse is written before enabling to prevent the servo from seeing a stale
 * pulse value on power-up and jerking to the wrong position.
 *
 * @param ch        Servo channel (0–11).
 * @param enable    1 = on, 0 = off.
 * @param pulse_us  Pulse width in microseconds.
 */
static void _servo_move(byte ch, byte enable, int pulse_us)
{
    _pwm_write_period(ch, 20000);
    _pwm_write_pulse(ch, (uint16_t)pulse_us);
    _pwm_write_ctrl(ch, enable);
}

/**
 * @brief Read current pulse width from a servo channel.
 * @param ch  Servo channel (0–11).
 * @return    Pulse width in microseconds.
 */
static int _servo_read_pulse(byte ch)
{
    return (int)_pwm_read_pulse(ch);
}

/**
 * @brief Map angle (-90°–90°) to pulse width (500–2500 us). Input is clamped.
 * @param angle  Target angle in degrees.
 * @return       Pulse width in microseconds.
 */
static int _angle_to_pulse(int angle)
{
    angle = (angle < -90) ? -90 : (angle > 90) ? 90 : angle;
    return map(angle, -90, 90, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
}

/**
 * @brief Map pulse width (500–2500 us) back to angle (-90°–90°). Input is clamped.
 * @param pulse  Pulse width in microseconds.
 * @return       Angle in degrees.
 */
static int _pulse_to_angle(int pulse)
{
    pulse = (pulse < SERVO_MIN_PULSE) ? SERVO_MIN_PULSE
         : (pulse > SERVO_MAX_PULSE) ? SERVO_MAX_PULSE
         : pulse;
    return map(pulse, SERVO_MIN_PULSE, SERVO_MAX_PULSE, -90, 90);
}
