/**
 * @example MotorTest.ino
 * @brief Test M0 motor: forward 3s → brake 1s → reverse 3s → brake.
 *
 * Demonstrates Motor setPower() via Bridge RPC.
 */

#include "RobotShield.h"
#include "Arduino_RouterBridge.h"
#include <Wire.h>

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    I2cBus::instance().begin();
    RobotShieldBridge::begin();

    Bridge.begin();
    RobotShieldBridge::registerAll();

    Serial.println("=== MotorTest Ready ===");
}

void loop()
{
    Serial.println("M0: Forward 50%");
    Bridge.call("motor_set_power", "M0", "50");
    delay(3000);

    Serial.println("M0: Brake");
    Bridge.call("motor_set_power", "M0", "0");
    delay(1000);

    Serial.println("M0: Reverse 50%");
    Bridge.call("motor_set_power", "M0", "-50");
    delay(3000);

    Serial.println("M0: Brake");
    Bridge.call("motor_set_power", "M0", "0");
    delay(3000);
}
