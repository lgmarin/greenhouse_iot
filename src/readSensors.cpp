#include <readSensors.h>

// GLOBAL VARIABLES
uint32_t                _dhtDelay;
int                     _dhtTemperature;
int                     _dhtHumidity;
int                     _soilPercent;
int                     _soilValue;

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
    _soilValue = soil.readValue();
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

String readSensorsJSON()
{
    String json;

    json += "{";
    json +=  "{\"dht_t\":" + _dhtTemperature;
    json +=  "}, {\"dht_h\":" + _dhtHumidity;
    json +=  "}, {\"soil_p\":" + _soilPercent;
    json +=  "}, {\"soil_v\":" + _soilValue;
    json += "}}";

    return json;
}