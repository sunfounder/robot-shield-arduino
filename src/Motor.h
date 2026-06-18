#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "PwmChannel.h"

class Motor {
public:
    static constexpr uint16_t PERIOD_US = 10000;  // 100 Hz

    Motor(const char* id, uint8_t ch_a, uint8_t ch_b);

    void begin();
    void setPower(int16_t power);

    const char* id() const { return _id; }

private:
    const char* _id;
    PwmChannel _pwm_a;
    PwmChannel _pwm_b;
};

#endif // MOTOR_H
