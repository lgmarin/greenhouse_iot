#include <readSensors.h>

// GLOBAL VARIABLES
uint32_t                _dhtDelay;
int                     _dhtTemperature;
int                     _dhtHumidity;
int                     _soilPercent;

SoilMoistureSensor      soil(SOIL_A);
Dht                     dhtSensor;

void initSensors()
{
    _dhtDelay = dhtSensor.dhtInit();

    delay(_dhtDelay);

    _dhtTemperature = dhtSensor.readTemperature();
    _dhtHumidity = dhtSensor.readHumidity();
    _soilPercent = soil.readPercent();  
}

void sensorsLoop()
{
    _dhtTemperature = dhtSensor.readTemperature();
    _dhtHumidity = dhtSensor.readHumidity();
    _soilPercent = soil.readPercent();
}

unsigned long dhtDelay()
{
    return _dhtDelay;
}

int airTemp()
{
    return _dhtTemperature;
}

int airHumidity()
{
    return _dhtHumidity;
}

int soilHumidity()
{
    return _soilPercent;
}