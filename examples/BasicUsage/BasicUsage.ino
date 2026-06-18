/**
 * @example BasicUsage.ino
 * @brief Minimal setup with Bridge — initialises I2cBus and registers all Bridge bindings.
 */

#include "RobotShield.h"
#include "Arduino_RouterBridge.h"

void setup()
{
    Serial.begin(115200);
    I2cBus::instance().begin();      // Wire + I2C addr init
    RobotShieldBridge::begin();
    Bridge.begin();
    RobotShieldBridge::registerAll();

    Serial.println("=== RobotShield Ready ===");
}

void loop()
{
    delay(100);
}
