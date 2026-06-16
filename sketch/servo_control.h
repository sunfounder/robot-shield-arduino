#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>
#include "reg_map.h"

// Minimum pulse width for -90°
#define SERVO_MIN_PULSE        500
// Mid-point pulse width for 0°
#define SERVO_MID_PULSE        1500
// Maximum pulse width for +90°
#define SERVO_MAX_PULSE        2500
// Write to REG_SYS_CTRL to center all servos
#define SERVO_ALL_CENTER      0x40

// ch_str: servo channel "0"–"11", angle_str: target angle "-90"–"90" -> return: none
void servo_set_angle(String ch_str, String angle_str);

// ch_str: servo channel "0"–"11" -> return: current angle (°)
int servo_get_angle(String ch_str);

#endif
