#ifndef POWER_MONITOR_H
#define POWER_MONITOR_H

#include <Arduino.h>

class PowerMonitor {
public:
    void begin();

    uint8_t batVolt();
    uint8_t batPercent();
    uint8_t batStatus();
    uint8_t arduinoCurrent();
    uint8_t iorefVolt();
};

#endif // POWER_MONITOR_H
