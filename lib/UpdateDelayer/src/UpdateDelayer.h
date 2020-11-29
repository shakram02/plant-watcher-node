#pragma once
#include <Arduino.h>

/**
 * https://learn.adafruit.com/thermistor/using-a-thermistor
 */
class UpdateDelayer
{
private:
    unsigned int delay = 0;
    unsigned int lastRead = 0;
public:
    UpdateDelayer(unsigned int readingDelay) : delay(readingDelay) {} // static TemperatureSensor &getInstance();
    bool canUpdate();
};