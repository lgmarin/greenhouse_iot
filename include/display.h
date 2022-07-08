#ifndef display_h
#define display_h

#include <definitions.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display{
private:
    
public:
    Display();
    ~Display();
    void UpdateScreen();
};

#endif