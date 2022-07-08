#ifndef definitions_h
#define definitions_h

#include <Arduino.h>
#include <Wire.h>

/*
*            SENSORS DEFINITIONS
*/

// DHT CONFIGURATION
#define DHTTYPE             DHT11
#define DHTPIN              2 //GPIO2 - D4

// DHT Read Values
extern int                  dhtTemperature;
extern int                  dhtHumidity;

// SOIL MOISTURE CONFIGURATION
#define SOIL_A              A0
#define DEF_CAL_AIR         120
#define DEF_CAL_WAT         1000

// SOIL READ VALUES
extern int                  soilValue;
extern int                  soilPercent;

// DISPLAY CONFIGURATION
// PINOUT I2C               
// SDA                      GPIO4 - D2
// SCK                      GPIO5 - D1
#define SCREEN_W            128
#define SCREEN_H            64
#define D_I2C_ADDR          0x3C


/*
*            WEB DEFINITIONS
*/



#endif