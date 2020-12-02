#define MAIN_DEBUG 0
#define TEMP_DEBUG 0

#include <Arduino.h>
#include <ArduinoJson.h>
#include <TemperatureSensor.h>
#include <UpdateDelayer.h>
#include <SPI.h>
#include <WiFiConnection.h>
#include <NodeServer.h>
#include <Timing.h>
#include "DHT.h"
#include "secrets.h"
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

const char *const ssid = WIFI_SSID;
const char *const password = WIFI_SECRET;

const char *const serverIp = NODE_SERVER;
const uint16_t serverPort = NODE_SERVER_PORT;
const uint16_t localPort = 4999;
const char *const secret = NODE_SECRET;

#define THERMISTOR_PIN A1
#define ANALOG_RES 10
#define WIFI_CONNECTED_PIN 1
#define SERVER_CONNECTED_PIN LED_BUILTIN

#define DHT_PIN 0
#define DHT_TYPE DHT11
#define DHT_INTERVAL 2000

#define DEADSTATE_RETRY_MILLIS 10000

float readTemperature();
void setupPins();
void readDHT11(unsigned char results[]);
void initWiFiConnection();

DHT dht(DHT_PIN, DHT_TYPE);
UpdateDelayer dhtDelayer(DHT_INTERVAL);
UpdateDelayer tempDelayer(500);

void initServerConnection();
void serverDeadState(unsigned char indicatorLedPin);

WiFiUDP udp;
Timing time(udp);
NodeServer nodeServer(udp, secret, serverIp, serverPort, localPort);

void setup()
{
#if MAIN_DEBUG
  Serial.begin(9600);
#endif

  delay(2000);
  setupPins();

  initWiFiConnection();

  udp.begin(localPort);
  time.init();
  initServerConnection();

  dht.begin();
}

void loop()
{
  StaticJsonDocument<256> doc;
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
    doc["epoch"] = time.getEpochTime();
    doc["uuid"] = nodeServer.getUuid();

    String json = "";
    serializeJson(doc, json);
    if (!nodeServer.sendData(json.c_str()))
    {
      initServerConnection();
    }

#if MAIN_DEBUG
    Serial.println(json);
#endif
  }
  delay(1000);
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

float readTemperature()
{
  unsigned short reading = analogRead(THERMISTOR_PIN);
  float temp = TemperatureSensor.getTemperature(reading);
  return temp;
}

void initServerConnection()
{
  bool status = nodeServer.initConnection();

  if (status)
  {
    digitalWrite(SERVER_CONNECTED_PIN, HIGH);
  }
  else
  {
    serverDeadState(SERVER_CONNECTED_PIN);
  }

#if MAIN_DEBUG
  Serial.println("UUID: " + nodeServer.getUuid());
#endif
}

void serverDeadState(unsigned char indicatorPinLed)
{
  UpdateDelayer deadStateDelayer(DEADSTATE_RETRY_MILLIS);
  while (true)
  {
    digitalWrite(indicatorPinLed, HIGH);
    delay(100);
    digitalWrite(indicatorPinLed, LOW);
    delay(200);

    if (deadStateDelayer.canUpdate() &&
        nodeServer.initConnection())
    {
      digitalWrite(SERVER_CONNECTED_PIN, HIGH);
#if MAIN_DEBUG
      Serial.println("Exiting server dead state");
#endif
      return;
    }
  }
}

void initWiFiConnection()
{
  connectWiFi(ssid, password);
  digitalWrite(WIFI_CONNECTED_PIN, HIGH);

#if MAIN_DEBUG
  printWiFiStatus();
#endif
}

void setupPins()
{
  pinMode(THERMISTOR_PIN, INPUT);
  pinMode(WIFI_CONNECTED_PIN, OUTPUT);
  pinMode(SERVER_CONNECTED_PIN, OUTPUT);
  analogReadResolution(ANALOG_RES);
}