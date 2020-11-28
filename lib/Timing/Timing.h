#pragma once
#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

class Timing
{
private:
    NTPClient *timeClient;
    WiFiUDP *ntpUDP;

public:
    Timing();
    Timing(const Timing &) = delete;
    Timing &operator=(const Timing &) = delete;
    ~Timing();

    void setup();
    unsigned long getEpochTime();
};
