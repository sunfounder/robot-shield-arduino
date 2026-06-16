#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>

// initialize built-in LED pin -> return: none
void led_init();

// state: true=on, false=off (active-low) -> return: none
void led_set_state(bool state);

#endif
