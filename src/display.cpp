#include <display.h>

Display::Display(): display(SCREEN_W, SCREEN_H, &Wire, -1)
{
}

Display::~Display()
{
}

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

void Display::UpdateScreen(int temp, int hum, int soilP)
{
    // DISPLAY HANDLING
    display.clearDisplay();
    display.setTextSize(1);
    display.setFont(NULL);
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.print("IP: 192.168.100.254");
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

void Display::ConnectingWifi(String ssid)
{
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setFont(NULL);
    display.setCursor(0, 10);
    display.println("Connecting wifi:");
    display.setCursor(0, 20);
    display.println(ssid);
    display.display();
}