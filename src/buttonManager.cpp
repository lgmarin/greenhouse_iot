#include <buttonManager.h>

void Button::loop()
{
    if (_pin >= 0)
        _checkState(digitalRead(_pin) == BUTTON_PRESSED);

}

void Button::attachSingleClickFunc(buttonCallback function)
{
    _singleClickFunc = function;
}

void Button::attachDoubleClickFuncc(buttonCallback function)
{
    _doubleClickFunc = function;
}

void Button::attachLongPressFunc(buttonCallback function)
{
    _longPressFunc = function;
}


void Button::_newState(buttonState_t nextState)
{
    _lastState = _state;
    _state = nextState;
}

void Button::_reset()
{
    _state = BTN_STATE_INIT;
    _lastState = BTN_STATE_INIT;
    _startTime = 0;
    _nClicks = 0;
}


void Button::_checkState(bool isPressed)
{
    unsigned long timeNow = millis();
    unsigned long timeInterval = timeNow - _startTime;

    switch (_state)
    {
    case BTN_STATE_INIT:
        if(isPressed)
        {
            _newState(BTN_STATE_DOWN);
            _startTime = timeNow;
            _nClicks = 0;
        }
        break;

    case BTN_STATE_DOWN:
        // Button became inactive too fast
        if(!isPressed && (timeInterval < _debounceDelay))
            _newState(_lastState);

        // Button became inactive
        else if (!isPressed)
        {
            _newState(BTN_STATE_UP);
            _startTime = timeNow;
        }
        
        // Button was pressed for some time
        else if (isPressed && (timeInterval > _pressDelay))
            _newState(BTN_STATE_PRESS);

        break;
        
    case BTN_STATE_UP:
        // Button became inactive too fast
        if (isPressed && (timeInterval < _debounceDelay))
            _newState(_lastState);

        // Button pressed, start counting
        else if (timeInterval >= _debounceDelay)
        {
            _nClicks++;
            _newState(BTN_STATE_COUNT);
        }
        break;
    
    case BTN_STATE_COUNT:
        if(isPressed)
        {
            _newState(BTN_STATE_DOWN);
            _startTime = timeNow;
        }
        else if ((timeInterval > _clickDelay)) 
        {
            if (_nClicks == 1)
            {
                if (_singleClickFunc) _singleClickFunc();
            }

            else if (_nClicks == 2)
            {
                if (_doubleClickFunc) _doubleClickFunc();
            }

            _reset();
        }
        break;

    case BTN_STATE_PRESS:
        if(!isPressed)
        {
            _newState(BTN_STATE_PRESSEND);
            _startTime = timeNow;
        }
        break;

    case BTN_STATE_PRESSEND:
        if (isPressed && (timeInterval < _debounceDelay))
            _newState(_lastState);
        

    default:
        _newState(BTN_STATE_INIT);
        break;
    }
}