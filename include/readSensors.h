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
public:
    Dht();
    ~Dht();
    void dhtInit();
};




#endif