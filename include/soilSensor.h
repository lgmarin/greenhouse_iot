#ifndef soilSensor_h
#define soilSensor_h

#include <Arduino.h>



/**
 * Read Soil Moisture Sensor values from Analog input.
 */
class SoilMoistureSensor
{
public:
    enum calibrationState: uint8_t {
        START_AIR           = 0,
        AIR_CALIB           = 1,
        START_WAT           = 2,
        WAT_CALIB           = 3,
        CALIB_SUCCESS       = 4,
        CALIB_FAIL          = 5,
        NOT_STARTED         = 6,
        WAIT                = 7,
    };

    typedef struct calibrationStatus
    {
        calibrationState   currentState;
        calibrationState   nextState;
    }calibrationStatus;

private:
    int     _analogPin;
    int     _airMoist;
    int     _waterMoist;
    bool    _calibrated;
    calibrationStatus   _calibStatus = {NOT_STARTED, START_AIR};

public:
    SoilMoistureSensor(int pin);
    SoilMoistureSensor(int pin, int airM, int watM);
    int readPercent(bool twoDigits = true);
    int readValue();
    bool enterCalibration(int airM, int watM);
    void calibrateSensor(calibrationStatus* status);
};

int readSoilP();

#endif