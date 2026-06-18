/**
 * @example MotorTest.ino
 * @brief Test M0 motor: forward 3s → brake 1s → reverse 3s → brake.
 */

#include "RobotShield.h"
#include "Arduino_RouterBridge.h"
#include <Wire.h>

Motor motor("M0", 4, 5);

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    Bridge.begin();    // hardware init (may be required for I2C clock)
    I2cBus::instance().begin();
    motor.begin();

    Serial.println("=== MotorTest Ready ===");
}

void loop()
{
    Serial.println("M0: Forward 50%");
    motor.setPower(50);
    delay(3000);

    Serial.println("M0: Brake");
    motor.setPower(0);
    delay(1000);

    Serial.println("M0: Reverse 50%");
    motor.setPower(-50);
    delay(3000);

    Serial.println("M0: Brake");
    motor.setPower(0);
    delay(3000);
}
