/**
 * @example ServoSweep.ino
 * @brief Sweep the pan servo (channel 0) back and forth between -45 and +45 degrees.
 *
 * Demonstrates Servo setAngle() and getAngle().
 * Prints current angle to Serial every frame.
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

    Serial.println("=== ServoSweep Ready ===");
}

void loop()
{
    // Sweep from -45 to +45
    for (int16_t angle = -45; angle <= 45; angle += 2) {
        Bridge.call("servo_set_angle", "0", String(angle));
        Serial.print("Servo 0 angle: ");
        Serial.println(angle);
        delay(30);
    }
    // Sweep back
    for (int16_t angle = 45; angle >= -45; angle -= 2) {
        Bridge.call("servo_set_angle", "0", String(angle));
        Serial.print("Servo 0 angle: ");
        Serial.println(angle);
        delay(30);
    }
}
