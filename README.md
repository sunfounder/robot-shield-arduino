# Robot Shield Bridge

这个项目是 UNO Q AI Robot 的 Arduino 固件（sketch），运行在 Arduino UNO Q（STM32U5）开发板上。它为 Robot Shield 提供 **Bridge（桥接）** 功能，作为 Python 应用层与 I2C 硬件外设之间的通信桥梁。

## 架构概述

```
Python (main.py)  ←→  Bridge RPC  ←→  sketch.ino  ←→  I2C  ←→  robot shield (i2c address:0x20)
```

- **Python 端**：通过 Arduino Bridge RPC 调用本 sketch 暴露的函数
- **sketch.ino**：注册 Bridge 函数绑定，入口逻辑极简，`loop()` 为空
- **I2C 协处理器**：地址 `0x20`，负责 PWM 生成（PCA9685）、ADC 采样（电池/电流/电压）等底层硬件操作

## 主要功能

### 1. I2C 寄存器透传

`read_reg` / `write_reg` 将 I2C 总线操作封装为 Bridge 函数，Python 通过这两个函数间接读写协处理器的全部寄存器，避免了 I2C 多主竞争问题。

### 2. PWM 控制（12 通道）

支持 12 路 PWM 通道（0–11），提供频率、脉宽、使能的完整控制。默认周期 20000μs（50Hz），适用于标准舵机。

- `pwm_set_freq` — 设置 PWM 频率
- `pwm_set_pulse` — 设置脉宽（微秒），自动使能通道
- `pwm_get_pulse` — 读取当前脉宽
- `pwm_enable` — 使能/关闭通道

### 3. 舵机控制

在 PWM 层之上封装了角度控制。角度范围 **-90° ~ +90°**，自动映射到脉宽 **500 ~ 2500μs**（线性映射）。

关键设计：写入脉宽后再使能通道，防止舵机上电时因读到旧脉宽值而猛跳。

- `servo_set_angle` — 设置舵机角度
- `servo_get_angle` — 读取当前角度
- `SERVO_ALL_CENTER` (0x40) — 写入 `REG_SYS_CTRL` 可使全部舵机回中

### 4. 直流电机控制（H 桥）

4 路直流电机（M0–M3），每路使用两个 PWM 通道组成 H 桥驱动：

| 状态 | 通道 A | 通道 B |
|------|--------|--------|
| 正转 | duty | 0 |
| 反转 | 0 | duty |
| 刹车 | 0 | 0（关闭） |

电机 PWM 频率固定为 **100Hz**（10000μs 周期），与舵机的 50Hz 独立。

通道映射：
- M0 → PWM4 + PWM5
- M1 → PWM6 + PWM7
- M2 → PWM8 + PWM9
- M3 → PWM10 + PWM11

### 5. 电源监控

从协处理器寄存器读取电池状态：

- `get_bat_volt` — 电池电压（mV）
- `get_bat_percent` — 电池电量百分比（0–100）
- `get_bat_status` — 电池状态（0=正常, 1=低压, 2=过压, 3=过流）
- `get_arduino_current` — Arduino 板电流（mA）
- `get_ioref_volt` — IOREF 电压（mV）

### 6. LED 控制

控制板载 LED（低电平有效）。

### 7. 系统控制

robot shield 系统寄存器支持：
- 电源开关控制
- **PWR 按键信号** — 单击 / 双击进入零位 / 双击退出零位
- **USR 按键信号** — PTT（Push-to-Talk）长按开始录音 / 松开停止录音

## I2C 寄存器映射

参见 [reg_map.h](sketch/reg_map.h) 获取完整定义。

| 地址范围 | 用途 |
|----------|------|
| `0x00–0x0C` | 系统寄存器（芯片ID、I2C地址、固件版本、关机、按键信号） |
| `0x20–0x23` | 电池/电源遥测（电压、电量、状态、电流） |
| `0x25–0x2A` | 原始 ADC 值（16 位，小端序） |
| `0x40–0x4B` | PWM 通道控制（每通道 1 字节，使能/关闭） |
| `0x50–0x67` | PWM 周期（每通道 2 字节 16 位，小端序）— 默认 20000μs / 50Hz |
| `0x70–0x87` | PWM 脉宽（每通道 2 字节 16 位，小端序）— 默认 500–2500μs |

## 项目文件结构

```
sketch/
├── CLAUDE.md              ← Claude Code 开发指引
├── README.md              ← readme
├── .claude/
│   └── settings.local.json
└── sketch/
    ├── sketch.ino          ← 主入口：setup()、loop()、bridge_binds()
    ├── sketch.yaml         ← Arduino 配置文件（zephyr 平台）
    ├── reg_map.h           ← I2C 寄存器映射宏定义
    ├── i2c.cpp / .h        ← I2C 底层读写
    ├── pwm_control.cpp / .h ← PWM 抽象层
    ├── servo_control.cpp / .h ← 舵机角度控制
    ├── motor_control.cpp / .h ← 直流电机 H 桥控制
    ├── led_control.cpp / .h   ← 板载 LED
    └── power_monitor.cpp / .h ← 电池/电流遥测
```

## 部署方式

本 sketch 作为 UNO Q AI Robot 整体项目的一部分部署。sketch 源文件位于 `~/ArduinoApps/{Your Project}/sketch/`，由 `arduino-app-cli app restart` 命令自动编译并烧录到 STM32U5。

```bash
# 完整重启（编译 sketch + 烧录 + 重建容器）
ssh arduino@192.168.100.174 "cd ~/ArduinoApps/{Your Project} && arduino-app-cli app restart ."

# 仅重启 Python 容器（sketch 未改动时）
ssh arduino@192.168.100.174 "docker restart {Your Project}-main-1"
```
