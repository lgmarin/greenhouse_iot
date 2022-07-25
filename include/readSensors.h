#ifndef readSensors_h
#define readSensors_h

#include <definitions.h>
#include <soilSensor.h>
#include <dhtSensor.h>
#include <configManager.h>

void initSensors();
void sensorsLoop();
unsigned long dhtDelay();
int airTemp();
int airHumidity();
int soilHumidity();

String readSensorsJSON();

#endif