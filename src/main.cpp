#include <definitions.h>
#include <readSensors.h>
#include <display.h>

// GLOBAL VARIABLES
extern uint32_t            dhtDelay;

// CREATE INSTANCES
Display                    display;
Dht                        dhtSensor;

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialize DHT
  dhtDelay = dhtSensor.dhtInit();
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