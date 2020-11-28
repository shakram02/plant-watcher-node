#include <Arduino.h>
#include <ArduinoJson.h>
#include <TemperatureSensor.h>
#include <UpdateDelayer.h>
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <RTCZero.h>
#include <NTPClient.h>
#include "DHT.h"

RTCZero rtc;

const char *ssid = "TP-LINK_D0510E";
const char *password = "73730941";
#define MAIN_DEBUG 0
WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

#define THERMISTOR_PIN A1
#define ANALOG_RES 10
#define WIFI_CONNECTED_PIN 1
float readTemperature();

#define DHT_PIN 0
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);
UpdateDelayer dhtDelayer(2000);
UpdateDelayer tempDelayer(500);

void readDHT11(unsigned char results[]);
void printWiFiStatus();
void connectWiFi();

void setup()
{
  pinMode(THERMISTOR_PIN, INPUT);
  pinMode(WIFI_CONNECTED_PIN, OUTPUT);
  analogReadResolution(ANALOG_RES);

  connectWiFi();
  timeClient.update();
  dht.begin();
  Serial.begin(9600);
}

StaticJsonDocument<128> doc; // Store the last readings
void loop()
{
  bool updated = false;
  if (dhtDelayer.canUpdate())
  {
    updated = true;
    unsigned char dht_results[3] = {0};
    readDHT11(dht_results);
    doc["dhtH"] = dht_results[0];
    doc["dhtT"] = dht_results[1];
    doc["dhtR"] = dht_results[2];
  }

  if (tempDelayer.canUpdate())
  {
    updated = true;
    unsigned char temperature = readTemperature();
    doc["temp"] = temperature;
  }

  if (updated)
  {
    doc["epoch"] = timeClient.getEpochTime();
    serializeJsonPretty(doc, Serial);
    Serial.println();
  }

  delay(100);
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
  Serial.print("TEMP> ");
  Serial.print(reading);
  Serial.print("\nTemperature ");
  Serial.print(temp);
  Serial.println(" *C");
#endif
  return temp;
}

void printWiFiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void connectWiFi()
{
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
#if MAIN_DEBUG
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
#endif
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, password);
    delay(3000);
  }
  digitalWrite(WIFI_CONNECTED_PIN, OUTPUT);
#if MAIN_DEBUG
  printWiFiStatus();
#endif
}