#ifndef PWM_CHANNEL_H
#define PWM_CHANNEL_H

#include <Arduino.h>

class PwmChannel {
public:
    explicit PwmChannel(uint8_t ch);

    void begin();
    void setFreq(uint16_t hz);
    void setPulse(uint16_t us);
    uint16_t getPulse();
    uint16_t getPeriod();
    void enable(bool on);

    uint8_t channel() const { return _ch; }

private:
    uint8_t _ch;
};

#endif // PWM_CHANNEL_H
