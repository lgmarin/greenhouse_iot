#include <readSensors.h>

// uint32_t            dhtDelay;
// int                 dhtTemperature;
// int                 dhtHumidity;
// int                 soilPercent;

int                  airMoist;
int                  waterMoist;

SoilMoistureSensor  soil(SOIL_A, airMoist, waterMoist);

/*!
 *  @brief  Read soil moisture value

 *	@return Soil Moisture value
 */
Dht::Dht() : _dht(DHTPIN, DHTTYPE)
{
}

Dht::~Dht()
{
}

uint32_t Dht::dhtInit()
{
    _dht.begin();
    dhtDelay = _dhtSensor.min_delay;
   
    if (dhtDelay <= 1000)
        dhtDelay = DEFAULT_DELAY;

    return dhtDelay;
}

uint32_t Dht::getDhtDelay()
{
    return dhtDelay;
}

int Dht::readTemperature()
{
    _dht.temperature().getEvent(&_dhtEvent);

    if (isnan(_dhtEvent.temperature)) {
        Serial.println(F("\n[ERROR]: Error reading temperature!"));
        return 00;
    }
    else {
        return round(_dhtEvent.temperature);
    }
}

int Dht::readHumidity()
{
    _dht.humidity().getEvent(&_dhtEvent);
    
    if (isnan(_dhtEvent.relative_humidity)) {
        Serial.println(F("\n[ERROR]: Error reading humidity!"));
        return 00;
    }
    else {
        return round(_dhtEvent.relative_humidity);
    }
}
