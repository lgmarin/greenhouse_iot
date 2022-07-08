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
    DHT_Unified         _dht;
    sensors_event_t     _dhtEvent;
    sensor_t            _dhtSensor;
    uint32_t            _dhtDelay;

public:
    Dht();
    ~Dht();
    uint32_t dhtInit();
    uint32_t getDhtDelay();
    int readTemperature();
    int readHumidity();
};




#endif