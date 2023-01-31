#ifndef btnmanager_h
#define btnmanager_h

#include <definitions.h>

extern "C" {
  typedef void (*buttonCallback)(void);
}

class Button
{
private:
    uint8_t             _pin;
    unsigned long       _clickDelay = 400;
    unsigned long       _pressDelay = 800;
    unsigned long       _debounceDelay = 50;
    unsigned long       _startTime = 0;
    int                 _nClicks = 0;

    enum buttonState_t : uint8_t {
        BTN_STATE_INIT      = 0,
        BTN_STATE_DOWN      = 1,
        BTN_STATE_UP        = 2,
        BTN_STATE_COUNT     = 3,
        BTN_STATE_PRESS     = 6,
        BTN_STATE_PRESSEND  = 7,
    };

    buttonState_t _state = BTN_STATE_INIT;
    buttonState_t _lastState = BTN_STATE_INIT;

public:
    Button(uint8_t pin) {
        _pin = pin;
        pinMode(_pin, INPUT);
        digitalWrite(_pin, HIGH);
    }
    ~Button() {}
    void loop();

//Callback Attachers
    void attachSingleClickFunc(buttonCallback function);
    void attachDoubleClickFuncc(buttonCallback function);
    void attachLongPressFunc(buttonCallback function);

private:
    void _newState(buttonState_t nextState);
    void _reset();
    void _checkState(bool isActive);

    //Callbacks
    buttonCallback _singleClickFunc = NULL;
    buttonCallback _doubleClickFunc = NULL;
    buttonCallback _longPressFunc = NULL;

};

#endif