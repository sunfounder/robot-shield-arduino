/**
 * @file motor_control.cpp
 * @brief DC motor control via dual PWM channels (H-bridge).
 *
 * Control logic:
 *   A=1 B=0 → forward
 *   A=0 B=1 → reverse
 *   A=0 B=0 → brake
 *
 * PWM frequency: 100 Hz (period = 10000 us) for DC motors.
 */

#include "motor_control.h"
#include "pwm_control.h"

/**
 * @brief Set motor power level using dual PWM channels.
 *
 * Forward: A = duty, B = 0. Reverse: A = 0, B = duty. Brake: both low.
 *
 * @param motor      Motor name: "M0"–"M3".
 * @param power_str  Power level: "-100" (full reverse) to "100" (full forward), 0 = brake.
 */
void motor_set_power(String motor, String power_str)
{
    int power = power_str.toInt();
    power = (power < -100) ? -100 : (power > 100) ? 100 : power;

    byte ch_a, ch_b;
    if (motor == "M0")      
    { 
        ch_a = 4;  
        ch_b = 5;  
    }
    else if (motor == "M1") 
    { 
        ch_a = 6;
        ch_b = 7;  
    }
    else if (motor == "M2") 
    { 
        ch_a = 8;  
        ch_b = 9;  
    }
    else if (motor == "M3") 
    { 
        ch_a = 10; 
        ch_b = 11; 
    }
    else return;

    const uint16_t period_us = 10000;
    _pwm_write_period(ch_a, period_us);
    _pwm_write_period(ch_b, period_us);

    uint16_t duty;
    if (power > 0) 
    {
        duty = (uint16_t)map(power, 0, 100, 0, period_us);
        _pwm_write_pulse(ch_a, duty);
        _pwm_write_pulse(ch_b, 0);
        _pwm_write_ctrl(ch_a, 1);
        _pwm_write_ctrl(ch_b, 1);
    } 
    else if (power < 0) 
    {
        duty = (uint16_t)map(-power, 0, 100, 0, period_us);
        _pwm_write_pulse(ch_a, 0);
        _pwm_write_pulse(ch_b, duty);
        _pwm_write_ctrl(ch_a, 1);
        _pwm_write_ctrl(ch_b, 1);
    } 
    else 
    {
        _pwm_write_pulse(ch_a, 0);
        _pwm_write_pulse(ch_b, 0);
        _pwm_write_ctrl(ch_a, 0);
        _pwm_write_ctrl(ch_b, 0);
    }
}
