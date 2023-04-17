#ifndef readSensors_h
#define readSensors_h

#include <definitions.h>
#include <soilSensor.h>
#include <dhtSensor.h>
#include <configManager.h>

/**
 * @brief Initialize all the sensors functions 
 */
void initSensors();
/**
 * @brief Sensors main loop, read all the sensors values periodically
 */
void sensorsLoop();
unsigned long dhtDelay();
int airTemp();
int airHumidity();
int soilHumidity();

/**
 * @brief Read Sensors values in JSON format
 * @return String JSON sensors readings
 */
String readSensorsJSON();

#endif