#ifndef display_h
#define display_h

#include <definitions.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Button
{
private:
    uint8_t             _pin;
    int                 _debounceDelay = 5;
    int                 _lastDebounce = 0;
    uint8_t             _inputState = LOW;
    uint8_t             _lastInputState = LOW;

public:
    Button(uint8_t pin) {
        _pin = pin;
        pinMode(_pin, INPUT);
        digitalWrite(_pin, HIGH);
    }
    ~Button() {}
    void ButtonLoop(Display display);

private:
    void _readButtonPress();
};

class Display{
private:
    Adafruit_SSD1306    _display;
    unsigned long       _previous_millis = 0;
    bool                _sleeping = false;
    uint8_t             _activeScreen = 0;
    uint8_t             _totalScreens = 3;
    
public:
    Display();
    ~Display();
    void Init();
    void UpdateDisplay();
    void ChangeScreen(uint8_t screen);
    void nextScreen();
    void Sleep(unsigned long current_millis);
    bool Wake(unsigned long current_millis);

private:
    void _mainScreen();
    void _wifiScreen();
    void _calibrationScreen();
};

#endif