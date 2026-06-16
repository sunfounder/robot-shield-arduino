/**
 * @file power_monitor.cpp
 * @brief Power telemetry — reads battery/current/voltage registers over I2C.
 *
 * Register values are continuously updated by the co-processor's ADC and
 * polled at 1 Hz by the Python register-polling thread.
 */

#include "power_monitor.h"
#include "reg_map.h"
#include "i2c.h"

//------------------------ Implementation ------------------------//

/**
 * @brief Read battery voltage from the co-processor.
 * @param dummy  Unused (Bridge calling convention).
 * @return       Battery voltage in mV.
 */
int get_bat_volt(String dummy)
{
    return _i2c_read_reg(I2C_SLAVE_ADDR, REG_BAT_VOLT, 1);
}

/**
 * @brief Read battery charge percentage from the co-processor.
 * @param dummy  Unused (Bridge calling convention).
 * @return       Battery percentage (0–100).
 */
int get_bat_percent(String dummy)
{
    return _i2c_read_reg(I2C_SLAVE_ADDR, REG_BAT_PERCENT, 1);
}

/**
 * @brief Read battery status flags from the co-processor.
 * @param dummy  Unused (Bridge calling convention).
 * @return       0=Normal, 1=Low Voltage, 2=Over Voltage, 3=Over Current.
 */
int get_bat_status(String dummy)
{
    return _i2c_read_reg(I2C_SLAVE_ADDR, REG_BAT_STATUS, 1);
}

/**
 * @brief Read Arduino board current draw from the co-processor.
 * @param dummy  Unused (Bridge calling convention).
 * @return       Current in mA.
 */
int get_arduino_current(String dummy)
{
    return _i2c_read_reg(I2C_SLAVE_ADDR, REG_ARDUINO_CURRENT, 1);
}

/**
 * @brief Read IOREF voltage from the co-processor.
 * @param dummy  Unused (Bridge calling convention).
 * @return       Voltage in mV.
 */
int get_ioref_volt(String dummy)
{
    return _i2c_read_reg(I2C_SLAVE_ADDR, REG_IOREF_VOLT, 1);
}
