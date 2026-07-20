#include "RobotShield.h"
#include "Arduino_RouterBridge.h"
#include "reg_map.h"

static Pwm _pwms[12] = {
    Pwm(0), Pwm(1), Pwm(2), Pwm(3),
    Pwm(4), Pwm(5), Pwm(6), Pwm(7),
    Pwm(8), Pwm(9), Pwm(10), Pwm(11)
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

static Power _power;

void RobotShield::begin()
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

void RobotShield::registerAll()
{
    Bridge.provide("read_reg", I2cBus::bridgeReadReg);
    Bridge.provide("write_reg", I2cBus::bridgeWriteReg);

    Bridge.provide("pwm_set_freq",  bridgePwmSetFreq);
    Bridge.provide("pwm_set_pulse", bridgePwmSetPulse);
    Bridge.provide("pwm_get_pulse", bridgePwmGetPulse);
    Bridge.provide("pwm_enable",    bridgePwmEnable);

    Bridge.provide("servo_set_angle", bridgeServoSetAngle);
    Bridge.provide("servo_get_angle", bridgeServoGetAngle);

    Bridge.provide("motor_set_power", bridgeMotorSetPower);

    Bridge.provide("get_bat_volt",        bridgeGetBatVolt);
    Bridge.provide("get_bat_percent",     bridgeGetBatPercent);
    Bridge.provide("get_bat_status",      bridgeGetBatStatus);
    // Bridge.provide("get_arduino_current", bridgeGetArduinoCurrent);  // deprecated
    Bridge.provide("get_ioref_volt",      bridgeGetIorefVolt);

    Bridge.provide("usr_btn_read", bridgeUsrBtnRead);
}

//------------------------ PWM wrappers ------------------------//

void RobotShield::bridgePwmSetFreq(String ch_str, String hz_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    uint16_t hz = (uint16_t)hz_str.toInt();
    if (ch < 12) _pwms[ch].setFreq(hz);
}

void RobotShield::bridgePwmSetPulse(String ch_str, String pulse_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    int pulse = pulse_str.toInt();
    if (pulse < 0) pulse = 0;
    if (pulse > 65535) pulse = 65535;
    if (ch < 12) {
        _pwms[ch].setPulse((uint16_t)pulse);
        _pwms[ch].setEnable(true);
    }
}

int RobotShield::bridgePwmGetPulse(String ch_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    if (ch < 12) return (int)_pwms[ch].getPulse();
    return 0;
}

int RobotShield::bridgePwmEnable(String ch_str, String en_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    uint8_t en = (uint8_t)en_str.toInt();
    if (ch < 12) _pwms[ch].setEnable(en ? true : false);
    return 0;
}

//------------------------ Servo wrappers ------------------------//

void RobotShield::bridgeServoSetAngle(String ch_str, String angle_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    int16_t angle = (int16_t)angle_str.toInt();
    if (ch < 12) _servos[ch].setAngle(angle);
}

int RobotShield::bridgeServoGetAngle(String ch_str)
{
    uint8_t ch = (uint8_t)ch_str.toInt();
    if (ch < 12) return (int)_servos[ch].getAngle();
    return 0;
}

//------------------------ Motor wrappers ------------------------//

void RobotShield::bridgeMotorSetPower(String motor, String power_str)
{
    int16_t power = (int16_t)power_str.toInt();
    if (motor == "M0")      _motors[0].setPower(power);
    else if (motor == "M1") _motors[1].setPower(power);
    else if (motor == "M2") _motors[2].setPower(power);
    else if (motor == "M3") _motors[3].setPower(power);
}

//------------------------ Power wrappers ------------------------//

int RobotShield::bridgeGetBatVolt(String dummy)
{
    (void)dummy;
    return (int)_power.getVoltage();
}

int RobotShield::bridgeGetBatPercent(String dummy)
{
    (void)dummy;
    return (int)_power.getPercent();
}

int RobotShield::bridgeGetBatStatus(String dummy)
{
    (void)dummy;
    return (int)_power.getStatus();
}

// int RobotShield::bridgeGetArduinoCurrent(String dummy)
// {
//     (void)dummy;
//     return (int)_power.getCurrent();
// }

int RobotShield::bridgeGetIorefVolt(String dummy)
{
    (void)dummy;
    return (int)_power.getIoref();
}

//------------------------ Button wrapper ------------------------//

int RobotShield::bridgeUsrBtnRead(String dummy)
{
    (void)dummy;
    return (int)I2cBus::i2c().readReg(REG_USR_KEY_SIGNAL, 1);
}
