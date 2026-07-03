# Robot Shield

This project is the Arduino firmware (sketch) for the UNO Q AI Robot, running on the Arduino UNO Q (STM32U5) board. It provides **Bridge** functionality for the Robot Shield, serving as the communication bridge between the Python application layer and I2C hardware peripherals.

## Architecture Overview

```
Python (main.py)  ←→  Bridge RPC  ←→  RobotShieldBridge  ←→  I2cBus  ←→  robot shield (i2c address:0x20)
```

- **Python side**: Calls functions exposed by this library via Arduino Bridge RPC
- **RobotShieldBridge**: Static class responsible for registering all Bridge function bindings and delegating calls to the corresponding instances
- **I2cBus**: I2C bus singleton, encapsulating Wire operations
- **I2C robot shield**: Address `0x20`, responsible for low-level hardware operations such as PWM generation (PCA9685) and ADC sampling (battery/current/voltage)

## Class Design

| Class | File | Description |
|---|---|---|
| `I2cBus` | `I2cBus.h/cpp` | I2C bus singleton, encapsulates register read/write |
| `PwmChannel` | `PwmChannel.h/cpp` | Single PWM channel (frequency, pulse width, enable) |
| `Servo` | `Servo.h/cpp` | Servo angle control (composes PwmChannel), -90°~90° ↔ 500~2500μs |
| `Motor` | `Motor.h/cpp` | DC motor H-bridge control (composes 2 PwmChannels), -100~100 power |
| `PowerMonitor` | `PowerMonitor.h/cpp` | Power telemetry (battery voltage/percentage/status/current/IOREF) |
| `RobotShieldBridge` | `RobotShieldBridge.h/cpp` | Bridge registration hub, maintains static instance arrays |

## Key Features

### 1. I2C Register Pass-through

`read_reg` / `write_reg` encapsulate I2C bus operations as Bridge functions. Python uses these two functions to indirectly read and write all robot shield registers, avoiding I2C multi-master bus contention.

### 2. PWM Control (12 Channels)

Supports 12 PWM channels (0–11) with full control over frequency, pulse width, and enable. Default period is 20000μs (50Hz), suitable for standard servos.

- `pwm_set_freq` — Set PWM frequency
- `pwm_set_pulse` — Set pulse width (microseconds), auto-enables the channel
- `pwm_get_pulse` — Read current pulse width
- `pwm_enable` — Enable/disable channel

### 3. Servo Control

Angle control built on top of the PWM layer. Angle range **-90° ~ +90°**, automatically mapped to pulse width **500 ~ 2500μs** (linear mapping).

Key design: pulse width is written before enabling the channel, preventing the servo from jerking to a stale pulse value on power-up.

- `servo_set_angle` — Set servo angle
- `servo_get_angle` — Read current angle

### 4. DC Motor Control (H-Bridge)

4 DC motors (M0–M3), each using two PWM channels to form an H-bridge drive:

| State | Channel A | Channel B |
|-------|-----------|------------|
| Forward | duty | 0 |
| Reverse | 0 | duty |
| Brake | 0 | 0 (disabled) |

Motor PWM frequency is fixed at **100Hz** (10000μs period), independent of the servo 50Hz.

Channel mapping: M0→PWM4/5, M1→PWM6/7, M2→PWM8/9, M3→PWM10/11

### 5. Power Monitoring

Reads battery status from robot shield registers:

- `get_bat_volt` — Battery voltage (mV)
- `get_bat_percent` — Battery charge percentage (0–100)
- `get_bat_status` — Battery status (0=Normal, 1=Low Voltage, 2=Over Voltage, 3=Over Current)
- `get_arduino_current` — Arduino board current (mA)
- `get_ioref_volt` — IOREF voltage (mV)

### 6. System Control

Robot shield system registers support:
- Power switch control
- **PWR button signal** — Single click / Double-click enter zero / Double-click exit zero
- **USR button signal** — PTT (Push-to-Talk) long press to start recording / release to stop recording

## I2C Register Map

See [reg_map.h](src/reg_map.h) for the complete definitions.

| Address Range | Purpose |
|---------------|---------|
| `0x00–0x0C` | System registers (chip ID, I2C address, firmware version, shutdown, button signals) |
| `0x20–0x23` | Battery/power telemetry (voltage, percentage, status, current) |
| `0x25–0x2A` | Raw ADC values (16-bit, little-endian) |
| `0x40–0x4B` | PWM channel control (1 byte per channel, enable/disable) |
| `0x50–0x67` | PWM period (2 bytes per channel, 16-bit, little-endian) — default 20000μs / 50Hz |
| `0x70–0x87` | PWM pulse width (2 bytes per channel, 16-bit, little-endian) — default 500–2500μs |

## Project File Structure

```
robot_shield_bridge/
├── CLAUDE.md                    ← Claude Code development guide
├── README.md                    ← readme
├── LICENSE
├── VERSION
├── library.properties           ← Arduino library metadata
├── .gitignore
├── src/                         ← Library source code
│   ├── RobotShield.h            ← Main header file
│   ├── reg_map.h                ← I2C register map macro definitions
│   ├── I2cBus.h / .cpp          ← I2C bus singleton class
│   ├── PwmChannel.h / .cpp      ← Single PWM channel class
│   ├── Servo.h / .cpp           ← Servo class
│   ├── Motor.h / .cpp           ← Motor class (dual PWM H-bridge)
│   ├── PowerMonitor.h / .cpp    ← Power telemetry class
│   └── RobotShieldBridge.h/.cpp ← Bridge registration hub
└── examples/
    └── BasicUsage/
        ├── BasicUsage.ino       ← Example sketch
        └── sketch.yaml          ← Arduino configuration file (zephyr platform)
```

## Deployment

This sketch is deployed as part of the UNO Q AI Robot project.

```bash
# Full restart (compile sketch + flash + rebuild containers)
ssh arduino@192.168.100.174 "cd ~/ArduinoApps/{Your Project} && arduino-app-cli app restart ."

# Restart Python container only (when sketch is unchanged)
ssh arduino@192.168.100.174 "docker restart {Your Project}-main-1"
```
