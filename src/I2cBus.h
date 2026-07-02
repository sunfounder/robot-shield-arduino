/**
 * @file I2cBus.h
 * @brief I2C bus singleton for reading/writing co-processor registers at address 0x20.
 */

#ifndef I2C_BUS_H
#define I2C_BUS_H

#include <Arduino.h>

class I2cBus {
public:
    static constexpr uint8_t DEFAULT_ADDR = 0x20;  ///< Default I2C slave address.

    /**
     * @brief Initialise the I2C bus and Wire hardware.
     * @param dev_addr I2C slave address, default 0x20.
     */
    void begin(uint8_t dev_addr = DEFAULT_ADDR);

    /**
     * @brief Read a register from the I2C slave.
     * @param reg Register address (0x00–0xFF).
     * @param len Number of bytes to read (default 1).
     * @return uint8_t Register value, or 0xFF on NACK.
     */
    uint8_t readReg(uint8_t reg, uint8_t len = 1);

    /**
     * @brief Write a value to an I2C slave register.
     * @param reg  Register address (0x00–0xFF).
     * @param data Byte value to write (0–255).
     */
    void writeReg(uint8_t reg, uint8_t data);

    /**
     * @brief Get the global I2cBus singleton instance.
     * @return I2cBus& Reference to the singleton.
     */
    static I2cBus& i2c();

    // Bridge RPC wrappers -------------------------------------------------

    /**
     * @brief [Bridge] Read a register. Called by Python via Bridge RPC.
     * @param addr_str Register address as a decimal string.
     * @return int Register value (0–255), or 0xFF on NACK.
     */
    static int bridgeReadReg(String addr_str);

    /**
     * @brief [Bridge] Write a register. Called by Python via Bridge RPC.
     * @param addr  Register address.
     * @param value Byte value to write (0–255).
     * @return int Always returns 0.
     */
    static int bridgeWriteReg(int addr, int value);

private:
    uint8_t _dev_addr;  ///< Current I2C slave address.
};

#endif // I2C_BUS_H
