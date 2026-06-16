/**
 * @file led_control.cpp
 * @brief Built-in LED control — trivial GPIO wrapper.
 *
 * LED_BUILTIN on this board is active-low (LOW = on, HIGH = off).
 */

#include "led_control.h"

/**
 * @brief Initialise the built-in LED pin for output.
 */
void led_init()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

/**
 * @brief Turn the built-in LED on or off.
 * @param state  true = on, false = off.
 */
void led_set_state(bool state)
{
    digitalWrite(LED_BUILTIN, state ? LOW : HIGH);
}
