#include <readSensors.h>

// GLOBAL VARIABLES
uint32_t                _dhtDelay;
int                     _dhtTemperature;
int                     _dhtHumidity;
int                     _soilPercent;
int                     _soilValue;
bool                    _soilCalibration = false;
unsigned long           _soil_millis = 0;

SoilMoistureSensor      soil(SOIL_A);
Dht                     dhtSensor;

void initSensors()
{
    _dhtDelay = dhtSensor.dhtInit();

    delay(_dhtDelay);

    _dhtTemperature = dhtSensor.readTemperature();
    _dhtHumidity = dhtSensor.readHumidity();

    if(soil.enterCalibration(Device_config.air_value, Device_config.wat_value))
        _soilPercent = soil.readPercent();
}

void sensorsLoop()
{
    _dhtTemperature = dhtSensor.readTemperature();
    _dhtHumidity = dhtSensor.readHumidity();

    if(!_soilCalibration)
    {
        unsigned long current_millis = millis();
        if (current_millis - _soil_millis >= SOIL_NORMAL_INTERVAL) 
        {
            _soil_millis = current_millis;
            _soilValue = soil.readValue();
            _soilPercent = soil.readPercent();
        }
    }
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

    json += "{\"sensors\":{";
    json +=  "\"dht_t\":\""+ String(_dhtTemperature);
    json +=  "\", \"dht_h\":\""+ String(_dhtHumidity);
    json +=  "\", \"soil_p\":\""+ String(_soilPercent);
    json +=  "\", \"soil_v\":\""+ String(_soilValue);
    json += "\"}}";

    return json;
}

int readCallibrationValue()
{
    _soilCalibration = true;
    unsigned long current_millis = millis();
    int read = 0;

    for (size_t i = 0; i < 10; i++)
    {
        if (current_millis - _soil_millis >= SOIL_CALLIB_INTERVAL + 10) 
        {
            _soil_millis = current_millis;
            read += soil.readValue();
        }
    }

    _soilCalibration = false;

    return read/10;
}