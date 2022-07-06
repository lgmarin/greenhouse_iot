#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>

// DHT CONFIGURATION
#define DHTTYPE     DHT11
#define DHTPIN      2 //D4
uint32_t            dhtDelay;

DHT_Unified dht(DHTPIN, DHTTYPE);

// Screen CONFIGURATION


void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialize DHT
  dht.begin();
  sensor_t dhtSensor;
  dhtDelay = dhtSensor.min_delay / 1000;


}

void loop() {
  delay(2500);

  // DHT EVENT HANDLING
  sensors_event_t event;

  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}