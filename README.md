# Plant Watcher - Sensor Node

This repo contains the code for the sensor node in the plant watcher system.

### Code Structure

The code utilizes the PlatformIO framework to stay away from using Arduino tooling. More information about PlatformIO [here](platformio.org/).

This code runs on Arduino `MKR1000`, the logic components are separated into library files that are platform independent under the `lib` directory.

### Sub components

The `main.cpp` manages the following components

- UDP Node + Communication protocol `NodeServer`
- Timing (`NTP` client)
- Other simple components for thermistor, WiFi, DHT Sensor and delayed update
