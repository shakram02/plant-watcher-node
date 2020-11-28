// Source: https://learn.adafruit.com/thermistor/using-a-thermistor
#include "TemperatureSensor.h"
#define CONST_RESISTOR 10000
#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
#define BCOEFFICIENT 3950
#define ANALOG_MAX_08_BIT 255.0
#define ANALOG_MAX_10_BIT 1023.0

float TemperatureSensor::getTemperature(unsigned short pinValue)
{
    float resistance = CONST_RESISTOR / ((ANALOG_MAX_10_BIT / pinValue) - 1);

    float steinhart = resistance / THERMISTORNOMINAL; // (R/Ro)
    steinhart = log(steinhart);                       // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                        // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                      // Invert
    steinhart -= 273.15;

    return steinhart;
}
