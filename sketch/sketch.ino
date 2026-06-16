/**
 * @file sketch.ino
 * @brief UNO Q AI Robot — Arduino main program.
 *
 * Architecture: Python (main.py) ← Bridge → sketch.ino ← I2C → co-processor @ 0x20
 *
 * Wires up Bridge function registrations; all logic lives in module .cpp files.
 * The loop is empty — work is driven by Bridge callbacks and Python polling.
 */

#include "Arduino_RouterBridge.h"
#include "led_control.h"
#include "power_monitor.h"
#include "servo_control.h"
#include "pwm_control.h"
#include "motor_control.h"
#include "i2c.h"
#include <Wire.h>

//------------------------ Bridge bindings ------------------------//

void bridge_binds()
{
    // LED
    Bridge.provide("led_set_state", led_set_state);

    // I2C pass-through (telemetry, system control)
    Bridge.provide("read_reg", read_reg);
    Bridge.provide("write_reg", write_reg);

    // Power telemetry
    Bridge.provide("get_bat_volt", get_bat_volt);
    Bridge.provide("get_bat_percent", get_bat_percent);
    Bridge.provide("get_bat_status", get_bat_status);
    Bridge.provide("get_arduino_current", get_arduino_current);
    Bridge.provide("get_ioref_volt", get_ioref_volt);

    // PWM control (frequency, pulse, enable)
    Bridge.provide("pwm_set_freq", pwm_set_freq);
    Bridge.provide("pwm_set_pulse", pwm_set_pulse);
    Bridge.provide("pwm_get_pulse", pwm_get_pulse);
    Bridge.provide("pwm_enable", pwm_enable);

    // Servo control (angle → pulse → registers)
    Bridge.provide("servo_set_angle", servo_set_angle);
    Bridge.provide("servo_get_angle", servo_get_angle);

    // Motor control (power percentage → dual PWM channels)
    Bridge.provide("motor_set_power", motor_set_power);
}

//------------------------ Arduino setup ------------------------//

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    led_init();
    Bridge.begin();
    bridge_binds();

    Serial.println("=== UNO Q AI Robot Ready ===");
}

void loop()
{
    delay(100);
}
