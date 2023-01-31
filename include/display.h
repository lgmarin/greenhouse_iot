#ifndef display_h
#define display_h

#include <definitions.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <buttonManager.h>

class Display{
private:
    Adafruit_SSD1306    _display;
    unsigned long       _previous_millis = 0;
    bool                _sleeping = false;
    uint8_t             _activeScreen = 0;
    uint8_t             _totalScreens = 3;
    
public:
    Display() 
        : _display(SCREEN_W, SCREEN_H, &Wire, -1) {};
    ~Display(){};

    void init();
    void updateDisplay();

    void changeScreen(uint8_t screen);
    void nextScreen();

    void sleep();
    bool wake();

public: // MENU BUTTON CALLBACKS
    void btnHandleClick();
    void btnHandleLongPress();

private: // DRAW SCREENS
    void _drawHeader();
    void _mainScreen();
    void _wifiScreen();
    void _calibrationScreen();
};

#endif