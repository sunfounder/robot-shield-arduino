/**
 * @example PowerReadout.ino
 * @brief Read and print battery/power telemetry to Serial every 2 seconds.
 *
 * Demonstrates PowerMonitor via Bridge RPC (get_bat_volt etc.).
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

    Serial.println("=== PowerReadout Ready ===");
}

void loop()
{
    int volt    = Bridge.call("get_bat_volt", "");
    int percent = Bridge.call("get_bat_percent", "");
    int status  = Bridge.call("get_bat_status", "");
    int current = Bridge.call("get_arduino_current", "");
    int ioref   = Bridge.call("get_ioref_volt", "");

    Serial.print("Battery: ");
    Serial.print(volt);
    Serial.print(" mV  ");
    Serial.print(percent);
    Serial.print("%  status=");
    Serial.print(status);
    Serial.print("  current=");
    Serial.print(current);
    Serial.print(" mA  IOREF=");
    Serial.print(ioref);
    Serial.println(" mV");

    delay(2000);
}
