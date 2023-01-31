#include <display.h>
#include <readSensors.h>
#include <wifiConfig.h>

/*!
 *  @brief  Initialize SSD1306 Display.
 */
void Display::init()
{
    if(!_display.begin(SSD1306_SWITCHCAPVCC, D_I2C_ADDR)) {
        Serial.print(F("\n[ERROR]: SSD1306 allocation failed!"));
        for(;;);
    }
    delay(2000);
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setFont(NULL);
    _display.setCursor(0, 0);
    _display.setTextColor(WHITE);
    _display.print("Initializing...");
}

/*!
 *  @brief  Update the display screen for loop().
 */
void Display::updateDisplay()
{
    if(_sleeping)
        return;
    
    sleep();

    switch (_activeScreen)
    {
    case 0:
        _mainScreen();
        break;
    case 1:
        _calibrationScreen();
        break;
    case 2:
        _wifiScreen();
        break;
    default:
        _mainScreen();
        break;
    }
}

void Display::changeScreen(uint8_t screen)
{
    _activeScreen = screen;
}

void Display::nextScreen()
{
    int currentScreen = _activeScreen;
    if (currentScreen += 1 > _totalScreens - 1)
        _activeScreen = 0;
    else 
        _activeScreen += 1;
}

void Display::sleep()
{
    unsigned long current_millis = millis();

    if (current_millis - _previous_millis >= SLEEP_INTERVAL) 
    {
        _previous_millis = current_millis;
        _display.ssd1306_command(SSD1306_DISPLAYOFF);
        _sleeping = true;
        changeScreen(0);
    }    
}

bool Display::wake()
{
    if (_sleeping) 
    {
        _previous_millis = millis();
        _display.ssd1306_command(SSD1306_DISPLAYON);
        _sleeping = false;
        return false;
    }

    return true;
}


void Display::btnHandleClick()
{
    if(_sleeping)
        wake();

    else
    {
        nextScreen();
        _previous_millis = millis();
    }
}

void Display::btnHandleLongPress()
{
    if(_sleeping)
       return;

    switch (_activeScreen)
    {
    case 0:
        break;
    case 1:
        //Do calibration stuff
        break;
    case 2:
        //Do Wifi stuff
        break;
    default:
        break;
    }    
}


/*
    SCREENS DRAWING FUNCTIONS
*/
void Display::_drawHeader()
{
    // DISPLAY HANDLING
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setFont(NULL);
    _display.setTextColor(WHITE);
    // Information
    _display.setCursor(0, 0);
}

void Display::_mainScreen()
{
    _drawHeader();
    _display.print(wifiInfo());
    // Temperature
    _display.setCursor(0, 16);
    _display.setTextSize(1);
    _display.print("Ta ");
    _display.setTextSize(2);
    _display.print(airTemp());
    _display.setTextSize(1);
    _display.print("o");
    _display.setTextSize(2);
    _display.print("C ");
    // Humidity
    _display.setTextSize(1);
    _display.print("Ua ");
    _display.setTextSize(2);
    _display.print(airHumidity());
    _display.print("%");
    // Soil Humidity
    _display.setCursor(0, 36);
    _display.setTextSize(1);
    _display.print("Us ");
    _display.setTextSize(2);
    _display.print(soilHumidity());
    _display.print("%");
    _display.display();
}

void Display::_wifiScreen()
{
    _drawHeader();
    _display.print("WIFI Config");
    // Temperature
    _display.setCursor(0, 16);
    _display.setTextSize(1);
    _display.print("Ta ");
    _display.setTextSize(2);
    _display.print(airTemp());
    _display.setTextSize(1);
    _display.print("o");
    _display.setTextSize(2);
    _display.print("C ");
    // Humidity
    _display.setTextSize(1);
    _display.print("Ua ");
    _display.setTextSize(2);
    _display.print(airHumidity());
    _display.print("%");
    // Soil Humidity
    _display.setCursor(0, 36);
    _display.setTextSize(1);
    _display.print("Us ");
    _display.setTextSize(2);
    _display.print(soilHumidity());
    _display.print("%");
    _display.display();
}

void Display::_calibrationScreen()
{
    _drawHeader();
    _display.print("WIFI Config");
    // Temperature
    _display.setCursor(0, 16);
    _display.setTextSize(1);
    _display.print("Ta ");
    _display.setTextSize(2);
    _display.print(airTemp());
    _display.setTextSize(1);
    _display.print("o");
    _display.setTextSize(2);
    _display.print("C ");
    // Humidity
    _display.setTextSize(1);
    _display.print("Ua ");
    _display.setTextSize(2);
    _display.print(airHumidity());
    _display.print("%");
    // Soil Humidity
    _display.setCursor(0, 36);
    _display.setTextSize(1);
    _display.print("Us ");
    _display.setTextSize(2);
    _display.print(soilHumidity());
    _display.print("%");
    _display.display();
}
