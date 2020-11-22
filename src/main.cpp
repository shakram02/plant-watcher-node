#include <Arduino.h>
#include <TemperatureSensor.h>
#include "DHT.h"

#define THERMISTOR_PIN A0
#define DHT_PIN D1
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);

void showDHT11();
void showTemperature();

void setup()
{
  dht.begin();
  pinMode(THERMISTOR_PIN, INPUT);
  Serial.begin(9600);
}

void loop()
{
  showDHT11();
  showTemperature();
}

void showDHT11()
{
  // Wait a few seconds between measurements.
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
}

void showTemperature()
{
  unsigned short reading = analogRead(THERMISTOR_PIN);
  Serial.println();
  Serial.print("> ");
  Serial.print(reading);
  Serial.print("\nTemperature ");
  Serial.print(TemperatureSensor.getTemperature(reading));
  Serial.println(" *C"); // convert absolute
}
