/**
 * @file PowerMonitor.h
 * @brief Power telemetry — reads battery/current/voltage registers via I2cBus.
 *
 * Register values are maintained by the co-processor's ADC and read on demand.
 */

#ifndef POWER_MONITOR_H
#define POWER_MONITOR_H

#include <Arduino.h>

class PowerMonitor {
public:
    /**
     * @brief Initialise the power monitor. No hardware setup required —
     *        registers are maintained by the co-processor ADC.
     */
    void begin();

    /**
     * @brief Read battery voltage.
     * @return uint8_t Battery voltage in mV.
     */
    uint8_t batVolt();

    /**
     * @brief Read battery charge percentage.
     * @return uint8_t Battery percentage (0–100).
     */
    uint8_t batPercent();

    /**
     * @brief Read battery status flags.
     * @return uint8_t 0=Normal, 1=Low Voltage, 2=Over Voltage, 3=Over Current.
     */
    uint8_t batStatus();

    /**
     * @brief Read Arduino board current draw.
     * @return uint8_t Current in mA.
     */
    uint8_t arduinoCurrent();

    /**
     * @brief Read IOREF voltage.
     * @return uint8_t IOREF voltage in mV.
     */
    uint8_t iorefVolt();
};

#endif // POWER_MONITOR_H
