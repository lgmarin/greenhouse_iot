#ifndef definitions_h
#define definitions_h

#include <Arduino.h>
#include <Wire.h>

/*
*            SENSORS DEFINITIONS
*/
#define DEFAULT_DELAY       2000 //2secs delay loop

//             DHT CONFIGURATION
#define DHTTYPE             DHT11
#define DHTPIN              12 //GPIO12 - D4
// DHT Read Values
// extern uint32_t             dhtDelay;
// extern int                  dhtTemperature;
// extern int                  dhtHumidity;

//      SOIL MOISTURE CONFIGURATION
#define SOIL_A              A0
#define DEF_CAL_AIR         790
#define DEF_CAL_WAT         390
// SOIL READ VALUES
// extern int                  soilValue;
// extern int                  soilPercent;
// SOIL CALIBRATION CONFIGURATION
extern int                  airMoist;
extern int                  waterMoist;


//          DISPLAY CONFIGURATION
// PINOUT I2C               
// SDA                      GPIO4 - D2
// SCK                      GPIO5 - D1
#define SCREEN_W            128
#define SCREEN_H            64
#define D_I2C_ADDR          0x3C //0x7A //0x78


/*
*            WEB DEFINITIONS
*/
#define WIFI_RETRY_COUNT        3
#define WIFI_RETRY_TIMEOUT      5000
#define SCAN_PERIOD             3500
#define DEFAULT_HOSTNAME        F("ESP_MANAGER")
#define HTTP_PORT               80


#endif