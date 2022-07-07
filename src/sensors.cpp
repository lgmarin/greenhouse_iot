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

int SoilMoistureSensor::readValue()
{
    return analogRead(analogPin);
}

int SoilMoistureSensor::readPercent()
{
    if (calibrated)
    {
        int val = analogRead(analogPin);
        return map(val, airMoist, waterMoist, 0, 100);
    }
    return 00;
}

void SoilMoistureSensor::calibrate()
{
    Serial.print(F("\n<<< SOIL MOISTURE CALIBRATION >>>"));
    Serial.print(F("\n1. Leave the probe in the air (probe dry)."));
    Serial.print(F("\n--> Take note of this measurement! "));
    Serial.print(F("\n2. Insert the probe in a glass of water."));
    Serial.print(F("\n--> Take note of this measurement!"));
    Serial.print(F("\nREAD THE INSTRUCTIONS AND PRESS ENTER!"));
    Serial.read();
    Serial.print(F("\nCURRENT READING: ")); Serial.print(readValue());
}