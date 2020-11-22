#include <Arduino.h>
#include <TemperatureSensor.h>

#define THERMISTOR_PIN A0

void setup()
{
  pinMode(THERMISTOR_PIN, INPUT);
  Serial.begin(9600);
}

void loop()
{
  Serial.print("Temperature ");
  unsigned short reading = analogRead(THERMISTOR_PIN);
  Serial.print(TemperatureSensor.getTemperature(reading));
  Serial.println(" *C"); // convert absolute
  delay(500);
}
