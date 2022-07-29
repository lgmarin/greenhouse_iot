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
    * Allow to calibrate sensor through the web interface
* Develop clean and modular code (OOP)
  * Will be merged later for the [esp_manager][esp_manager] boiler plate project

## Web FrontEnd

* Made with Material Design Lite
  * Using HTML and pure JavaScript
  * Obsolete but with a very small size and easy to use across different devices
  * Focus on simple usage
    * Data visualization
    * Wifi Configuration
    * Device Configuration

<p align="center">
  <img width="250" src="https://raw.githubusercontent.com/lgmarin/greenhouse_iot/main/img/scr_index.png" alt="Home Screen">
  <img width="250" src="https://raw.githubusercontent.com/lgmarin/greenhouse_iot/main/img/scr_cfg1.png" alt="Config Screen - Part 1">
  <img width="250" src="https://raw.githubusercontent.com/lgmarin/greenhouse_iot/main/img/scr_cfg2.png" alt="Config Screen - Part 2">
  <img width="250" src="https://raw.githubusercontent.com/lgmarin/greenhouse_iot/main/img/scr_wifi1.png" alt="Wifi Config Screen - Part 1">
  <img width="250" src="https://raw.githubusercontent.com/lgmarin/greenhouse_iot/main/img/scr_wifi2.png" alt="Wifi Config Screen - Part 2">    
</p>

## Schematics and List of Materials

<p align="center">
  <img width="300" src="https://raw.githubusercontent.com/lgmarin/greenhouse_iot/main/img/schematics.png" alt="Schematics">
</p>

| Label | Part Type | Properties |
|-----|-----|---|
|C1|Capacitor Polarized|Electrolytic 10uf 25v|
|J1|Piezo Speaker|-|
|R1-R4|10kΩ Resistor|Tolerance ±5%, Resistance 10kΩ|
|U1|ESP8266 WiFi Module|ESP12F|
|U2|DHT11|Temperature and humidity sensor|
|U3|SSD1306|0.96 in OLED I2C Display 128x64|
|U4|HW-103|Soil Moisture Sensor|

## Libraries

* [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) - Base library for ESPAsyncWebServer.
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) - An Async Web Server implementation for the ESP8266.
* Adafruit DHT11 Air Temperature and Humidity Sensor
* Adafruit SSD1306 Display


## References

* [Capacitive soil moisture](https://how2electronics.com/capacitive-soil-moisture-sensor-esp8266-esp32-oled-display/)
* [Weather station](https://www.engineersgarage.com/nodemcu-weather-station/)
* [DHT11/22 Sensor](https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/)
* [SSD1306 OLED Display](https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide/)
* [Display Generator](https://rickkas7.github.io/DisplayGenerator/index.html)
* [Class Inheritance](https://forum.arduino.cc/t/access-adafruit-ssd1306-object-inside-my-class/969569)

[esp_manager]: https://github.com/lgmarin/esp_manager