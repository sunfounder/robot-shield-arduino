/**
 * @example PowerReadout.ino
 * @brief Read and print battery/power telemetry to Serial every 2 seconds.
 */

#include "RobotShield.h"
#include "Arduino_RouterBridge.h"
#include <Wire.h>

PowerMonitor power;

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    Bridge.begin();    // hardware init (may be required for I2C clock)
    I2cBus::instance().begin();
    power.begin();

    Serial.println("=== PowerReadout Ready ===");
}

void loop()
{
    Serial.print("Battery: ");
    Serial.print(power.batVolt());
    Serial.print(" mV  ");
    Serial.print(power.batPercent());
    Serial.print("%  status=");
    Serial.print(power.batStatus());
    Serial.print("  current=");
    Serial.print(power.arduinoCurrent());
    Serial.print(" mA  IOREF=");
    Serial.print(power.iorefVolt());
    Serial.println(" mV");

    delay(2000);
}
