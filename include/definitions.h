#ifndef definitions_h
#define definitions_h

#include <Arduino.h>

/*
*            SENSORS DEFINITIONS
*/
#define DEFAULT_DELAY           3000    //3secs delay loop


//             DHT CONFIGURATION
#define DHTTYPE                 DHT11
#define DHTPIN                  12      //GPIO12 - D4


//      SOIL MOISTURE CONFIGURATION
#define SOIL_A                  A0
// SOIL CALIBRATION CONFIGURATION
#define DEF_CAL_AIR             1024
#define DEF_CAL_WAT             500


//          DISPLAY CONFIGURATION
// PINOUT I2C               
// SDA                          GPIO4 - D2
// SCK                          GPIO5 - D1
#define SCREEN_W                128
#define SCREEN_H                64
#define D_I2C_ADDR              0x3C    //0x7A //0x78
#define SLEEP_INTERVAL          60000   // 1/2 minute to sleep
#define WAKE_BTN_PIN            14      //GPIO14 - D


/*
*            WEB DEFINITIONS
*/
#define WIFI_RETRY_COUNT        3
#define WIFI_RETRY_TIMEOUT      30000
#define SCAN_PERIOD             3500
#define DEFAULT_HOSTNAME        F("greenhouseiot")
#define HTTP_PORT               80
#define DEFAULT_TO_AP           true

#endif