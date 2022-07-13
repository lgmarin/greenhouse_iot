#ifndef display_h
#define display_h

#include <definitions.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display{
private:
    Adafruit_SSD1306 display;
    
public:
    Display();
    ~Display();
    void Init();
    void UpdateScreen(int temp, int hum, int soilP, String info);
};

#endif