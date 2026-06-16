#ifndef I2C_H
#define I2C_H

#include <Arduino.h>

// I2C slave address of the co-processor (PCA9685 + power monitor)
#define I2C_SLAVE_ADDR  0x20

// dev_addr: I2C device address, reg_addr: register address, data: byte to write -> return: none
void _i2c_write_reg(byte dev_addr, byte reg_addr, byte data);

// dev_addr: I2C device address, reg_addr: register address, len: bytes to read -> return: register value (0xFF on NACK)
byte _i2c_read_reg(byte dev_addr, byte reg_addr, byte len);

//------------------------ Bridge API ------------------------//

// addr_str: register address as decimal string -> return: register value (0xFF on NACK)
int read_reg(String addr_str);

// addr: register address, value: byte to write -> return: 0
int write_reg(int addr, int value);

#endif
