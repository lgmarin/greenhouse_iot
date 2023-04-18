#ifndef display_h
#define display_h

#include <definitions.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <readSensors.h>
#include <wifiConfig.h>

class Display{
private:
    Adafruit_SSD1306    _display;
    unsigned long       _previous_millis = 0;
    bool                _sleeping = false;
    unsigned long       _sleep_millis = 0;
    bool                _sleepBlocked = false;
    uint8_t             _activeScreen = 0;
    uint8_t             _totalScreens = 3;
    bool                _isCallibrationRunning = false;

    enum screens: uint8_t {
        MAIN_SCREEN         = 0,
        WIFI_SCREEN         = 1,
        CALIB_SCREEN        = 2,
        RUN_CALIB_SCREEN    = 21,
        FINISH_SCREEN       = 22,
    };

    screens mainScreens[2] = {MAIN_SCREEN, WIFI_SCREEN};

    enum callibState: uint8_t {
        START_AIR           = 0,
        AIR_CALIB           = 1,
        START_WAT           = 2,
        WAT_CALIB           = 3,
        CALIB_SUCCESS       = 4,
        CALIB_FAIL          = 5,
        NOT_STARTED         = 6,
    };

    callibState          _currentCalibState = NOT_STARTED;
    callibState          _nextCalibState = START_AIR;
    
public:
    Display() 
        : _display(SCREEN_W, SCREEN_H, &Wire, -1) {};
    ~Display(){};

    void init();
    void updateDisplay();

    void changeScreen(uint8_t screen);
    void nextScreen();

    void sleep();
    void lockSleepMode();
    void unlockSleepMode();
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
    void _calibrationFinishScreen();

    void _drawCenterText(const char *buf, uint8_t y);

    void _callibrationHelper();
};

#endif