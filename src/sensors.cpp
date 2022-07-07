#include <sesnsors.h>

SoilMoistureSensor::SoilMoistureSensor(int pin)
{
    analogPin = pin;
    airMoist = 0;
    waterMoist = 0;
    calibrated = false;
}

SoilMoistureSensor::SoilMoistureSensor(int pin, int airM, int watM)
{
    analogPin = pin;
    airMoist = airM;
    waterMoist = watM;
    calibrated = true;  
}

int SoilMoistureSensor::readPercent()
{

}

int SoilMoistureSensor::readValue()
{

}

void SoilMoistureSensor::calibrate()
{

}