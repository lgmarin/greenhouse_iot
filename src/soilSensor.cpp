#include <soilSensor.h>

/*!
 *  @brief  Initialize the SoilMoisture Sensor
 *  @param  pin SoilMoisture Sensor Analog pin
 */
SoilMoistureSensor::SoilMoistureSensor(int pin)
{
    _analogPin = pin;
    _airMoist = 0;
    _waterMoist = 0;
    _calibrated = false;
}

/*!
 *  @brief  Initialize the SoilMoisture Sensor
 *  @param  pin SoilMoisture Sensor Analog pin
 *  @param  airM Calibration value - Air raw measure
 *  @param  watM Calibration value - Water raw measure
 */
SoilMoistureSensor::SoilMoistureSensor(int pin, int airM, int watM)
{
    _analogPin = pin;
    _airMoist = airM;
    _waterMoist = watM;
    _calibrated = true;  
}

/*!
 *  @brief  Read soil moisture value
 *	@return Soil Moisture value
 */
int SoilMoistureSensor::readValue()
{
    return analogRead(_analogPin);
}

/*!
 *  @brief  Read soil moisture percent
 *  @param  twoDigits
 *          Display only 2 digits value 0-99%:
 *					- true = Two Digits
 *					- false = All Digits
 *	@return Soil Moisture value in percent
 */
int SoilMoistureSensor::readPercent(bool twoDigits)
{  
    if (_calibrated)
    {
        int val = map(readValue(), _airMoist, _waterMoist, 0, 100);

        if (twoDigits)
        {
            if(val > 100)
            {
                return 99;
            }
            else if(val < 0)
            {
                return 0;
            }
            else if(val >=0 && val < 100)
            {
                return val;
            }  
        }

        return val;
    }
    return 0;
}

/*!
 *  @brief  Enter calibration values after creating class.
*   @param airM Moisture value in Air
*   @param watM Moisture value in Water
*   @return Returns true if entered values are valid.
 */
bool SoilMoistureSensor::enterCalibration(int airM, int watM)
{
    if (!isnan(airM) && !isnan(watM))
    {
        _airMoist = airM;
        _waterMoist = watM;
        _calibrated = true; 
        return true;
    }
    return false;
}

void SoilMoistureSensor::calibrateSensor(calibrationStatus* status)
{
    if(_calibStatus.currentState == NOT_STARTED)
    {
        _calibStatus.currentState = START_AIR;
        _calibStatus.nextState = WAIT;
        *status = _calibStatus;
    }
        
    switch (_calibStatus.currentState)
    {
    case START_AIR:
        _calibStatus.nextState = AIR_CALIB;
        break;
    
    case AIR_CALIB:
        _calibStatus.nextState = START_WAT;
        break;

    case START_WAT:
        _calibStatus.nextState = WAT_CALIB;
        break;
    
    case WAT_CALIB:
        _calibStatus.nextState = CALIB_SUCCESS;
        break;

    case CALIB_SUCCESS:
        _calibStatus.nextState = NOT_STARTED;
        break;

    case CALIB_FAIL:
        _calibStatus.nextState = NOT_STARTED;
        break;        

    default:
        break;
    }

    *status = _calibStatus;
}
