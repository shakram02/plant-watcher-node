#include "UpdateDelayer.h"

bool UpdateDelayer::canUpdate()
{
    unsigned int now = millis();
    bool canRead = (now - this->lastRead) >= this->delay;

    if (canRead)
    {
        this->lastRead = now;
    }

    return canRead;
}