#include <soilSensor.h>

SoilMoistureSensor::SoilMoistureSensor(int pin)
{
    _analogPin = pin;
    _airMoist = 0;
    _waterMoist = 0;
    _calibrated = false;
}

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
        int val = map(analogRead(_analogPin), _airMoist, _waterMoist, 0, 100);

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
    //Serial.print(F("\n[WARNING]: Soil sensor needs calibration!"));
    return 0;
}

/*!
 *  @brief  Calibrate from Serial Monitor.
*           Put in the loop function and read the values.   
 */
void SoilMoistureSensor::calibrateSerial()
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