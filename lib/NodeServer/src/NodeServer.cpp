#include "NodeServer.h"
#include "string.h"
#define HEADER_SIZE 1
// Space for the null character
#define FOOTER_SIZE 1
#define RECV_BUFF_SIZE 32

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

    char buffer[fullLen] = {0};
    buffer[0] = msgType; // TODO: add length byte
    memcpy(buffer + 1, content, fullLen - 1);

    return client->beginPacket(ip, port) &&
           client->write(buffer, fullLen) &&
           client->endPacket();
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

    while (!client->parsePacket())
    {
        delay(10);
    }

    memset(deviceUid, 0, DEVICE_UID_LENGTH);
    client->read(deviceUid, DEVICE_UID_LENGTH);
    return true;
}

const char *NodeServer::getUuid()
{
    return deviceUid;
}
