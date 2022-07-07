#include <soilSensor.h>

/*!
 *  @brief  Read temperature
 *  @param  S
 *          Scale. Boolean value:
 *					- true = Fahrenheit
 *					- false = Celcius
 *  @param  force
 *          true if in force mode
 *	@return Temperature value in selected scale
 */
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

/*!
 *  @brief  Read soil moisture value

 *	@return Soil Moisture value
 */
int SoilMoistureSensor::readValue()
{
    return analogRead(analogPin);
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
    if (calibrated)
    {
        int val = map(analogRead(analogPin), airMoist, waterMoist, 0, 100);

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
    Serial.print(F("\n[WARNING]: Soil sensor needs calibration!"));
    return 00;
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