#ifndef SERVO_H
#define SERVO_H

#include <Arduino.h>
#include "PwmChannel.h"

class Servo {
public:
    static constexpr int16_t MIN_PULSE = 500;
    static constexpr int16_t MID_PULSE = 1500;
    static constexpr int16_t MAX_PULSE = 2500;
    static constexpr int16_t ANGLE_MIN = -90;
    static constexpr int16_t ANGLE_MAX = 90;
    static constexpr uint16_t PERIOD_US = 20000;

    explicit Servo(uint8_t ch);

    void begin();
    void setAngle(int16_t angle);
    int16_t getAngle();

private:
    PwmChannel _pwm;

    static int16_t angleToPulse(int16_t angle);
    static int16_t pulseToAngle(uint16_t pulse);
};

#endif // SERVO_H
