#include "Timing.h"
#define UTC_OFFSET 2
#define NTP_SYNC_INTERVAL 60000
const char *NTP_SERVER = "europe.pool.ntp.org";

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset()). Additionally you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
// GMT+2 offset

unsigned long Timing::getEpochTime()
{
    return this->timeClient->getEpochTime();
}

Timing::Timing(WiFiUDP &ntpUdp)
{
    udp = &ntpUdp;
}

void Timing::init()
{
    this->timeClient = new NTPClient(*udp, NTP_SERVER, UTC_OFFSET * 3600, NTP_SYNC_INTERVAL);
    this->timeClient->forceUpdate(); // Don't reinitialize UDP.
}

Timing::~Timing()
{
    delete this->timeClient;
}