/**
 * @file RobotShieldBridge.h
 * @brief Bridge RPC registration hub.
 *
 * Maintains static arrays of all module instances (12 PWM, 12 Servo, 4 Motor,
 * 1 Power). Static wrapper methods parse Bridge String arguments and
 * delegate to the appropriate instance.
 *
 * All 14 Bridge function names are a contract with the Python side — do NOT
 * rename without coordinating both sides.
 */

#ifndef ROBOT_SHIELD_BRIDGE_H
#define ROBOT_SHIELD_BRIDGE_H

#include <Arduino.h>

class RobotShieldBridge {
public:
    /**
     * @brief Initialise all module instances (PWM, Servo, Motor, Power).
     */
    static void begin();

    /**
     * @brief Register all 14 Bridge function bindings.
     *        Must be called after Bridge.begin().
     */
    static void registerAll();

    // ---- PWM Bridge wrappers --------------------------------------------

    /**
     * @brief [Bridge] Set PWM frequency.
     * @param ch_str PWM channel as string ("0"–"11").
     * @param hz_str Frequency as string (Hz).
     */
    static void bridgePwmSetFreq(String ch_str, String hz_str);

    /**
     * @brief [Bridge] Set PWM pulse width and auto-enable the channel.
     * @param ch_str   PWM channel as string ("0"–"11").
     * @param pulse_str Pulse width as string (us), 0–65535.
     */
    static void bridgePwmSetPulse(String ch_str, String pulse_str);

    /**
     * @brief [Bridge] Read PWM pulse width.
     * @param ch_str PWM channel as string ("0"–"11").
     * @return int Pulse width in microseconds.
     */
    static int bridgePwmGetPulse(String ch_str);

    /**
     * @brief [Bridge] Enable or disable a PWM channel.
     * @param ch_str PWM channel as string ("0"–"11").
     * @param en_str "1" = enable, "0" = disable.
     * @return int Always returns 0.
     */
    static int bridgePwmEnable(String ch_str, String en_str);

    // ---- Servo Bridge wrappers ------------------------------------------

    /**
     * @brief [Bridge] Set servo angle.
     * @param ch_str   Servo channel as string ("0"–"11").
     * @param angle_str Target angle as string (-90 to +90 degrees).
     */
    static void bridgeServoSetAngle(String ch_str, String angle_str);

    /**
     * @brief [Bridge] Read servo angle.
     * @param ch_str Servo channel as string ("0"–"11").
     * @return int Current angle in degrees (-90 to +90).
     */
    static int bridgeServoGetAngle(String ch_str);

    // ---- Motor Bridge wrappers ------------------------------------------

    /**
     * @brief [Bridge] Set motor power.
     * @param motor     Motor id ("M0"–"M3").
     * @param power_str Power as string (-100 to +100).
     */
    static void bridgeMotorSetPower(String motor, String power_str);

    // ---- Power Bridge wrappers -----------------------------------

    /** @brief [Bridge] Read battery voltage. @param dummy Unused (Bridge requirement). @return int Voltage in mV. */
    static int bridgeGetBatVolt(String dummy);

    /** @brief [Bridge] Read battery percentage. @param dummy Unused. @return int Percentage (0–100). */
    static int bridgeGetBatPercent(String dummy);

    /** @brief [Bridge] Read battery status. @param dummy Unused. @return int 0=Normal, 1=Low, 2=Over, 3=Over Current. */
    static int bridgeGetBatStatus(String dummy);

    /** @brief [Bridge] Read Arduino current. @param dummy Unused. @return int Current in mA. */
    static int bridgeGetArduinoCurrent(String dummy);

    /** @brief [Bridge] Read IOREF voltage. @param dummy Unused. @return int Voltage in mV. */
    static int bridgeGetIorefVolt(String dummy);
};

#endif // ROBOT_SHIELD_BRIDGE_H
