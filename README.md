# Robot Shield Bridge

这个项目是 UNO Q AI Robot 的 Arduino 固件（sketch），运行在 Arduino UNO Q（STM32U5）开发板上。它为 Robot Shield 提供 **Bridge（桥接）** 功能，作为 Python 应用层与 I2C 硬件外设之间的通信桥梁。

## 架构概述

```
Python (main.py)  ←→  Bridge RPC  ←→  RobotShieldBridge  ←→  I2cBus  ←→  robot shield (i2c address:0x20)
```

- **Python 端**：通过 Arduino Bridge RPC 调用本库暴露的函数
- **RobotShieldBridge**：静态类，负责注册所有 Bridge 函数绑定，将调用委托到对应实例
- **I2cBus**：I2C 总线单例，封装 Wire 操作
- **I2C robot shield**：地址 `0x20`，负责 PWM 生成（PCA9685）、ADC 采样（电池/电流/电压）等底层硬件操作

## 类设计

| 类 | 文件 | 说明 |
|---|---|---|
| `I2cBus` | `I2cBus.h/cpp` | I2C 总线单例，封装寄存器读写 |
| `PwmChannel` | `PwmChannel.h/cpp` | 单路 PWM 通道（频率、脉宽、使能） |
| `Servo` | `Servo.h/cpp` | 舵机角度控制（组合 PwmChannel），-90°~90° ↔ 500~2500μs |
| `Motor` | `Motor.h/cpp` | DC 电机 H 桥控制（组合 2 个 PwmChannel），-100~100 功率 |
| `PowerMonitor` | `PowerMonitor.h/cpp` | 电源遥测（电池电压/电量/状态/电流/IOREF） |
| `RobotShieldBridge` | `RobotShieldBridge.h/cpp` | Bridge 注册中心，维护静态实例数组 |

## 主要功能

### 1. I2C 寄存器透传

`read_reg` / `write_reg` 将 I2C 总线操作封装为 Bridge 函数，Python 通过这两个函数间接读写 robot shield 的全部寄存器，避免了 I2C 多主竞争问题。

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

### 4. 直流电机控制（H 桥）

4 路直流电机（M0–M3），每路使用两个 PWM 通道组成 H 桥驱动：

| 状态 | 通道 A | 通道 B |
|------|--------|--------|
| 正转 | duty | 0 |
| 反转 | 0 | duty |
| 刹车 | 0 | 0（关闭） |

电机 PWM 频率固定为 **100Hz**（10000μs 周期），与舵机的 50Hz 独立。

通道映射：M0→PWM4/5, M1→PWM6/7, M2→PWM8/9, M3→PWM10/11

### 5. 电源监控

从 robot shield 寄存器读取电池状态：

- `get_bat_volt` — 电池电压（mV）
- `get_bat_percent` — 电池电量百分比（0–100）
- `get_bat_status` — 电池状态（0=正常, 1=低压, 2=过压, 3=过流）
- `get_arduino_current` — Arduino 板电流（mA）
- `get_ioref_volt` — IOREF 电压（mV）

### 6. 系统控制

robot shield 系统寄存器支持：
- 电源开关控制
- **PWR 按键信号** — 单击 / 双击进入零位 / 双击退出零位
- **USR 按键信号** — PTT（Push-to-Talk）长按开始录音 / 松开停止录音

## I2C 寄存器映射

参见 [reg_map.h](src/reg_map.h) 获取完整定义。

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
robot_shield_bridge/
├── CLAUDE.md                    ← Claude Code 开发指引
├── README.md                    ← readme
├── LICENSE
├── VERSION
├── library.properties           ← Arduino 库元数据
├── .gitignore
├── src/                         ← 库源码
│   ├── RobotShield.h            ← 主头文件
│   ├── reg_map.h                ← I2C 寄存器映射宏定义
│   ├── I2cBus.h / .cpp          ← I2C 总线单例类
│   ├── PwmChannel.h / .cpp      ← 单路 PWM 类
│   ├── Servo.h / .cpp           ← 舵机类
│   ├── Motor.h / .cpp           ← 电机类（双 PWM H 桥）
│   ├── PowerMonitor.h / .cpp    ← 电源遥测类
│   └── RobotShieldBridge.h/.cpp ← Bridge 注册中心
└── examples/
    └── BasicUsage/
        ├── BasicUsage.ino       ← 示例 sketch
        └── sketch.yaml          ← Arduino 配置文件（zephyr 平台）
```

## 部署方式

本 sketch 作为 UNO Q AI Robot 整体项目的一部分部署。

```bash
# 完整重启（编译 sketch + 烧录 + 重建容器）
ssh arduino@192.168.100.174 "cd ~/ArduinoApps/{Your Project} && arduino-app-cli app restart ."

# 仅重启 Python 容器（sketch 未改动时）
ssh arduino@192.168.100.174 "docker restart {Your Project}-main-1"
```
