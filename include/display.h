#ifndef display_h
#define display_h

#include <definitions.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display{
private:
    Adafruit_SSD1306    _display;
    unsigned long       _previous_millis = 0;
    bool                _sleeping = false;
    uint8               _activeScreen = 0;
    uint8               _totalScreens = 3;
    
public:
    Display();
    ~Display();
    void Init();
    void UpdateDisplay();
    void ChangeScreen(uint8 screen);
    void nextScreen();
    void Sleep(unsigned long current_millis);
    bool Wake(unsigned long current_millis);

private:
    void _mainScreen();
    void _wifiScreen();
    void _calibrationScreen();
};

#endif