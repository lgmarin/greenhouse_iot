#ifndef sensors_h
#define sensors_h

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
    int readPercent();
    int readValue();
    void calibrate();
};


#endif