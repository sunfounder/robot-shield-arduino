#ifndef POWER_MONITOR_H
#define POWER_MONITOR_H

#include <Arduino.h>

// dummy: unused (Bridge convention) -> return: battery voltage (mV)
int get_bat_volt(String dummy);

// dummy: unused (Bridge convention) -> return: battery percentage (0–100)
int get_bat_percent(String dummy);

// dummy: unused (Bridge convention) -> return: 0=Normal, 1=Low Voltage, 2=Over Voltage, 3=Over Current
int get_bat_status(String dummy);

// dummy: unused (Bridge convention) -> return: Arduino current draw (mA)
int get_arduino_current(String dummy);

// dummy: unused (Bridge convention) -> return: IOREF voltage (mV)
int get_ioref_volt(String dummy);

#endif
