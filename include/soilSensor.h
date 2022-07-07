#ifndef soilSensor_h
#define soilSensor_h

#include <Arduino.h>

class SoilMoistureSensor
{
private:
    int analogPin;
    int airMoist;
    int waterMoist;
    bool calibrated;
public:
    SoilMoistureSensor(int pin);
    SoilMoistureSensor(int pin, int airM, int watM);
    int readPercent(bool twoDigits = true);
    int readValue();
    void calibrateSerial();
};

#endif