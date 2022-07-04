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

* [Wifi Manager - Configure Wifi Connection](https://randomnerdtutorials.com/wifimanager-with-esp8266-autoconnect-custom-parameter-and-manage-your-ssid-and-password/)
* [Async WifiManager - AsyncWebServer](https://randomnerdtutorials.com/esp8266-nodemcu-wi-fi-manager-asyncwebserver/)
* [Async WifiManager](https://stonez56.blogspot.com/2021/07/asyncwifimanager-elegantota-esp8266.html)
* [Material Design Lite Pages](https://www.luisllamas.es/material-design-esp8266/)
* [Scan WIFI](https://www.engineersgarage.com/esp8266-post-data-to-thingspeak-server/)
* [Rest Server](https://www.mischianti.org/2020/05/24/rest-server-on-esp8266-and-esp32-get-and-json-formatter-part-2/)
* [AsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer#setup-event-source-in-the-browser)
* [WIFI Connect](https://github.com/ryanamaral/wifi-connect-esp8266/tree/master/html-preview)