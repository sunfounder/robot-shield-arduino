/**
 * @file PowerMonitor.cpp
 * @brief PowerMonitor implementation — reads co-processor battery/power registers via I2cBus.
 */

#include "PowerMonitor.h"
#include "reg_map.h"
#include "I2cBus.h"

void PowerMonitor::begin()
{
    // No init needed — registers are maintained by co-processor ADC.
}

uint8_t PowerMonitor::batVolt()
{
    return I2cBus::instance().readReg(REG_BAT_VOLT, 1);
}

uint8_t PowerMonitor::batPercent()
{
    return I2cBus::instance().readReg(REG_BAT_PERCENT, 1);
}

uint8_t PowerMonitor::batStatus()
{
    return I2cBus::instance().readReg(REG_BAT_STATUS, 1);
}

uint8_t PowerMonitor::arduinoCurrent()
{
    return I2cBus::instance().readReg(REG_ARDUINO_CURRENT, 1);
}

uint8_t PowerMonitor::iorefVolt()
{
    return I2cBus::instance().readReg(REG_IOREF_VOLT, 1);
}
