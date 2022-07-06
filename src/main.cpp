#include <Arduino.h>
#include <DHT.h>

// DHT CONFIGURATION
#define DHTTYPE     DHT11
#define DHTPIN      2 //D4

DHT dht(DHTPIN, DHTTYPE);

// Screen CONFIGURATION


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}