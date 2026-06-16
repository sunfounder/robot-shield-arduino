/**
 * @file i2c.cpp
 * @brief Low-level I2C register read/write primitives over Arduino Wire.
 *
 * Write sequence: START → dev_addr+W → reg_addr → data → STOP
 * Read sequence:  START → dev_addr+W → reg_addr → STOP →
 *                 START → dev_addr+R → read data → STOP
 */

#include <Arduino.h>
#include <Wire.h>
#include "i2c.h"

//------------------------ Implementation ------------------------//

/**
 * @brief Write a single byte to an I2C device register.
 * @param dev_addr  I2C device address.
 * @param reg_addr  Register address to write to.
 * @param data      Byte value to write.
 */
void _i2c_write_reg(byte dev_addr, byte reg_addr, byte data)
{
    Wire.beginTransmission(dev_addr);
    Wire.write(reg_addr);
    Wire.write(data);
    Wire.endTransmission();
}

/**
 * @brief Read a single byte from an I2C device register.
 * @param dev_addr  I2C device address.
 * @param reg_addr  Register address to read from.
 * @param len       Number of bytes to read.
 * @return          Register value, or 0xFF on NACK.
 */
byte _i2c_read_reg(byte dev_addr, byte reg_addr, byte len)
{
    Wire.beginTransmission(dev_addr);
    Wire.write(reg_addr);
    Wire.endTransmission(false);

    Wire.requestFrom(dev_addr, len);
    if (Wire.available())
        return Wire.read();

    return 0xFF;
}

//------------------------ Bridge API ------------------------//

/**
 * @brief Generic I2C register read — Bridge pass-through for Python polling.
 * @param addr_str  Register address as a decimal string.
 * @return          Register value (0–255), or 0xFF on NACK.
 */
int read_reg(String addr_str)
{
    int addr = addr_str.toInt();
    return _i2c_read_reg(I2C_SLAVE_ADDR, (byte)addr, 1);
}

/**
 * @brief Generic I2C register write — Bridge pass-through for Python control.
 * @param addr   Register address.
 * @param value  Byte value to write.
 * @return       0.
 */
int write_reg(int addr, int value)
{
    _i2c_write_reg(I2C_SLAVE_ADDR, (byte)addr, (byte)value);
    return 0;
}
