#ifndef ROBOT_SHIELD_BRIDGE_H
#define ROBOT_SHIELD_BRIDGE_H

#include <Arduino.h>

class RobotShieldBridge {
public:
    static void begin();
    static void registerAll();

    // PWM Bridge wrappers
    static void bridgePwmSetFreq(String ch_str, String hz_str);
    static void bridgePwmSetPulse(String ch_str, String pulse_str);
    static int  bridgePwmGetPulse(String ch_str);
    static int  bridgePwmEnable(String ch_str, String en_str);

    // Servo Bridge wrappers
    static void bridgeServoSetAngle(String ch_str, String angle_str);
    static int  bridgeServoGetAngle(String ch_str);

    // Motor Bridge wrappers
    static void bridgeMotorSetPower(String motor, String power_str);

    // PowerMonitor Bridge wrappers
    static int bridgeGetBatVolt(String dummy);
    static int bridgeGetBatPercent(String dummy);
    static int bridgeGetBatStatus(String dummy);
    static int bridgeGetArduinoCurrent(String dummy);
    static int bridgeGetIorefVolt(String dummy);
};

#endif // ROBOT_SHIELD_BRIDGE_H
