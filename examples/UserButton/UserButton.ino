/**
 * @example UserButton.ino
 * @brief Poll USR button state via I2C and print press/release events to Serial.
 */

#include "RobotShield.h"
#include "reg_map.h"

void setup()
{
    Serial.begin(115200);
    I2cBus::i2c().begin();

    Serial.println("=== UserButton Ready ===");
}

void loop()
{
    uint8_t state = I2cBus::i2c().readReg(REG_USR_KEY_SIGNAL, 1);

    if (state == USR_KEY_PRESSED) 
    {
        Serial.println("USR button: PRESSED");
    } 
    else 
    {
        Serial.println("USR button: released");
    }

    delay(200);
}
