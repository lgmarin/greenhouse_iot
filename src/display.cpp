#include <display.h>
#include <readSensors.h>
#include <wifiConfig.h>

Display::Display(): display(SCREEN_W, SCREEN_H, &Wire, -1)
{
}

Display::~Display()
{
}

/*!
 *  @brief  Initialize SSD1306 Display.
 */
void Display::Init()
{
    if(!display.begin(SSD1306_SWITCHCAPVCC, D_I2C_ADDR)) {
        Serial.print(F("\n[ERROR]: SSD1306 allocation failed!"));
        for(;;);
    }
    delay(2000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setFont(NULL);
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.print("Initializing...");
}

/*!
 *  @brief  Update the display screen for loop().
 */
void Display::UpdateScreen()
{
    switch (_activeScreen)
    {
    case 0:
        mainScreen();
        break;
    case 1:
        wifiScreen();
        break;
    case 2:
        calibrationScreen();
        break;
    default:
        mainScreen();
        break;
    }
}

void Display::wifiScreen()
{
}

void Display::calibrationScreen()
{
}

void Display::mainScreen()
{
    // DISPLAY HANDLING
    display.clearDisplay();
    display.setTextSize(1);
    display.setFont(NULL);
    display.setTextColor(WHITE);
    // Information
    display.setCursor(0, 0);
    display.print(wifiInfo());
    // Temperature
    display.setCursor(0, 16);
    display.setTextSize(1);
    display.print("Ta ");
    display.setTextSize(2);
    display.print(airTemp());
    display.setTextSize(1);
    display.print("o");
    display.setTextSize(2);
    display.print("C ");
    // Humidity
    display.setTextSize(1);
    display.print("Ua ");
    display.setTextSize(2);
    display.print(airHumidity());
    display.print("%");
    // Soil Humidity
    display.setCursor(0, 36);
    display.setTextSize(1);
    display.print("Us ");
    display.setTextSize(2);
    display.print(soilHumidity());
    display.print("%");
    display.display();
}

void Display::Sleep(unsigned long current_millis)
{
    if (current_millis - _previous_millis >= SLEEP_INTERVAL) 
    {
        _previous_millis = current_millis;
        display.ssd1306_command(SSD1306_DISPLAYOFF);
        _sleeping = true;
    }    
}

bool Display::Wake(unsigned long current_millis)
{
    _previous_millis = current_millis;
    if (_sleeping) 
    {
        display.ssd1306_command(SSD1306_DISPLAYON);
        _sleeping = false;
        return false;
    }

    return true;
}