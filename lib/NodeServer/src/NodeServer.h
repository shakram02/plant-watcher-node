#pragma once
#include <WiFi101.h>
#include <WiFiUdp.h>
#define DEVICE_UID_LENGTH 37
/**
 * https://learn.adafruit.com/thermistor/using-a-thermistor
 */
class NodeServer
{
private:
    enum MsgType
    {
        Hello = 0x1,
        Data = 0x2,
        Command = 0x3,
    };

    WiFiUDP *client;
    const char *secret;
    char deviceUid[DEVICE_UID_LENGTH] = {0};
    uint16_t port;
    uint16_t localPort;
    const char *ip;
    bool send(const char *content, MsgType msgType);
    void fillHeader(char *const buffer, MsgType msgType, const uint16_t msgLen);

public:
    /*!
     *  @brief Constructs a node server using device secret
     */
    NodeServer(WiFiUDP &udpSocket, const char *nodeSecret, const char *serverIp, uint16_t serverPort, uint16_t clientPort);
    NodeServer(const NodeServer &) = delete;
    NodeServer &operator=(const NodeServer &) = delete;

    bool initConnection();
    bool sendData(const char *content);
    size_t receive(char *const buffer, const size_t bufferSize);
    int available();
    const char *getUuid();
};
