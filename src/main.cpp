#include <definitions.h>
#include <soilSensor.h>
#include <display.h>

// DHT CONFIGURATION

// Create Instances
uint32_t            dhtDelay;            

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
  // DHT EVENT HANDLING
  // END DHT EVENT HANDLING  

  // SOIL MOISTURE HANDLING
  // END SOIL MOISTURE HANDLING

  //display.UpdateScreen(dhtDelay);
}