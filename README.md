# RobotShield

Hardware abstraction library for the UNO Q AI Robot shield. Runs on the Arduino UNO Q (STM32U5) board and acts as a **Bridge layer** connecting the Python application to I2C hardware peripherals (co-processor at address `0x20`).

```
Python (main.py)  ←→  Bridge RPC  ←→  RobotShield  ←→  I2cBus  ←→  co-processor @ 0x20
```

## Features

- **Servo Control** — 12 servos, angle range -90° ~ 90°, auto-converted to 500–2500 μs pulses
- **PWM Control** — 12 independent PWM channels with adjustable frequency and pulse width, individually enable/disable
- **Motor Control** — 4 DC motors via dual-PWM H-bridge, supporting forward/reverse/brake, power range -100 ~ 100
- **Power Monitoring** — Real-time battery voltage, charge percentage, status, and IOREF voltage
- **I2C Pass-through** — Register read/write to the co-processor, accessed indirectly by Python via Bridge RPC

## Hardware Resources

| Resource | Count | Description |
|----------|-------|-------------|
| PWM Channels | 12 (0–11) | Default period 20000 μs (50 Hz), pulse range 0–65535 μs |
| Servos | 12 (0–11) | Each servo uses 1 PWM channel |
| DC Motors | 4 (M0–M3) | Each motor uses 2 PWM channels (H-bridge push-pull) |

## Quick Start

### Installation

1. Clone this repository into your Arduino libraries directory:
   ```
   git clone https://github.com/sunfounder/robot-shield-arduino.git
   ```
2. Include it in your sketch with `#include "RobotShield.h"`.

### Basic Usage (Bridge Mode)

```cpp
#include "RobotShield.h"

void setup() {
    Serial.begin(115200);
    I2cBus::i2c().begin();  // Initialize I2C bus
}

void loop() {
    // All functionality is driven by Python via Bridge RPC
    // loop() remains empty
}
```

This library is primarily used as a Bridge layer — after initializing the bus and registering Bridge bindings in `setup()`, all functionality is invoked by Python via Bridge RPC. The `loop()` remains empty.

### Direct Usage (Standalone Mode)

You can also use the C++ classes directly without Bridge:

```cpp
#include "RobotShield.h"

Servo servo(0);
Motor motor("M0", 4, 5);
Power power;

void setup() {
    Serial.begin(115200);
    I2cBus::i2c().begin();
    servo.begin();
    motor.begin();
    power.begin();
}

void loop() {
    servo.setAngle(45);                  // Move servo to 45°
    motor.setPower(80);                  // Motor 80% forward
    Serial.println(power.getVoltage());  // Read battery voltage (mV)
    delay(1000);
}
```

## Bridge API Reference

All functions are registered in `RobotShield::registerAll()`. Parameters are `String` type.

### I2C Pass-through

| Function | Signature | Description |
|----------|-----------|-------------|
| `read_reg` | `(String addr) → int` | Read co-processor register byte (returns 0xFF on NACK) |
| `write_reg` | `(String addr, String value) → int` | Write byte to co-processor register |

### Power Monitoring

| Function | Signature | Description |
|----------|-----------|-------------|
| `get_bat_volt` | `(String) → int` | Battery voltage (mV) |
| `get_bat_percent` | `(String) → int` | Battery charge percentage (0–100) |
| `get_bat_status` | `(String) → int` | 0=Normal, 1=Charging, 2=Full, 3=Low |
| `get_ioref_volt` | `(String) → int` | IOREF voltage (mV) |

### PWM Control

| Function | Signature | Description |
|----------|-----------|-------------|
| `pwm_set_freq` | `(String ch, String hz)` | Set PWM frequency in Hz |
| `pwm_set_pulse` | `(String ch, String us)` | Set pulse width (μs), auto-enables channel |
| `pwm_get_pulse` | `(String ch) → int` | Read current pulse width (μs) |
| `pwm_enable` | `(String ch, String en)` | `"1"` to enable, `"0"` to disable |

### Servo Control

| Function | Signature | Description |
|----------|-----------|-------------|
| `servo_set_angle` | `(String ch, String angle)` | Set angle (-90° ~ 90°) |
| `servo_get_angle` | `(String ch) → int` | Read current angle in degrees |

### Motor Control

| Function | Signature | Description |
|----------|-----------|-------------|
| `motor_set_power` | `(String motor, String power)` | Set power: -100 (full reverse) ~ 100 (full forward), 0 = brake |

### User Button

| Function | Signature | Description |
|----------|-----------|-------------|
| `usr_btn_read` | `(String) → int` | Read USR button state (0x01=pressed, 0x00=released) |

## Examples

| Example | Description |
|---------|-------------|
| [ServoSweep](examples/ServoSweep/ServoSweep.ino) | Sweep servo on channel 0 back and forth between -45° and 45° |
| [MotorTest](examples/MotorTest/MotorTest.ino) | M0 motor: forward 3s → brake 1s → reverse 3s → brake |
| [PowerReadout](examples/PowerReadout/PowerReadout.ino) | Print battery voltage, percentage, status, and IOREF via Serial every 2s |

## Project Structure

```
robot-shield-arduino/
├── README.md                       ← This file
├── LICENSE                         ← License
├── VERSION                         ← Version number
├── library.properties              ← Arduino library metadata
├── src/                            ← Library source
│   ├── RobotShield.h               ← Main header (aggregates all modules)
│   ├── reg_map.h                   ← I2C register map definitions
│   ├── I2cBus.h / .cpp             ← I2C bus singleton
│   ├── Pwm.h / .cpp                ← Single PWM channel
│   ├── Servo.h / .cpp              ← Servo angle control
│   ├── Motor.h / .cpp              ← DC motor H-bridge control
│   ├── Power.h / .cpp              ← Battery/voltage telemetry
│   └── RobotShield.h / .cpp        ← Bridge registration hub
└── examples/                       ← Example sketches
    ├── ServoSweep/
    ├── MotorTest/
    └── PowerReadout/
```

## Architecture

### Class-based Design

Each hardware module is encapsulated in its own class:
- `I2cBus` — I2C bus singleton, wraps Arduino Wire for register read/write
- `Pwm` — Single PWM channel: enable, period, pulse width
- `Servo` — Composes `Pwm`, converts angle ↔ pulse width
- `Motor` — Composes two `Pwm` instances for H-bridge push-pull drive
- `Power` — Reads co-processor power telemetry via `I2cBus`
- `RobotShield` — Holds static instances of all modules, provides static wrapper methods for Bridge RPC

### I2C Pass-through Pattern

Python never talks to the co-processor directly. All I2C communication goes through `read_reg` / `write_reg` via Bridge RPC, keeping I2C bus ownership in the Arduino sketch and avoiding multi-master bus contention.

### Servo: Pulse-before-Enable

`Servo::setAngle()` writes the pulse width BEFORE enabling the PWM channel. This prevents the servo from jerking to a stale pulse value on power-up.

### Motor: Dual-PWM H-Bridge

Each motor uses two PWM channels in push-pull:
- **Forward**: channel A = duty, channel B = 0
- **Reverse**: channel A = 0, channel B = duty
- **Brake**: both 0, both disabled

Motor PWM runs at 100 Hz (10000 μs period), independent from servo 50 Hz.

### Input Value Clamping

All input parameters are clamped — servo angles to ±90°, motor power to ±100, PWM pulse to 0–65535. Invalid input is silently clamped and never causes a crash.

## Dependencies

- Arduino UNO Q (STM32U5) board
- Arduino Bridge library
- I2C co-processor (address `0x20`)

## License

See [LICENSE](LICENSE).
