#include <dhtSensor.h>

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
    delay(200);

    _dhtDelay = _dhtSensor.min_delay;
   
    if (_dhtDelay <= 1000)
        _dhtDelay = DEFAULT_DELAY;

    Serial.print(F("\n[INFO]: DHT Initialized with delay: ")); Serial.print(_dhtDelay);
    return _dhtDelay;
}

uint32_t Dht::getDhtDelay()
{
    return _dhtDelay;
}

int Dht::readTemperature()
{
    _dht.temperature().getEvent(&_dhtEvent);

    if (isnan(_dhtEvent.temperature)) {
        //Serial.print(F("\n[ERROR]: Error reading temperature!"));
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
        //Serial.print(F("\n[ERROR]: Error reading humidity!"));
        return 00;
    }
    else {
        return round(_dhtEvent.relative_humidity);
    }
}