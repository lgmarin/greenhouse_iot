#include <DHT.h>
#include <DHT_U.h>

#include <definitions.h>
#include <soilSensor.h>
#include <display.h>

// DHT CONFIGURATION
uint32_t            dhtDelay;
int                 dhtTemperature;
int                 dhtHumidity;
DHT_Unified         dht(DHTPIN, DHTTYPE);

// SOIL MOISTURE CONFIGURATION
int                 airMoist   = 790;   //CALIBRATION NEEDED
int                 waterMoist = 390;   //CAILBRATION NEEDED


// Create Instances
SoilMoistureSensor  soil(SOIL_A, airMoist, waterMoist);
Display             display;

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialize DHT
  dht.begin();
  sensor_t dhtSensor;
  dhtDelay = dhtSensor.min_delay / 1000;
  // END Initialize DHT

  // Initialize DISPLAY
  display.Init();
  // END Initialize DISPLAY  
}

void loop() {
  delay(2500);

  // DHT EVENT HANDLING
  sensors_event_t event;

  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("\n[ERROR]: Error reading temperature!"));
  }
  else {
    dhtTemperature = round(event.temperature);
    Serial.print(F("Temperature: "));
    Serial.print(dhtTemperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("\n[ERROR]: Error reading humidity!"));
  }
  else {
    dhtHumidity = event.relative_humidity;
    Serial.print(F("Humidity: "));
    Serial.print(dhtHumidity);
    Serial.println(F("%"));
  }
  // END DHT EVENT HANDLING  

  // SOIL MOISTURE HANDLING
  Serial.print(F("\nMoisture")); Serial.print(soil.readPercent()); Serial.print("%");
  // END SOIL MOISTURE HANDLING

  display.UpdateScreen();

}