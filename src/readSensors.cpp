#include <readSensors.h>

// GLOBAL VARIABLES
uint32_t                _dhtDelay;
int                     _dhtTemperature;
int                     _dhtHumidity;
int                     _soilPercent;
int                     _soilValue;

SoilMoistureSensor      soil(SOIL_A);
Dht                     dhtSensor;

/**
 * @brief Initialize all the sensors functions
 * 
 */
void initSensors()
{
    _dhtDelay = dhtSensor.dhtInit();

    delay(_dhtDelay);

    _dhtTemperature = dhtSensor.readTemperature();
    _dhtHumidity = dhtSensor.readHumidity();

    soil.enterCalibration(Device_config.air_value, Device_config.wat_value);
    _soilPercent = soil.readPercent();
}

/**
 * @brief Sensors main loop, read all the sensors values periodically
 * 
 */
void sensorsLoop()
{
    _dhtTemperature = dhtSensor.readTemperature();
    _dhtHumidity = dhtSensor.readHumidity();
    _soilValue = soil.readValue();
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

/**
 * @brief Read Sensors values in JSON format
 * 
 * @return String JSON sensors readings
 */
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