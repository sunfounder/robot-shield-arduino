# CLAUDE.md

This file provides guidance to Claude Code when working with this sketch project.

## Project overview

This is the **Arduino sketch** for the UNO Q AI Robot's "Robot Shield Bridge". It runs on an Arduino UNO Q (STM32U5) board and acts as a **Bridge** between the Python application layer and I2C hardware peripherals (co-processor at address `0x20`).

```
Python (main.py)  ←→  Bridge RPC  ←→  RobotShield  ←→  I2cBus  ←→  co-processor @ 0x20
```

The entry sketch is minimal — it initialises `I2cBus`, `RobotShield`, and registers all Bridge bindings. The `loop()` is empty; all work is driven by Bridge callbacks from Python.

## Architecture (class-based)

| Class | Files | Purpose |
|---|---|---|
| `I2cBus` | `src/I2cBus.cpp`, `src/I2cBus.h` | I2C bus singleton — register read/write over Arduino Wire |
| `Pwm` | `src/Pwm.cpp`, `src/Pwm.h` | Single PWM channel — enable, period, pulse width |
| `Servo` | `src/Servo.cpp`, `src/Servo.h` | Servo angle control — angle↔pulse conversion, composes `Pwm` |
| `Motor` | `src/Motor.cpp`, `src/Motor.h` | DC motor via dual PWM (H-bridge) — composes 2 `Pwm` |
| `Power` | `src/Power.cpp`, `src/Power.h` | Power telemetry — reads battery/voltage from co-processor via `I2cBus` |
| `RobotShield` | `src/RobotShield.cpp`, `src/RobotShield.h` | Bridge registration hub — static instances of all modules, static wrapper methods for Bridge RPC |
| `reg_map` | `src/reg_map.h` | Complete I2C register map for the co-processor |

## Bridge API (functions exposed to Python)

All functions are registered in `RobotShield::registerAll()`:

### I2C pass-through
| Function | Signature | Purpose |
|---|---|---|
| `read_reg` | `(String addr) → int` | Read co-processor register byte (returns 0xFF on NACK) |
| `write_reg` | `(int addr, int value) → int` | Write byte to co-processor register |

### Power telemetry
| Function | Signature | Purpose |
|---|---|---|
| `get_bat_volt` | `(String) → int` | Battery voltage (mV) |
| `get_bat_percent` | `(String) → int` | Battery charge percentage (0–100) |
| `get_bat_status` | `(String) → int` | 0=Normal, 1=Charging, 2=Full, 3=Low |
| `get_ioref_volt` | `(String) → int` | IOREF voltage (mV) |

### User button
| Function | Signature | Purpose |
|---|---|---|
| `usr_btn_read` | `(String) → int` | Read USR button state: 0x01=pressed, 0x00=released |

### PWM control (12 channels: 0–11)
| Function | Signature | Purpose |
|---|---|---|
| `pwm_set_freq` | `(String ch, String hz)` | Set PWM frequency in Hz |
| `pwm_set_pulse` | `(String ch, String us)` | Set pulse width (μs), auto-enables |
| `pwm_get_pulse` | `(String ch) → int` | Read current pulse width (μs) |
| `pwm_enable` | `(String ch, String en)` | Enable ("1") or disable ("0") |

### Servo control
| Function | Signature | Purpose |
|---|---|---|
| `servo_set_angle` | `(String ch, String angle)` | Set angle (-90°–90°), converts to 500–2500μs |
| `servo_get_angle` | `(String ch) → int` | Read current angle in degrees |

### Motor control
| Function | Signature | Purpose |
|---|---|---|
| `motor_set_power` | `(String motor, String power)` | Set power: -100 (full rev) to 100 (full fwd), 0=brake |

## I2C register map

See [src/reg_map.h](src/reg_map.h) for the full register definitions. Key sections:

| Range | Purpose |
|---|---|
| `0x00–0x0C` | System registers (chip ID, I2C addr, FW version, shutdown, button signals) |
| `0x20–0x22` | Battery/power telemetry (voltage, %, status) |
| `0x40–0x4B` | PWM channel control (1 byte per channel, enable/disable) |
| `0x50–0x67` | PWM period (16-bit per channel, LE) — default 0x4E20 = 20000μs = 50Hz |
| `0x70–0x87` | PWM pulse width (16-bit per channel, LE) — default 500–2500μs |

## Key design decisions

### Class-based architecture
Each hardware module is encapsulated in its own class. `I2cBus` is a singleton. `Servo` composes `Pwm`, `Motor` composes two `Pwm` instances. `RobotShield` owns static arrays of all instances (12 PWM, 12 Servo, 4 Motor) and provides static wrapper methods for Bridge registration.

### I2C pass-through pattern
Python does NOT talk to the co-processor directly. Instead, `read_reg`/`write_reg` act as I2C pass-through over Bridge RPC. This keeps all I2C bus ownership in the Arduino sketch and avoids multi-master bus contention.

### Servo: pulse-before-enable
`Servo::setAngle()` writes the pulse width BEFORE enabling the channel. This prevents the servo from jerking to a stale pulse value on power-up.

### Motor: dual-PWM H-bridge
Each motor uses two PWM channels in push-pull:
- Forward: ch_a = duty, ch_b = 0
- Reverse: ch_a = 0, ch_b = duty
- Brake: both 0, both disabled

Motor PWM runs at 100Hz (10000μs period), distinct from servo 50Hz.

### Bridge calling convention
Bridge functions receive `String` arguments (even when semantically numeric). Each static wrapper parses with `.toInt()`. Power functions receive a dummy `String` parameter they ignore — it's a Bridge framework requirement.

## Filesystem

```
robot_shield_bridge/
├── CLAUDE.md                     ← this file
├── README.md
├── LICENSE
├── VERSION
├── library.properties            ← Arduino library metadata
├── .gitignore
├── .claude/
│   └── settings.local.json
├── src/                          ← library source (single source of truth)
│   ├── RobotShield.h             ← main header (aggregates all modules)
│   ├── reg_map.h                 ← I2C register map definitions
│   ├── I2cBus.h / .cpp           ← I2C bus singleton
│   ├── Pwm.h / .cpp              ← single PWM channel
│   ├── Servo.h / .cpp            ← servo angle control
│   ├── Motor.h / .cpp            ← DC motor H-bridge control
│   ├── Power.h / .cpp            ← battery/voltage telemetry
│   └── RobotShield.h / .cpp      ← Bridge registration hub
└── examples/
    └── BasicUsage/
        ├── BasicUsage.ino        ← example sketch entry point
        └── sketch.yaml           ← Arduino profile: zephyr platform
```

## Rules

### 1. Bridge function names are a contract
The 14 Bridge function names registered in `RobotShield::registerAll()` must match the Python side's `Bridge.call()` strings exactly. Do NOT rename them without coordinating both sides.

### 2. Class encapsulation
Logic lives in classes, not loose functions. New hardware abstractions should follow the existing pattern:
- **Header** — class declaration + constants
- **Implementation** — method definitions
- **Bridge binding** — static wrapper in `RobotShield`

### 3. I2C register changes go in reg_map.h only
All register address definitions live in [src/reg_map.h](src/reg_map.h). Use the `REG_*` macros — never hardcode register addresses in class code.

### 4. Bridge API uses String parameters
All Bridge-exposed static methods take `String` arguments (Arduino Bridge framework requirement). Parse numeric values with `.toInt()`.

### 5. Clamp input values
Follow the existing pattern: servo angles clamped to ±90°, motor power to ±100, PWM pulse to 0–65535. Bad input should be silently clamped, never crash.
