#include <Arduino.h>
#include <ArduinoJson.h>
#include <TemperatureSensor.h>

#include "DHT.h"

#define MAIN_DEBUG 0

#define THERMISTOR_PIN A0
float readTemperature();

#define DHT_PIN D1
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);
void readDHT11(unsigned char results[]);

void setup()
{
  dht.begin();
  pinMode(THERMISTOR_PIN, INPUT);
  Serial.begin(9600);
}

void loop()
{
  unsigned char temperature = readTemperature();
  unsigned char dht_results[3] = {0};

  // Wait a few seconds between measurements.
  delay(2000);

  readDHT11(dht_results);

  StaticJsonDocument<64> doc;
  doc["temp"] = temperature;
  doc["dhtH"] = dht_results[0];
  doc["dhtT"] = dht_results[1];
  doc["dhtR"] = dht_results[2];
  serializeJsonPretty(doc, Serial);

  Serial.println();
  delay(400);
}

void readDHT11(unsigned char results[])
{
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

#if MAIN_DEBUG
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
#endif

  results[0] = h;
  results[1] = t;
  results[2] = hic;
}

float readTemperature()
{
  unsigned short reading = analogRead(THERMISTOR_PIN);
  float temp = TemperatureSensor.getTemperature(reading);
#if MAIN_DEBUG
  Serial.println();
  Serial.print("> ");
  Serial.print(reading);
  Serial.print("\nTemperature ");
  Serial.print(temp);
  Serial.println(" *C");
#endif
  return temp;
}
