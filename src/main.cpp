#include <definitions.h>
#include <soilSensor.h>
#include <display.h>

// DHT CONFIGURATION
uint32_t            dhtDelay;
int                 dhtTemperature;
int                 dhtHumidity;


// Create Instances
Display             display;

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialize DHT

  // END Initialize DHT

  // Initialize DISPLAY
  display.Init();
  // END Initialize DISPLAY  
}

void loop() {
  delay(2500);

  // DHT EVENT HANDLING
  // END DHT EVENT HANDLING  

  // SOIL MOISTURE HANDLING
  // END SOIL MOISTURE HANDLING

  display.UpdateScreen();
}