# CLAUDE.md

This file provides guidance to Claude Code when working with this sketch project.

## Project overview

This is the **Arduino sketch** for the UNO Q AI Robot's "Robot Shield Bridge". It runs on an Arduino UNO Q (STM32U5) board and acts as a **Bridge** between the Python application layer and I2C hardware peripherals (co-processor at address `0x20`).

```
Python (main.py)  ←→  Bridge RPC  ←→  sketch.ino  ←→  I2C  ←→  co-processor @ 0x20
```

The sketch itself is thin — it registers Bridge function bindings and delegates all logic to module `.cpp` files. The `loop()` is empty; all work is driven by Bridge callbacks from Python.

## Architecture

| Module | Files | Purpose |
|---|---|---|
| `sketch.ino` | Main entry | Bridge bindings, `setup()`, empty `loop()` |
| `i2c` | `i2c.cpp`, `i2c.h` | Low-level I2C read/write over Arduino Wire |
| `pwm_control` | `pwm_control.cpp`, `pwm_control.h` | PWM hardware abstraction — enable, period, pulse width |
| `servo_control` | `servo_control.cpp`, `servo_control.h` | Servo control — angle ↔ pulse conversion, sits on PWM layer |
| `motor_control` | `motor_control.cpp`, `motor_control.h` | DC motor control via dual PWM (H-bridge) — 4 motors |
| `led_control` | `led_control.cpp`, `led_control.h` | Built-in LED control (active-low GPIO) |
| `power_monitor` | `power_monitor.cpp`, `power_monitor.h` | Power telemetry — battery, current, voltage from co-processor |
| `reg_map` | `reg_map.h` | Complete I2C register map for the co-processor |

## Bridge API (functions exposed to Python)

All functions are registered in `bridge_binds()` in `sketch.ino`:

### LED
| Function | Signature | Purpose |
|---|---|---|
| `led_set_state` | `(bool state)` | Turn built-in LED on/off (active-low) |

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
| `get_bat_status` | `(String) → int` | 0=Normal, 1=Low, 2=Over, 3=Over Current |
| `get_arduino_current` | `(String) → int` | Arduino current draw (mA) |
| `get_ioref_volt` | `(String) → int` | IOREF voltage (mV) |

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

See [reg_map.h](sketch/reg_map.h) for the full register definitions. Key sections:

| Range | Purpose |
|---|---|
| `0x00–0x0C` | System registers (chip ID, I2C addr, FW version, shutdown, button signals) |
| `0x20–0x23` | Battery/power telemetry (voltage, %, status, current) |
| `0x25–0x2A` | Raw ADC values (16-bit, little-endian) |
| `0x40–0x4B` | PWM channel control (1 byte per channel, enable/disable) |
| `0x50–0x67` | PWM period (16-bit per channel, LE) — default 0x4E20 = 20000μs = 50Hz |
| `0x70–0x87` | PWM pulse width (16-bit per channel, LE) — default 500–2500μs |

## Key design decisions

### I2C pass-through pattern
Python does NOT talk to the co-processor directly. Instead, `read_reg`/`write_reg` act as I2C pass-through over Bridge RPC. This keeps all I2C bus ownership in the Arduino sketch and avoids multi-master bus contention.

### Servo: pulse-before-enable
`_servo_move()` writes the pulse width BEFORE enabling the channel. This prevents the servo from jerking to a stale pulse value on power-up.

### Motor: dual-PWM H-bridge
Each motor uses two PWM channels in push-pull:
- Forward: ch_a = duty, ch_b = 0
- Reverse: ch_a = 0, ch_b = duty
- Brake: both 0, both disabled

Motor PWM runs at 100Hz (10000μs period), distinct from servo 50Hz.

### Bridge calling convention
Bridge functions receive `String` arguments (even when semantically numeric). Each function parses with `.toInt()`. Power functions receive a dummy `String` parameter they ignore — it's a Bridge framework requirement.

## Filesystem

```
sketch/
├── CLAUDE.md              ← this file
├── README.md
├── .claude/
│   └── settings.local.json
└── sketch/
    ├── sketch.ino          ← main entry: setup(), loop(), bridge_binds()
    ├── sketch.yaml         ← Arduino profile: zephyr platform
    ├── reg_map.h           ← I2C register map definitions
    ├── i2c.cpp / .h        ← I2C read/write primitives
    ├── pwm_control.cpp / .h ← PWM abstraction layer
    ├── servo_control.cpp / .h ← Servo angle control
    ├── motor_control.cpp / .h ← DC motor H-bridge control
    ├── led_control.cpp / .h   ← Built-in LED
    └── power_monitor.cpp / .h ← Battery/current telemetry
```

## Rules

### 1. Keep function registrations and implementations in sync
If you add a new Bridge function in a module `.cpp`, you MUST also register it in `bridge_binds()` in [sketch.ino](sketch/sketch.ino). Likewise, if you remove one, clean up the binding.

### 2. Follow the three-layer pattern
New hardware abstractions should follow the existing pattern:
- **`.h`** — public API declarations + `_internal` helpers
- **`.cpp`** — implementation with Doxygen doc comments
- **`bridge_binds()`** — register in `sketch.ino`

### 3. I2C register changes go in reg_map.h only
All register address definitions live in [reg_map.h](sketch/reg_map.h). Use the `REG_*` macros — never hardcode register addresses in module code.

### 4. Bridge API uses String parameters
All Bridge-exposed functions take `String` arguments (Arduino Bridge framework requirement). Parse numeric values with `.toInt()`.

### 5. Clamp input values
Follow the existing pattern: servo angles clamped to ±90°, motor power to ±100, PWM pulse to 0–65535. Bad input should be silently clamped, never crash.
