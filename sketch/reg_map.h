#ifndef __REGISTER_MAP_H
#define __REGISTER_MAP_H

#include <Arduino.h>

// Total register address space
#define REG_MAP_SIZE 255

//------------------------ System registers (0x00–0x0C) ------------------------//

#define REG_CHIP_ID          0x00  // Chip identification code
#define REG_I2C_ADDR         0x01  // I2C address configuration
#define REG_IO_VOLT          0x02  // IO voltage setting (0=Auto)
#define REG_IOREF_VOLT       0x03  // IOREF voltage readout
#define REG_FW_VER_MAJOR     0x04  // Firmware version — major
#define REG_FW_VER_MINOR     0x05  // Firmware version — minor
#define REG_FW_VER_PATCH     0x06  // Firmware version — patch
#define REG_POWER_SWITCH     0x07  // Power switch control
#define REG_AUTO_SHUTDOWN    0x08  // Auto-shutdown configuration
#define REG_SYS_CTRL         0x09  // System control (e.g. servo center)
#define REG_SHUTDOWN_SIGNAL  0x0A  // Write 0x01 to trigger PWR shutdown
#define REG_KEY_SIGNAL            0x0B  // PWR button event
#define KEY_SIGNAL_PRESSED        0x01  // Single click
#define KEY_SIGNAL_ZERO_ENTER     0x02  // Double click enter zero
#define KEY_SIGNAL_ZERO_EXIT      0x03  // Double click exit zero
#define REG_USR_KEY_SIGNAL        0x0C  // USR button event
#define USR_KEY_PTT_START         0x01  // PTT record start (press)
#define USR_KEY_PTT_STOP          0x02  // PTT record stop (release)

//------------------------ Battery / power registers (0x20–0x23) ------------------------//

#define REG_BAT_VOLT         0x20  // Battery voltage
#define REG_BAT_PERCENT      0x21  // Battery charge percentage
#define REG_BAT_STATUS       0x22  // Battery status flags
#define REG_ARDUINO_CURRENT  0x23  // Arduino board current draw

//------------------------ Raw ADC registers (0x25–0x2A) — 16-bit, LE ------------------------//

#define REG_RAW_BAT_ADC_L    0x25  // Raw battery ADC — low byte
#define REG_RAW_BAT_ADC_H    0x26  // Raw battery ADC — high byte
#define REG_RAW_CUR_ADC_L    0x27  // Raw current ADC — low byte
#define REG_RAW_CUR_ADC_H    0x28  // Raw current ADC — high byte
#define REG_RAW_IOREF_ADC_L  0x29  // Raw IOREF ADC — low byte
#define REG_RAW_IOREF_ADC_H  0x2A  // Raw IOREF ADC — high byte

//------------------------ PWM control registers (0x40–0x4B) ------------------------//

#define REG_PWM0_CTRL        0x40
#define REG_PWM1_CTRL        0x41
#define REG_PWM2_CTRL        0x42
#define REG_PWM3_CTRL        0x43
#define REG_PWM4_CTRL        0x44
#define REG_PWM5_CTRL        0x45
#define REG_PWM6_CTRL        0x46
#define REG_PWM7_CTRL        0x47
#define REG_PWM8_CTRL        0x48
#define REG_PWM9_CTRL        0x49
#define REG_PWM10_CTRL       0x4A
#define REG_PWM11_CTRL       0x4B

//------------------------ PWM period registers (0x50–0x67) — 16-bit, LE ------------------------//

#define REG_PWM0_PERIOD_L    0x50
#define REG_PWM0_PERIOD_H    0x51
#define REG_PWM1_PERIOD_L    0x52
#define REG_PWM1_PERIOD_H    0x53
#define REG_PWM2_PERIOD_L    0x54
#define REG_PWM2_PERIOD_H    0x55
#define REG_PWM3_PERIOD_L    0x56
#define REG_PWM3_PERIOD_H    0x57
#define REG_PWM4_PERIOD_L    0x58
#define REG_PWM4_PERIOD_H    0x59
#define REG_PWM5_PERIOD_L    0x5A
#define REG_PWM5_PERIOD_H    0x5B
#define REG_PWM6_PERIOD_L    0x5C
#define REG_PWM6_PERIOD_H    0x5D
#define REG_PWM7_PERIOD_L    0x5E
#define REG_PWM7_PERIOD_H    0x5F
#define REG_PWM8_PERIOD_L    0x60
#define REG_PWM8_PERIOD_H    0x61
#define REG_PWM9_PERIOD_L    0x62
#define REG_PWM9_PERIOD_H    0x63
#define REG_PWM10_PERIOD_L   0x64
#define REG_PWM10_PERIOD_H   0x65
#define REG_PWM11_PERIOD_L   0x66
#define REG_PWM11_PERIOD_H   0x67

//------------------------ PWM pulse-width registers (0x70–0x87) — 16-bit, LE ------------------------//

#define REG_PWM0_PULSE_L     0x70
#define REG_PWM0_PULSE_H     0x71
#define REG_PWM1_PULSE_L     0x72
#define REG_PWM1_PULSE_H     0x73
#define REG_PWM2_PULSE_L     0x74
#define REG_PWM2_PULSE_H     0x75
#define REG_PWM3_PULSE_L     0x76
#define REG_PWM3_PULSE_H     0x77
#define REG_PWM4_PULSE_L     0x78
#define REG_PWM4_PULSE_H     0x79
#define REG_PWM5_PULSE_L     0x7A
#define REG_PWM5_PULSE_H     0x7B
#define REG_PWM6_PULSE_L     0x7C
#define REG_PWM6_PULSE_H     0x7D
#define REG_PWM7_PULSE_L     0x7E
#define REG_PWM7_PULSE_H     0x7F
#define REG_PWM8_PULSE_L     0x80
#define REG_PWM8_PULSE_H     0x81
#define REG_PWM9_PULSE_L     0x82
#define REG_PWM9_PULSE_H     0x83
#define REG_PWM10_PULSE_L    0x84
#define REG_PWM10_PULSE_H    0x85
#define REG_PWM11_PULSE_L    0x86
#define REG_PWM11_PULSE_H    0x87

//------------------------ Power-on defaults ------------------------//

#define CHIP_ID_VALUE        0x01  // Default chip ID
#define I2C_ADDR_VALUE       0x20  // Default I2C address
#define IO_VOLT_VALUE        0x00  // IO voltage: 0=Auto
#define POWER_SWITCH_VALUE   0xC1  // Power switch default
#define AUTO_SHUTDOWN_VALUE  0x8A  // Auto-shutdown default
#define SYS_CTRL_VALUE       0x00  // System control default

// 0x4E20 = 20000 μs → 50 Hz
#define PWM_PERIOD_L_VALUE   0x20  // PWM period default — low byte
#define PWM_PERIOD_H_VALUE   0x4E  // PWM period default — high byte
#define PWM_PULSE_L_VALUE    0x00  // PWM pulse default — low byte
#define PWM_PULSE_H_VALUE    0x00  // PWM pulse default — high byte

#endif /* __REGISTER_MAP_H */
