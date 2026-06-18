/**
 * @file RobotShieldBridge.cpp
 * @brief Bridge registration hub — static instance arrays + Bridge.provide() bindings.
 *
 * All Bridge function names must match the Python side's Bridge.call() strings exactly.
 */

#include "RobotShieldBridge.h"
#include "PwmChannel.h"
#include "Servo.h"
#include "Motor.h"
#include "PowerMonitor.h"
#include "I2cBus.h"
#include "Arduino_RouterBridge.h"

// Static module instances — one per channel/motor

static PwmChannel _pwms[12] = {
    PwmChannel(0), PwmChannel(1), PwmChannel(2), PwmChannel(3),
    PwmChannel(4), PwmChannel(5), PwmChannel(6), PwmChannel(7),
    PwmChannel(8), PwmChannel(9), PwmChannel(10), PwmChannel(11)
};

static Servo _servos[12] = {
    Servo(0), Servo(1), Servo(2), Servo(3),
    Servo(4), Servo(5), Servo(6), Servo(7),
    Servo(8), Servo(9), Servo(10), Servo(11)
};

static Motor _motors[4] = {
    Motor("M0", 4, 5),
    Motor("M1", 6, 7),
    Motor("M2", 8, 9),
    Motor("M3", 10, 11)
};

static PowerMonitor _power;

//------------------------ Lifecycle ------------------------//

void RobotShieldBridge::begin()
{
    for (uint8_t i = 0; i < 12; i++) {
        _pwms[i].begin();
        _servos[i].begin();
    }
    for (uint8_t i = 0; i < 4; i++) {
        _motors[i].begin();
    }
    _power.begin();
}

void RobotShieldBridge::registerAll()
{
    // I2C pass-through
    Bridge.provide("read_reg", I2cBus::bridgeReadReg);
    Bridge.provide("write_reg", I2cBus::bridgeWriteReg);

    // PWM
    Bridge.provide("pwm_set_freq",  bridgePwmSetFreq);
    Bridge.provide("pwm_set_pulse", bridgePwmSetPulse);
    Bridge.provide("pwm_get_pulse", bridgePwmGetPulse);
    Bridge.provide("pwm_enable",    bridgePwmEnable);

    // Servo
    Bridge.provide("servo_set_angle", bridgeServoSetAngle);
    Bridge.provide("servo_get_angle", bridgeServoGetAngle);

    // Motor
    Bridge.provide("motor_set_power", bridgeMotorSetPower);

    // Power telemetry
    Bridge.provide("get_bat_volt",        bridgeGetBatVolt);
    Bridge.provide("get_bat_percent",     bridgeGetBatPercent);
    Bridge.provide("get_bat_status",      bridgeGetBatStatus);
    Bridge.provide("get_arduino_current", bridgeGetArduinoCurrent);
    Bridge.provide("get_ioref_volt",      bridgeGetIorefVolt);
}

//------------------------ PWM wrappers ------------------------//

void RobotShieldBridge::bridgePwmSetFreq(String ch_str, String hz_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    uint16_t hz = (uint16_t)hz_str.toInt();
    if (ch < 12) _pwms[ch].setFreq(hz);
}

void RobotShieldBridge::bridgePwmSetPulse(String ch_str, String pulse_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    int pulse = pulse_str.toInt();
    if (pulse < 0) pulse = 0;
    if (pulse > 65535) pulse = 65535;
    if (ch < 12) {
        _pwms[ch].setPulse((uint16_t)pulse);
        _pwms[ch].enable(true);
    }
}

int RobotShieldBridge::bridgePwmGetPulse(String ch_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    if (ch < 12) return (int)_pwms[ch].getPulse();
    return 0;
}

int RobotShieldBridge::bridgePwmEnable(String ch_str, String en_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    uint8_t en = (uint8_t)en_str.toInt();
    if (ch < 12) _pwms[ch].enable(en ? true : false);
    return 0;
}

//------------------------ Servo wrappers ------------------------//

void RobotShieldBridge::bridgeServoSetAngle(String ch_str, String angle_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    int16_t angle = (int16_t)angle_str.toInt();
    if (ch < 12) _servos[ch].setAngle(angle);
}

int RobotShieldBridge::bridgeServoGetAngle(String ch_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    if (ch < 12) return (int)_servos[ch].getAngle();
    return 0;
}

//------------------------ Motor wrappers ------------------------//

void RobotShieldBridge::bridgeMotorSetPower(String motor, String power_str)
{
    int16_t power = (int16_t)power_str.toInt();
    if (motor == "M0")      _motors[0].setPower(power);
    else if (motor == "M1") _motors[1].setPower(power);
    else if (motor == "M2") _motors[2].setPower(power);
    else if (motor == "M3") _motors[3].setPower(power);
}

//------------------------ PowerMonitor wrappers ------------------------//

int RobotShieldBridge::bridgeGetBatVolt(String dummy)
{
    (void)dummy;
    return (int)_power.batVolt();
}

int RobotShieldBridge::bridgeGetBatPercent(String dummy)
{
    (void)dummy;
    return (int)_power.batPercent();
}

int RobotShieldBridge::bridgeGetBatStatus(String dummy)
{
    (void)dummy;
    return (int)_power.batStatus();
}

int RobotShieldBridge::bridgeGetArduinoCurrent(String dummy)
{
    (void)dummy;
    return (int)_power.arduinoCurrent();
}

int RobotShieldBridge::bridgeGetIorefVolt(String dummy)
{
    (void)dummy;
    return (int)_power.iorefVolt();
}
