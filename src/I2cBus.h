#ifndef I2C_BUS_H
#define I2C_BUS_H

#include <Arduino.h>

class I2cBus {
public:
    static constexpr uint8_t DEFAULT_ADDR = 0x20;

    void begin(uint8_t dev_addr = DEFAULT_ADDR);

    uint8_t readReg(uint8_t reg, uint8_t len = 1);
    void writeReg(uint8_t reg, uint8_t data);

    static I2cBus& instance();

    // Bridge RPC wrappers
    static int bridgeReadReg(String addr_str);
    static int bridgeWriteReg(int addr, int value);

private:
    uint8_t _dev_addr;
};

#endif // I2C_BUS_H
