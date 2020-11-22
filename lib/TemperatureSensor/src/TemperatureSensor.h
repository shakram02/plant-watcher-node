#pragma once
#include <Arduino.h>

/**
 * https://learn.adafruit.com/thermistor/using-a-thermistor
 */
class TemperatureSensor
{
private:
    TemperatureSensor() = default;

public:
    // static TemperatureSensor &getInstance();
    TemperatureSensor(const TemperatureSensor &) = delete;
    TemperatureSensor &operator=(const TemperatureSensor &) = delete;

    static float getTemperature(unsigned short pinValue);
};

extern TemperatureSensor TemperatureSensor;