/**
 * @file I2cBus.cpp
 * @brief I2cBus singleton implementation — Wire read/write to co-processor @ 0x20.
 */

#include "I2cBus.h"
#include <Wire.h>

void I2cBus::begin(uint8_t dev_addr)
{
    _dev_addr = dev_addr;
    Wire.begin();
}

uint8_t I2cBus::readReg(uint8_t reg, uint8_t len)
{
    // Write register address, then read — standard I2C read sequence
    Wire.beginTransmission(_dev_addr);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(_dev_addr, len);
    if (Wire.available())
        return Wire.read();

    return 0xFF;
}

void I2cBus::writeReg(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(_dev_addr);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

I2cBus& I2cBus::instance()
{
    static I2cBus bus;
    return bus;
}

int I2cBus::bridgeReadReg(String addr_str)
{
    int addr = addr_str.toInt();
    return instance().readReg((uint8_t)addr, 1);
}

int I2cBus::bridgeWriteReg(int addr, int value)
{
    instance().writeReg((uint8_t)addr, (uint8_t)value);
    return 0;
}
