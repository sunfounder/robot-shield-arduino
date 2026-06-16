#ifndef PWM_CONTROL_H
#define PWM_CONTROL_H

#include <Arduino.h>

//------------------------ Internal ------------------------//

// ch: PWM channel (0–11), enable: 1=on 0=off -> return: none
void _pwm_write_ctrl(byte ch, byte enable);

// ch: PWM channel (0–11), period_us: period in microseconds -> return: none
void _pwm_write_period(byte ch, uint16_t period_us);

// ch: PWM channel (0–11), pulse_us: pulse width in microseconds -> return: none
void _pwm_write_pulse(byte ch, uint16_t pulse_us);

// ch: PWM channel (0–11) -> return: pulse width (μs)
uint16_t _pwm_read_pulse(byte ch);

// ch: PWM channel (0–11) -> return: period (μs)
uint16_t _pwm_read_period(byte ch);

//------------------------ Bridge API ------------------------//

// ch_str: PWM channel "0"–"11", hz_str: frequency in Hz -> return: none
void pwm_set_freq(String ch_str, String hz_str);

// ch_str: PWM channel "0"–"11", pulse_str: pulse width in μs (auto-enables) -> return: none
void pwm_set_pulse(String ch_str, String pulse_str);

// ch_str: PWM channel "0"–"11" -> return: current pulse width (μs)
int pwm_get_pulse(String ch_str);

// ch_str: PWM channel "0"–"11", en_str: "1"=enable "0"=disable -> return: 0
int pwm_enable(String ch_str, String en_str);

#endif
