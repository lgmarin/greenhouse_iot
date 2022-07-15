#ifndef display_h
#define display_h

#include <definitions.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display{
private:
    Adafruit_SSD1306    display;
    unsigned long       _previous_millis = 0;
    bool                _sleeping = false;
    
public:
    Display();
    ~Display();
    void Init();
    void UpdateScreen(int temp, int hum, int soilP, String info);
    void Sleep(unsigned long current_millis);
    bool Wake(unsigned long current_millis);
};

#endif