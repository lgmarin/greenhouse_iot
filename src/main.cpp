#include <definitions.h>
#include <configManager.h>
#include <readSensors.h>
#include <display.h>
#include <wifiConfig.h>
#include <webServer.h>

// Prepare loop variables
unsigned long previous_millis = 0;
unsigned long sensors_interval;
unsigned long sleep_interval;

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
  setupWebServer();
}

void loop() {

  //PREPARE LOOP - BASED on DHT min delay or DEFAULT_DELAY
  unsigned long current_millis = millis();
  sensors_interval = dhtDelay() || DEFAULT_DELAY;
  
  if (current_millis - previous_millis >= sensors_interval) 
  {
    previous_millis = current_millis;

    // RUN SENSORS UPDATE LOOP
    sensorsLoop();

    // EventListenner Loop
    // eventsLoop();

    //Update Screen
    display.UpdateScreen(airTemp(), airHumidity(), soilHumidity(), wifiInfo());

  }

  if (digitalRead(WAKE_BTN_PIN))
    display.Wake(current_millis);

  
  display.Sleep(current_millis);

  dnsProcessNext();
  updatemDNS();
}