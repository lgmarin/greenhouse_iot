#include <display.h>

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
 *  @brief  Update the display screen.
 *  @param  temp Air temperature.
 *  @param  humidity Air humidity.
 *  @param  soilP Soil humidity in percent.
 *  @param  info Info string to display at the top. 
 */
void Display::UpdateScreen(int temp, int hum, int soilP, String info)
{
    // DISPLAY HANDLING
    display.clearDisplay();
    display.setTextSize(1);
    display.setFont(NULL);
    display.setTextColor(WHITE);
    // Information
    display.setCursor(0, 0);
    display.print(info);
    // Temperature
    display.setCursor(0, 16);
    display.setTextSize(1);
    display.print("Ta ");
    display.setTextSize(2);
    display.print(temp);
    display.setTextSize(1);
    display.print("o");
    display.setTextSize(2);
    display.print("C ");
    // Humidity
    display.setTextSize(1);
    display.print("Ua ");
    display.setTextSize(2);
    display.print(hum);
    display.print("%");
    // Soil Humidity
    display.setCursor(0, 36);
    display.setTextSize(1);
    display.print("Us ");
    display.setTextSize(2);
    display.print(soilP);
    display.print("%");
    display.display();
}