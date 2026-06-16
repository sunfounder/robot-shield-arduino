#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

// DC motor via dual PWM (H-bridge): M0=PWM4/5, M1=PWM6/7, M2=PWM8/9, M3=PWM10/11

// motor: "M0"–"M3", power_str: "-100" (full reverse) ~ "100" (full forward), 0=brake -> return: none
void motor_set_power(String motor, String power_str);

#endif
