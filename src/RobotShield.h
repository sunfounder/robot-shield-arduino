#ifndef ROBOT_SHIELD_H
#define ROBOT_SHIELD_H

#include "I2cBus.h"
#include "Pwm.h"
#include "Servo.h"
#include "Motor.h"
#include "Power.h"

class RobotShield {
public:
    static void begin();
    static void registerAll();

    // PWM
    static void bridgePwmSetFreq(String ch_str, String hz_str);
    static void bridgePwmSetPulse(String ch_str, String pulse_str);
    static int  bridgePwmGetPulse(String ch_str);
    static int  bridgePwmEnable(String ch_str, String en_str);

    // Servo
    static void bridgeServoSetAngle(String ch_str, String angle_str);
    static int  bridgeServoGetAngle(String ch_str);

    // Motor
    static void bridgeMotorSetPower(String motor, String power_str);

    // Power telemetry
    static int bridgeGetBatVolt(String dummy);
    static int bridgeGetBatPercent(String dummy);
    static int bridgeGetBatStatus(String dummy);
    static int bridgeGetArduinoCurrent(String dummy);
    static int bridgeGetIorefVolt(String dummy);
};

#endif
