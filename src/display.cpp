#include <display.h>

/*!
 *  @brief  Initialize SSD1306 Display.
 */
void Display::init()
{
    if(!_display.begin(SSD1306_SWITCHCAPVCC, D_I2C_ADDR)) {
        Serial.print(F("\n[ERROR]: SSD1306 allocation failed!"));
        for(;;);
    }
    delay(2000);
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setFont(NULL);
    _display.setCursor(0, 0);
    _display.setTextColor(WHITE);
    _display.print("Initializing...");
}

/*!
 *  @brief  Update the display screen for loop().
 */
void Display::updateDisplay()
{
    if(_sleeping)
        return;
    
    sleep();

    switch (_activeScreen)
    {
    case MAIN_SCREEN:
        _mainScreen();
        break;
    case CALIB_SCREEN:
        _calibrationScreen();
        break;
    case WIFI_SCREEN:
        _wifiScreen();
        break;
    case RUN_CALIB_SCREEN:
        _runningCalibrationScreen();
        break;
    case FINISH_SCREEN:
        _calibrationFinishScreen();
        break;        
    default:
        _mainScreen();
        break;
    }
}

void Display::changeScreen(uint8_t screen)
{
    _activeScreen = screen;
}

void Display::nextScreen()
{
    int currentScreen = _activeScreen;
    if (currentScreen += 1 > _totalScreens - 1)
        _activeScreen = 0;
    else 
        _activeScreen += 1;
}

void Display::sleep()
{
    if(_sleepBlocked)
        return;

    unsigned long current_millis = millis();

    if (current_millis - _previous_millis >= SLEEP_INTERVAL) 
    {
        changeScreen(MAIN_SCREEN);
        _previous_millis = current_millis;
        _display.ssd1306_command(SSD1306_DISPLAYOFF);
        _sleeping = true;
    }
}

void Display::lockSleepMode()
{
    _sleepBlocked = true;
}

void Display::unlockSleepMode()
{
    _sleepBlocked = false;
}

bool Display::wake()
{
    if (_sleeping) 
    {
        _previous_millis = millis();
        _display.ssd1306_command(SSD1306_DISPLAYON);
        _sleeping = false;
        return false;
    }

    return true;
}


void Display::btnHandleClick()
{
    if(_sleeping)
        wake();
 
    else if (!_isCallibrationRunning)
    {
        nextScreen();
        _previous_millis = millis();
    }
    else
    {
        switch (_activeScreen)
        {
        case RUN_CALIB_SCREEN:
            if(_currentCalibState == AIR_CALIB || _currentCalibState == WAT_CALIB)
                break;

            _callibrationHelper();
            _currentCalibState = _nextCalibState;
            break;

        case FINISH_SCREEN:
            changeScreen(MAIN_SCREEN);
        
        default:
            break;
        }
    }
}

void Display::btnHandleLongPress()
{
    if(_sleeping)
       return;

    switch (_activeScreen)
    {
    case MAIN_SCREEN:
        break;
    case WIFI_SCREEN:
        //Do Wifi stuff
        
        break;
    case CALIB_SCREEN:
        //Do calibration stuff
        if(_currentCalibState == NOT_STARTED && !_isCallibrationRunning)
        {
            _currentCalibState = _nextCalibState;
            _callibrationHelper();
        }
            
        break;
    case RUN_CALIB_SCREEN:
        //Return to CALIB_SCREEN
        _activeScreen = CALIB_SCREEN;
        break;
    default:
        break;
    }    
}


/*
    SCREENS DRAWING FUNCTIONS
*/
void Display::_beginScreenDraw()
{
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setFont(NULL);
    _display.setTextColor(WHITE);
    _display.setCursor(0, 0);
}

void Display::_mainScreen()
{
    _beginScreenDraw();
    // Information    
    _display.setCursor(0, 0);
    _display.print("WIFI-");
    _display.print(wifiInfo());
    // Temperature
    _display.setCursor(0, 16);
    _display.setTextSize(1);
    _display.print("Ta ");
    _display.setTextSize(2);
    _display.print(airTemp());
    _display.setTextSize(1);
    _display.print("o");
    _display.setTextSize(2);
    _display.print("C ");
    // Humidity
    _display.setTextSize(1);
    _display.print("Ua ");
    _display.setTextSize(2);
    _display.print(airHumidity());
    _display.print("%");
    // Soil Humidity
    _display.setCursor(0, 36);
    _display.setTextSize(1);
    _display.print("Us ");
    _display.setTextSize(2);
    _display.print(soilHumidity());
    _display.print("%");
    _display.display();
}

void Display::_wifiScreen()
{
    _beginScreenDraw();
    _drawCenterText("WIFI", 0);
    _display.setCursor(0, 10);
    _display.drawLine(1, 10, 127, 10, 1);
    _display.setCursor(1, 13);
    _display.print("Modo: ");
    _display.setCursor(32, 13);
    if (wifiMode() == 2)
    {
        _display.print("STA"); //MDOE
        _display.setCursor(1, 23);
        _display.print("Rede:");
        _display.setCursor(32, 23);
        _display.print(getHostName()); //WIFINAME
        _display.setCursor(0, 10);
        _display.setCursor(1, 33);
        _display.print("IP:");
        _display.setCursor(32, 33);
        _display.print(getIpAddress()); //IP ADDRESS
    } else if(wifiMode() == 1 || wifiMode() == 3)
    {
        _display.print("AP"); //MDOE
        _display.setCursor(80, 13);
        _display.print("S: 100%"); //WIFI Strength
        _display.setCursor(1, 23);
        _display.print("Rede:");
        _display.setCursor(32, 23);
        _display.print(getHostName()); //WIFINAME
        _display.setCursor(0, 10);
        _display.setCursor(1, 33);
        _display.print("IP:");
        _display.setCursor(32, 33);
        _display.print(getIpAddress()); //IP ADDRESS
        _display.setCursor(0, 50);
        _drawCenterText(">> Resetar Config. <<", 54); //Remover se AP
    }
    _display.display();
}

void Display::_calibrationScreen()
{
    _beginScreenDraw();
    _drawCenterText("UMIDADE DO SOLO", 0);
    _display.setCursor(0, 10);
    _display.drawLine(1, 10, 127, 10, 1);
    _display.setCursor(1, 13);
    _display.println("Umidade:");
    _display.setCursor(50, 13);
    _display.print("100%");
    _drawCenterText(">> Calibrar Sensor <<", 54);
    _display.display();
}

void Display::_runningCalibrationScreen()
{
    _beginScreenDraw();
    _drawCenterText("CALIBRANDO SENSOR", 0);
    _display.setCursor(0, 10);
    _display.drawLine(1, 10, 127, 10, 1);
    _display.setCursor(1, 13);

    if(_currentCalibState == AIR_CALIB || _currentCalibState == START_AIR)
    {
        _display.println("MEIO: AR");
        _drawCenterText("Mantenha o Sensor", 26);
        _drawCenterText("SECO", 35);
    } 
    else if(_currentCalibState == WAT_CALIB || _currentCalibState == START_WAT)
    {
        _display.println("MEIO: AGUA");
        _drawCenterText("Mantenha o Submerso", 26);
        _drawCenterText("SECO", 35);
    }

    if(_currentCalibState == AIR_CALIB || _currentCalibState == WAT_CALIB)
        _drawCenterText("AGUARDE", 54);
    else
        _drawCenterText("> Iniciar <", 54);

    _display.display();
}

void Display::_calibrationFinishScreen()
{
    _beginScreenDraw();
    _drawCenterText("CALIBRANDO SENSOR", 0);
    _display.setCursor(0, 10);
    _display.drawLine(1, 10, 127, 10, 1);
    _display.setCursor(1, 13);
    _display.println("Umidade:");
    _display.setCursor(50, 13);
    _display.print("100%");

    if(_currentCalibState == CALIB_SUCCESS)
        _drawCenterText("SUCESSO!", 35);
    else
        _drawCenterText("FALHOU!", 35);

    _drawCenterText("> Voltar <", 54);
    _display.display();
}

void Display::_drawCenterText(const char *buf, uint8_t y)
{
    int16_t x1, y1;
    uint16_t w, h;
    _display.getTextBounds(buf, 0, y, &x1, &y1, &w, &h);
    _display.setCursor((SCREEN_W - w) / 2, y);
    _display.print(buf);
}

void Display::_callibrationHelper()
{
    if(_currentCalibState == NOT_STARTED)
        return;
        
    switch (_currentCalibState)
    {
    case START_AIR:
        _nextCalibState = AIR_CALIB;
        lockSleepMode();
        changeScreen(RUN_CALIB_SCREEN);
        break;
    
    case AIR_CALIB:
        _nextCalibState = START_WAT;
        break;

    case START_WAT:
        _nextCalibState = WAT_CALIB;
        break;
    
    case WAT_CALIB:
        _nextCalibState = CALIB_SUCCESS;
        changeScreen(FINISH_SCREEN);
        break;

    case CALIB_SUCCESS:
        _nextCalibState = NOT_STARTED;
        changeScreen(MAIN_SCREEN);
        break;

    case CALIB_FAIL:
        _nextCalibState = NOT_STARTED;
        changeScreen(MAIN_SCREEN);
        break;        

    default:
        break;
    }
}
