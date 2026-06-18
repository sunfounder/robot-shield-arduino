/**
 * @example BasicUsage.ino
 * @brief Minimal setup — initialises I2cBus and all modules via RobotShieldBridge.
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

    Serial.println("=== RobotShield Ready ===");
}

void loop()
{
    delay(100);
}
