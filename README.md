# Greenhouse IoT

## _Greenhouse monitoring system based on ESP8266_
[![ESP8266](https://img.shields.io/badge/ESP-8266-blue.svg)](https://github.com/esp8266/esp8266-wiki)
[![C++](https://img.shields.io/badge/C-++-blue.svg)]()
[![PlatformIO](https://img.shields.io/badge/Platform-IO-blue.svg)](https://platformio.org/)

## Objectives

Development of a greenhouse monitoring system:

* Multiple data accessing for easy reading
  * Display
  * AP mode dashboard
  * STA mode dashboard
  * Maybe: MQTT data provider
* Simple dashboard, configure wifi, board name, reading interval
* Sensors
  * Air temperature and humidity - DHT11
  * Soil moisture sensor

## Libraries

* [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) - Base library for ESPAsyncWebServer.
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) - An Async Web Server implementation for the ESP8266.
* [ArduinoJson]() - Json serializer and deserializer for Arduino.

## References

* [Capacitive soil moisture sensor](https://how2electronics.com/capacitive-soil-moisture-sensor-esp8266-esp32-oled-display/)
* [Weather station](https://www.engineersgarage.com/nodemcu-weather-station/)
* [DHT11/22 Sensor](https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/)