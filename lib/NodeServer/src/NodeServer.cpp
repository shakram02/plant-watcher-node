#include "NodeServer.h"

bool NodeServer::isConnected()
{
    return client.connected();
}

void NodeServer::stop()
{
    client.stop();
}

void NodeServer::sendLine(const String &content)
{
    client.println(content);
}

void NodeServer::setTimeout(unsigned long timeout)
{
    client.setTimeout(timeout);
}

bool NodeServer::connectServer(const char *ip, int port)
{
    bool status = client.connect(ip, port);
    client.setTimeout(1000);

    if (status)
    {
        return fetchUuid();
    }
    return status;
}

String &NodeServer::getUuid()
{
    return deviceUid;
}

bool NodeServer::fetchUuid()
{
    client.println(secret);
    // Wait for device UUID
    deviceUid = String(client.readString());
    return deviceUid.length() != 0;
}
