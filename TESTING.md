# 示例测试指南

## 原理

UNO Q AI Robot 有两个独立的部分：

```
┌─────────────────────────┐    Bridge RPC    ┌──────────────────┐
│  STM32U5 (sketch.ino)   │ ←─────────────→  │ Python (main.py)  │
│  烧录到单片机             │                  │  Docker 容器中     │
└─────────────────────────┘                   └──────────────────┘
```

- **STM32U5**：跑烧录进去的 sketch（RobotShield Bridge），通电就运行，不依赖 Python
- **Python**：跑在 Docker 容器里，通过 Bridge 给 sketch 发指令（读电池、控制舵机等）

二者可以独立启停。测试 example 时停掉 Python 就不会有人跟你的 example 抢舵机了。

## 测试步骤

以 PowerReadout 为例，三步走：

### 1. 停 Python，换 example

```bash
# 停掉 Python 容器，防止它发 Bridge 指令干扰
ssh arduino@192.168.100.174 "docker stop uno-q-ai-robot-puls-main-1"

# 备份原来的 sketch
ssh arduino@192.168.100.174 "cp ~/ArduinoApps/uno-q-ai-robot-puls/sketch/sketch.ino ~/ArduinoApps/uno-q-ai-robot-puls/sketch/sketch_main.bak"

# 复制 example 替换 sketch.ino
ssh arduino@192.168.100.174 "cp ~/ArduinoApps/uno-q-ai-robot-puls/sketch/libraries/RobotShield/examples/PowerReadout/PowerReadout.ino ~/ArduinoApps/uno-q-ai-robot-puls/sketch/sketch.ino"
```

### 2. 编译烧录，看串口

```bash
# 编译并烧录到 STM32U5（不会重建 Python 容器因为已经停了）
ssh arduino@192.168.100.174 "cd ~/ArduinoApps/uno-q-ai-robot-puls && arduino-app-cli app restart ."

# 打开串口监视器看输出
ssh arduino@192.168.100.174 "arduino-app-cli monitor"
```

预期输出（每 2 秒一行）：
```
Battery: 7900 mV  80%  status=0  current=120 mA  IOREF=3300 mV
Battery: 7900 mV  80%  status=0  current=120 mA  IOREF=3300 mV
```

按 `Ctrl+C` 退出监视器。

### 3. 恢复原样

```bash
# 恢复原来的 sketch.ino
ssh arduino@192.168.100.174 "cp ~/ArduinoApps/uno-q-ai-robot-puls/sketch/sketch_main.bak ~/ArduinoApps/uno-q-ai-robot-puls/sketch/sketch.ino"

# 重新编译烧录
ssh arduino@192.168.100.174 "cd ~/ArduinoApps/uno-q-ai-robot-puls && arduino-app-cli app restart ."

# 启动 Python 容器
ssh arduino@192.168.100.174 "docker start uno-q-ai-robot-puls-main-1"
```

## 4 个示例一览

| 示例 | 做什么 | 注意事项 |
|---|---|---|
| BasicUsage | 最小初始化，验证库加载正常 | 无 |
| PowerReadout | 串口打印电池电压/电量/电流 | **最安全**，只读不写 |
| ServoSweep | 舵机 0 在 -45°~+45° 来回扫 | 舵机会动，注意周围空间 |
| MotorTest | M0 正转/刹车/反转循环 | 电机会转，机器人可能移动 |

每次测试一个 example，测完恢复即可。
