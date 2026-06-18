#include "Motor.h"

Motor::Motor(const char* id, uint8_t ch_a, uint8_t ch_b)
    : _id(id), _pwm_a(ch_a), _pwm_b(ch_b)
{}

void Motor::begin()
{
    _pwm_a.setFreq(1000000 / PERIOD_US);
    _pwm_b.setFreq(1000000 / PERIOD_US);
}

void Motor::setPower(int16_t power)
{
    if (power < -100) power = -100;
    if (power > 100)  power = 100;

    uint16_t duty;
    if (power > 0) {
        duty = (uint16_t)map(power, 0, 100, 0, PERIOD_US);
        _pwm_a.setPulse(duty);
        _pwm_b.setPulse(0);
        _pwm_a.enable(true);
        _pwm_b.enable(true);
    } else if (power < 0) {
        duty = (uint16_t)map(-power, 0, 100, 0, PERIOD_US);
        _pwm_a.setPulse(0);
        _pwm_b.setPulse(duty);
        _pwm_a.enable(true);
        _pwm_b.enable(true);
    } else {
        _pwm_a.setPulse(0);
        _pwm_b.setPulse(0);
        _pwm_a.enable(false);
        _pwm_b.enable(false);
    }
}
