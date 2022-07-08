#include <readSensors.h>

uint32_t            dhtDelay;
int                 dhtTemperature;
int                 dhtHumidity;
int                 soilPercent;

SoilMoistureSensor  soil(SOIL_A, airMoist, waterMoist);

/*!
 *  @brief  Read soil moisture value

 *	@return Soil Moisture value
 */
Dht::Dht() : dht(DHTPIN, DHTTYPE)
{
}

Dht::~Dht()
{
}

void Dht::dhtInit()
{
    dht.begin();
    dhtDelay = dhtSensor.min_delay;
}

int32_t Dht::getDhtDelay()
{
    if (dhtDelay <= 1000)
    {
        return 2000;
    }
    return dhtDelay;
}

int Dht::readTemperature()
{
    dht.temperature().getEvent(&dhtEvent);

    if (isnan(dhtEvent.temperature)) {
        Serial.println(F("\n[ERROR]: Error reading temperature!"));
        return 00;
    }
    else {
        return round(dhtEvent.temperature);
    }
}

int Dht::readTemperature()
{
    dht.humidity().getEvent(&dhtEvent);
    
    if (isnan(dhtEvent.relative_humidity)) {
        Serial.println(F("\n[ERROR]: Error reading humidity!"));
        return 00;
    }
    else {
        return round(dhtEvent.relative_humidity);
    }
}
