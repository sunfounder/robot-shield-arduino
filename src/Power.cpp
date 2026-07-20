/**
 * @file Power.cpp
 * @brief Power implementation — reads co-processor battery/power registers via I2cBus.
 */

#include "Power.h"
#include "reg_map.h"
#include "I2cBus.h"

void Power::begin()
{
    // No init needed — registers are maintained by co-processor ADC.
}

uint8_t Power::getVoltage()
{
    return I2cBus::i2c().readReg(REG_BAT_VOLT, 1);
}

uint8_t Power::getPercent()
{
    return I2cBus::i2c().readReg(REG_BAT_PERCENT, 1);
}

uint8_t Power::getStatus()
{
    return I2cBus::i2c().readReg(REG_BAT_STATUS, 1);
}

// uint8_t Power::getCurrent()
// {
//     return I2cBus::i2c().readReg(REG_ARDUINO_CURRENT, 1);
// }

uint8_t Power::getIoref()
{
    return I2cBus::i2c().readReg(REG_IOREF_VOLT, 1);
}
