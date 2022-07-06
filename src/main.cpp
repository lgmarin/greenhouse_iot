#include <Arduino.h>
#include <DHT.h>

// DHT CONFIGURATION
#define DHTTYPE     DHT11
#define DHTPIN      2 //D4

DHT dht(DHTPIN, DHTTYPE);

// Screen CONFIGURATION


void setup() {
  Serial.begin(9600);
  delay(500);

  dht.begin();

}

void loop() {
  delay(2500);

  float temperatre = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(humidity) || isnan(temperatre)) {
    Serial.println(F("[ERROR]: Failed to read from DHT sensor!"));
  }
  Serial.print(F("Temp: ")); Serial.print(temperatre); 
  Serial.print(F("Humid: ")); Serial.print(humidity);
}