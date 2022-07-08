#ifndef readSensors_h
#define readSensors_h

#include <Arduino.h>
#include <definitions.h>
#include <soilSensor.h>
#include <DHT.h>
#include <DHT_U.h>

class Dht
{
private:
    DHT_Unified         dht;
    sensors_event_t     dhtEvent;
    sensor_t            dhtSensor;
    int32_t            dhtDelay;

public:
    Dht();
    ~Dht();
    void dhtInit();
    int32_t getDhtDelay();
    int readTemperature();
    int readHumidity();
};




#endif