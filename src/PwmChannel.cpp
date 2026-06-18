#include "PwmChannel.h"
#include "reg_map.h"
#include "I2cBus.h"

PwmChannel::PwmChannel(uint8_t ch)
    : _ch(ch)
{}

void PwmChannel::begin()
{
    I2cBus& i2c = I2cBus::instance();
    uint8_t reg_l = REG_PWM0_PERIOD_L + _ch * 2;
    i2c.writeReg(reg_l, PWM_PERIOD_L_VALUE);
    i2c.writeReg(reg_l + 1, PWM_PERIOD_H_VALUE);
}

void PwmChannel::setFreq(uint16_t hz)
{
    if (hz == 0) return;
    uint16_t period_us = (uint16_t)(1000000 / hz);
    I2cBus& i2c = I2cBus::instance();
    uint8_t reg_l = REG_PWM0_PERIOD_L + _ch * 2;
    i2c.writeReg(reg_l, period_us & 0xFF);
    i2c.writeReg(reg_l + 1, (period_us >> 8) & 0xFF);
}

void PwmChannel::setPulse(uint16_t us)
{
    I2cBus& i2c = I2cBus::instance();
    uint8_t reg_l = REG_PWM0_PULSE_L + _ch * 2;
    i2c.writeReg(reg_l, us & 0xFF);
    i2c.writeReg(reg_l + 1, (us >> 8) & 0xFF);
}

uint16_t PwmChannel::getPulse()
{
    I2cBus& i2c = I2cBus::instance();
    uint8_t reg_l = REG_PWM0_PULSE_L + _ch * 2;
    uint8_t low = i2c.readReg(reg_l, 1);
    uint8_t high = i2c.readReg(reg_l + 1, 1);
    return ((uint16_t)high << 8) | low;
}

uint16_t PwmChannel::getPeriod()
{
    I2cBus& i2c = I2cBus::instance();
    uint8_t reg_l = REG_PWM0_PERIOD_L + _ch * 2;
    uint8_t low = i2c.readReg(reg_l, 1);
    uint8_t high = i2c.readReg(reg_l + 1, 1);
    return ((uint16_t)high << 8) | low;
}

void PwmChannel::enable(bool on)
{
    I2cBus& i2c = I2cBus::instance();
    uint8_t ctrl_reg = REG_PWM0_CTRL + _ch;
    i2c.writeReg(ctrl_reg, on ? 1 : 0);
}
