#pragma once
#include <Arduino.h>

/**
 * https://learn.adafruit.com/thermistor/using-a-thermistor
 */
class NodeServer
{
private:
    NodeServer() = default;

public:
    NodeServer(const NodeServer &) = delete;
    NodeServer &operator=(const NodeServer &) = delete;

    static float getTemperature(unsigned short pinValue);
};
