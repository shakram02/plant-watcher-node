#pragma once
#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

class Timing
{
private:
    NTPClient *timeClient;
    WiFiUDP *udp;

public:
    Timing(WiFiUDP &udpSocket);
    Timing(const Timing &) = delete;
    Timing &operator=(const Timing &) = delete;
    ~Timing();

    void init();
    unsigned long getEpochTime();
};
