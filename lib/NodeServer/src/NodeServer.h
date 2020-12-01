#pragma once
#include <Arduino.h>
#include <WiFi101.h>

/**
 * https://learn.adafruit.com/thermistor/using-a-thermistor
 */
class NodeServer
{
private:
    WiFiClient client;
    String secret;
    String deviceUid = "";
    bool fetchUuid();

public:
    /*!
     *  @brief Constructs a node server using device secret
     */
    NodeServer(const String nodeSecret) : secret(nodeSecret){};
    NodeServer(const NodeServer &) = delete;
    NodeServer &operator=(const NodeServer &) = delete;

    bool connectServer(const char *ip, int port);
    bool isConnected();
    void stop();
    void sendLine(const String &content);
    void setTimeout(unsigned long timeout);
    String &getUuid();
};
