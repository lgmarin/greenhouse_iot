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
    uint8_t             _activeScreen = 0;
    uint8_t             _totalScreens = 3;
    bool                _isCallibrationRunning = false;

    enum screens: uint8_t
    {
        MAIN_SCREEN         = 0,
        WIFI_SCREEN         = 1,
        CALIB_SCREEN        = 2,
        RUN_CALIB_SCREEN    = 21,
        SUCC_CALIB_SCREEN   = 22
    };
    
public:
    Display() 
        : _display(SCREEN_W, SCREEN_H, &Wire, -1) {};
    ~Display(){};

    void init();
    void updateDisplay();

    void changeScreen(uint8_t screen);
    void nextScreen();

    void sleep();
    bool toggleSleepMode();
    bool wake();

public: // MENU BUTTON CALLBACKS
    void btnHandleClick();
    void btnHandleLongPress();

private: // DRAW SCREENS
    void _beginScreenDraw();
    void _mainScreen();
    void _wifiScreen();
    void _calibrationScreen();
    void _runningCalibrationScreen();
    void _calibrationSuccessScreen();

    void _drawCenterText(const char *buf, uint8_t x, uint8_t y);
};

#endif