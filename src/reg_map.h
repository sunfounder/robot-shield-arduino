/**
 * @file reg_map.h
 * @brief Complete I2C register map for the co-processor at address 0x20.
 *
 * Mirrors python-libraries/robot_shield/robot_shield/reg_map.py.
 * All register addresses are a cross-language contract — do not modify
 * one side without updating the other.
 */

#ifndef REG_MAP_H
#define REG_MAP_H

#include <Arduino.h>

/** @brief Total register address space. */
#define REG_MAP_SIZE 255

//------------------------ System registers (0x00–0x0C) ------------------------//

#define REG_CHIP_ID          0x00  ///< Chip identification code.
#define REG_I2C_ADDR         0x01  ///< I2C address configuration.
#define REG_IO_VOLT          0x02  ///< IO voltage setting (0 = Auto).
#define REG_IOREF_VOLT       0x03  ///< IOREF voltage readout.
#define REG_FW_VER_MAJOR     0x04  ///< Firmware version — major.
#define REG_FW_VER_MINOR     0x05  ///< Firmware version — minor.
#define REG_FW_VER_PATCH     0x06  ///< Firmware version — patch.
#define REG_POWER_SWITCH     0x07  ///< Power switch control.
#define REG_AUTO_SHUTDOWN    0x08  ///< Auto-shutdown configuration.
#define REG_SYS_CTRL         0x09  ///< System control (write 0x40 = servo all-centre).
#define REG_SHUTDOWN_SIGNAL  0x0A  ///< Write 0x01 to trigger PWR long-press shutdown.
#define REG_KEY_SIGNAL            0x0B  ///< PWR button event.
#define KEY_SIGNAL_PRESSED        0x01  ///< Single click.
#define KEY_SIGNAL_ZERO_ENTER     0x02  ///< Double-click enter zero.
#define KEY_SIGNAL_ZERO_EXIT      0x03  ///< Double-click exit zero.
#define REG_USR_KEY_SIGNAL        0x0C  ///< USR button event.
#define USR_KEY_PRESSED           0x00  ///< USR button pressed.
#define USR_KEY_RELEASED          0x01  ///< USR button released.

//------------------------ Battery / power registers (0x20–0x23) ------------------------//

#define REG_BAT_VOLT         0x20  ///< Battery voltage (mV).
#define REG_BAT_PERCENT      0x21  ///< Battery charge percentage (0–100).
#define REG_BAT_STATUS       0x22  ///< Battery status flags (0=Normal, 1=Low, 2=Over, 3=Over Current).
#define REG_ARDUINO_CURRENT  0x23  ///< Arduino board current draw (mA).

//------------------------ Raw ADC registers (0x25–0x2A) — 16-bit, LE ------------------------//

#define REG_RAW_BAT_ADC_L    0x25  ///< Raw battery ADC — low byte.
#define REG_RAW_BAT_ADC_H    0x26  ///< Raw battery ADC — high byte.
#define REG_RAW_CUR_ADC_L    0x27  ///< Raw current ADC — low byte.
#define REG_RAW_CUR_ADC_H    0x28  ///< Raw current ADC — high byte.
#define REG_RAW_IOREF_ADC_L  0x29  ///< Raw IOREF ADC — low byte.
#define REG_RAW_IOREF_ADC_H  0x2A  ///< Raw IOREF ADC — high byte.

//------------------------ PWM control registers (0x40–0x4B) ------------------------//

#define REG_PWM0_CTRL        0x40  ///< PWM0 enable.
#define REG_PWM1_CTRL        0x41  ///< PWM1 enable.
#define REG_PWM2_CTRL        0x42  ///< PWM2 enable.
#define REG_PWM3_CTRL        0x43  ///< PWM3 enable.
#define REG_PWM4_CTRL        0x44  ///< PWM4 enable.
#define REG_PWM5_CTRL        0x45  ///< PWM5 enable.
#define REG_PWM6_CTRL        0x46  ///< PWM6 enable.
#define REG_PWM7_CTRL        0x47  ///< PWM7 enable.
#define REG_PWM8_CTRL        0x48  ///< PWM8 enable.
#define REG_PWM9_CTRL        0x49  ///< PWM9 enable.
#define REG_PWM10_CTRL       0x4A  ///< PWM10 enable.
#define REG_PWM11_CTRL       0x4B  ///< PWM11 enable.

//------------------------ PWM period registers (0x50–0x67) — 16-bit, LE ------------------------//

#define REG_PWM0_PERIOD_L    0x50  ///< PWM0 period low byte.
#define REG_PWM0_PERIOD_H    0x51  ///< PWM0 period high byte.
#define REG_PWM1_PERIOD_L    0x52  ///< PWM1 period low byte.
#define REG_PWM1_PERIOD_H    0x53  ///< PWM1 period high byte.
#define REG_PWM2_PERIOD_L    0x54  ///< PWM2 period low byte.
#define REG_PWM2_PERIOD_H    0x55  ///< PWM2 period high byte.
#define REG_PWM3_PERIOD_L    0x56  ///< PWM3 period low byte.
#define REG_PWM3_PERIOD_H    0x57  ///< PWM3 period high byte.
#define REG_PWM4_PERIOD_L    0x58  ///< PWM4 period low byte.
#define REG_PWM4_PERIOD_H    0x59  ///< PWM4 period high byte.
#define REG_PWM5_PERIOD_L    0x5A  ///< PWM5 period low byte.
#define REG_PWM5_PERIOD_H    0x5B  ///< PWM5 period high byte.
#define REG_PWM6_PERIOD_L    0x5C  ///< PWM6 period low byte.
#define REG_PWM6_PERIOD_H    0x5D  ///< PWM6 period high byte.
#define REG_PWM7_PERIOD_L    0x5E  ///< PWM7 period low byte.
#define REG_PWM7_PERIOD_H    0x5F  ///< PWM7 period high byte.
#define REG_PWM8_PERIOD_L    0x60  ///< PWM8 period low byte.
#define REG_PWM8_PERIOD_H    0x61  ///< PWM8 period high byte.
#define REG_PWM9_PERIOD_L    0x62  ///< PWM9 period low byte.
#define REG_PWM9_PERIOD_H    0x63  ///< PWM9 period high byte.
#define REG_PWM10_PERIOD_L   0x64  ///< PWM10 period low byte.
#define REG_PWM10_PERIOD_H   0x65  ///< PWM10 period high byte.
#define REG_PWM11_PERIOD_L   0x66  ///< PWM11 period low byte.
#define REG_PWM11_PERIOD_H   0x67  ///< PWM11 period high byte.

//------------------------ PWM pulse-width registers (0x70–0x87) — 16-bit, LE ------------------------//

#define REG_PWM0_PULSE_L     0x70  ///< PWM0 pulse low byte.
#define REG_PWM0_PULSE_H     0x71  ///< PWM0 pulse high byte.
#define REG_PWM1_PULSE_L     0x72  ///< PWM1 pulse low byte.
#define REG_PWM1_PULSE_H     0x73  ///< PWM1 pulse high byte.
#define REG_PWM2_PULSE_L     0x74  ///< PWM2 pulse low byte.
#define REG_PWM2_PULSE_H     0x75  ///< PWM2 pulse high byte.
#define REG_PWM3_PULSE_L     0x76  ///< PWM3 pulse low byte.
#define REG_PWM3_PULSE_H     0x77  ///< PWM3 pulse high byte.
#define REG_PWM4_PULSE_L     0x78  ///< PWM4 pulse low byte.
#define REG_PWM4_PULSE_H     0x79  ///< PWM4 pulse high byte.
#define REG_PWM5_PULSE_L     0x7A  ///< PWM5 pulse low byte.
#define REG_PWM5_PULSE_H     0x7B  ///< PWM5 pulse high byte.
#define REG_PWM6_PULSE_L     0x7C  ///< PWM6 pulse low byte.
#define REG_PWM6_PULSE_H     0x7D  ///< PWM6 pulse high byte.
#define REG_PWM7_PULSE_L     0x7E  ///< PWM7 pulse low byte.
#define REG_PWM7_PULSE_H     0x7F  ///< PWM7 pulse high byte.
#define REG_PWM8_PULSE_L     0x80  ///< PWM8 pulse low byte.
#define REG_PWM8_PULSE_H     0x81  ///< PWM8 pulse high byte.
#define REG_PWM9_PULSE_L     0x82  ///< PWM9 pulse low byte.
#define REG_PWM9_PULSE_H     0x83  ///< PWM9 pulse high byte.
#define REG_PWM10_PULSE_L    0x84  ///< PWM10 pulse low byte.
#define REG_PWM10_PULSE_H    0x85  ///< PWM10 pulse high byte.
#define REG_PWM11_PULSE_L    0x86  ///< PWM11 pulse low byte.
#define REG_PWM11_PULSE_H    0x87  ///< PWM11 pulse high byte.

//------------------------ Power-on defaults ------------------------//

#define CHIP_ID_VALUE        0x01  ///< Default chip ID.
#define I2C_ADDR_VALUE       0x20  ///< Default I2C address.
#define IO_VOLT_VALUE        0x00  ///< IO voltage default (0 = Auto).
#define POWER_SWITCH_VALUE   0xC1  ///< Power switch default.
#define AUTO_SHUTDOWN_VALUE  0x8A  ///< Auto-shutdown default.
#define SYS_CTRL_VALUE       0x00  ///< System control default.

/** @brief PWM default period low byte. 0x4E20 = 20000 us → 50 Hz. */
#define PWM_PERIOD_L_VALUE   0x20
/** @brief PWM default period high byte. */
#define PWM_PERIOD_H_VALUE   0x4E
/** @brief PWM default pulse low byte (0 us). */
#define PWM_PULSE_L_VALUE    0x00
/** @brief PWM default pulse high byte. */
#define PWM_PULSE_H_VALUE    0x00

#endif /* REG_MAP_H */
