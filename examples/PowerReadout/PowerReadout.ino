/**
 * @example PowerReadout.ino
 * @brief Read and print battery/power telemetry to Serial every 2 seconds.
 */

#include "RobotShield.h"

Power power;

void setup()
{
    Serial.begin(115200);
    I2cBus::i2c().begin();      // Wire + I2C addr init
    power.begin();

    Serial.println("=== PowerReadout Ready ===");
}

void loop()
{
    Serial.print("Battery: ");
    Serial.print(power.getVoltage());
    Serial.print(" mV  ");
    Serial.print(power.getPercent());
    Serial.print("%  status=");
    Serial.print(power.getStatus());
    Serial.print("  current=");
    Serial.print(power.getCurrent());
    Serial.print(" mA  IOREF=");
    Serial.print(power.getIoref());
    Serial.println(" mV");

    delay(2000);
}
