#include <definitions.h>
#include <readSensors.h>
#include <display.h>
#include <wifiConfig.h>
#include <webServer.h>

unsigned long previousMillis = 0;
unsigned long interval;

// GLOBAL VARIABLES
uint32_t            dhtDelay;
int                 dhtTemperature;
int                 dhtHumidity;
int                 soilPercent;

// CREATE INSTANCES
Display                 display;
Dht                     dhtSensor;

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialize DHT
  dhtDelay = dhtSensor.dhtInit();

  // Initialize DISPLAY
  display.Init();

  // Initialize WIFI
  initWifi();

  // Initialize WebServer
  setupRoutes();
}

void loop() {

  //PREPARE LOOP - BASED on DHT min delay or DEFAULT_DELAY
  unsigned long currentMillis = millis();
  interval = dhtDelay || DEFAULT_DELAY;
  
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;

    //READ SENSORS
    soilPercent = readSoilP();
    dhtTemperature = dhtSensor.readTemperature();
    dhtHumidity = dhtSensor.readHumidity();

    //Update Screen
    display.UpdateScreen(dhtTemperature, dhtHumidity, soilPercent);
  }

  dnsProcessNext();
}