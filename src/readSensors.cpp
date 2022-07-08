#include <readSensors.h>

uint32_t            dhtDelay;
int                 dhtTemperature;
int                 dhtHumidity;
int                 soilPercent;
SoilMoistureSensor  soil(SOIL_A, airMoist, waterMoist);


Dht::Dht() : dht(DHTPIN, DHTTYPE)
{
}

Dht::~Dht()
{
}

void Dht::dhtInit()
{

}
