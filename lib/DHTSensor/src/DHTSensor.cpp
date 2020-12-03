#include "DHTSensor.h"

void readDHT11(DHT &dht, unsigned char results[3])
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t))
    {
        results[0] = 0xFF;
        results[1] = 0xFF;
        results[2] = 0xFF;
        return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
    results[0] = h;
    results[1] = t;
    results[2] = hic;
}