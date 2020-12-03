#include "NodeServer.h"
#include "string.h"
#define HEADER_SIZE 3
// Space for the null character
#define FOOTER_SIZE 1
#define RECV_BUFF_SIZE 32
#define UDP_MTU 1400
#define MAX_HANDSHAKE_RESPONSE_WAIT_MILLIS 2000

NodeServer::NodeServer(WiFiUDP &udpSocket, const char *nodeSecret, const char *serverIp, uint16_t serverPort, uint16_t clientPort)
{
    ip = serverIp;
    secret = nodeSecret;
    port = serverPort;
    client = &udpSocket;
}

bool NodeServer::send(const char *content, MsgType msgType)
{
    int msgLen = strlen(content);
    int fullLen = msgLen + HEADER_SIZE;
    if (fullLen > UDP_MTU)
    {
        return false;
    }

    char buffer[fullLen] = {0};

    fillHeader(buffer, msgType, msgLen);
    memcpy(buffer + HEADER_SIZE, content, msgLen);

    return client->beginPacket(ip, port) &&
           client->write(buffer, fullLen) &&
           client->endPacket();
}

void NodeServer::fillHeader(char *const buffer, MsgType msgType, const uint16_t msgLen)
{
    buffer[0] = msgType;
    // Little endian: smallest address, smallest value
    buffer[1] = (msgLen & 0xFF);
    buffer[2] = (msgLen >> 8) & 0xFF;
}

bool NodeServer::sendData(const char *content)
{
    return send(content, MsgType::Data);
}

size_t NodeServer::receive(char *const buffer, const size_t bufferSize)
{
    return client->read(buffer, bufferSize);
}

int NodeServer::available()
{
    return client->parsePacket();
}

bool NodeServer::initConnection()
{
    if (!send(secret, MsgType::Hello))
    {
        return false;
    }

    size_t waited = 0;
    while (!client->parsePacket())
    {
        delay(10);
        waited += 10;

        if (waited > MAX_HANDSHAKE_RESPONSE_WAIT_MILLIS)
        {
            return false;
        }
    }

    memset(deviceUid, 0, DEVICE_UID_LENGTH);
    client->read(deviceUid, DEVICE_UID_LENGTH);
    return true;
}

const char *NodeServer::getUuid()
{
    return deviceUid;
}
