/**
 * @example ServoSweep.ino
 * @brief Sweep servo on channel 0 back and forth between -45 and +45 degrees.
 */

#include "RobotShield.h"

Servo servo(0);

void setup()
{
    Serial.begin(115200);
    I2cBus::instance().begin();      // Wire + I2C addr init
    servo.begin();

    Serial.println("=== ServoSweep Ready ===");
}

void loop()
{
    for (int16_t angle = -45; angle <= 45; angle += 2) {
        servo.setAngle(angle);
        Serial.print("Servo 0 angle: ");
        Serial.println(angle);
        delay(30);
    }
    for (int16_t angle = 45; angle >= -45; angle -= 2) {
        servo.setAngle(angle);
        Serial.print("Servo 0 angle: ");
        Serial.println(angle);
        delay(30);
    }
}
