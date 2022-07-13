#include <definitions.h>
#include <configManager.h>
#include <readSensors.h>
#include <display.h>
#include <wifiConfig.h>
#include <webServer.h>

// Prepare loop variables
unsigned long previousMillis = 0;
unsigned long interval;

// CREATE INSTANCES
Display                 display;

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialize FS (Device Configuration Manager)
  initDeviceConfiguration();

  // Initialize Sensors
  initSensors();

  // Initialize DISPLAY
  display.Init();

  // Initialize WIFI
  initWifi(Device_config.ap_mode);

  // Initialize WebServer
  setupRoutes();
}

void loop() {

  //PREPARE LOOP - BASED on DHT min delay or DEFAULT_DELAY
  unsigned long currentMillis = millis();
  interval = dhtDelay() || DEFAULT_DELAY;
  
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;

    // RUN SENSORS UPDATE LOOP
    sensorsLoop();
    
    //Update Screen
    display.UpdateScreen(airTemp(), airHumidity(), soilHumidity(), "TESTE");
  }

  dnsProcessNext();
}