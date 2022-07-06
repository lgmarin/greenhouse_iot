#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// DHT CONFIGURATION
#define DHTTYPE     DHT11
#define DHTPIN      2 //D4
uint32_t            dhtDelay;
int                 dhtTemperature;
int                 dhtHumidity;
DHT_Unified         dht(DHTPIN, DHTTYPE);

// SOIL MOISTURE CONFIGURATION
#define SOIL_A      A0
int                 moistPercent = 0;
int                 moistValue = 0;
int                 airMoist = 790;   //CALIBRATION NEEDED
int                 waterMoist = 390; //CAILBRATION NEEDED

// DISPLAY CONFIGURATION
#define SCREEN_W    128
#define SCREEN_H    64
// PINOUT I2C
// SDA    GPIO4 - D2
// SCK    GPIO5 - D1
Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialize DHT
  dht.begin();
  sensor_t dhtSensor;
  dhtDelay = dhtSensor.min_delay / 1000;
  // END Initialize DHT

  // Initialize DISPLAY
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.print(F("\n[ERROR]: SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
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
  moistValue = analogRead(SOIL_A);
  moistPercent = map(moistValue, airMoist, waterMoist, 0, 100);

  if(moistPercent > 100)
  {
    Serial.print(F("\nMoisture")); Serial.print("99 %");
  }
  else if(moistPercent <0)
  {
    Serial.print(F("\nMoisture")); Serial.print("0 %");
  }
  else if(moistPercent >=0 && moistPercent < 100)
  {
    Serial.print(F("\nMoisture")); Serial.print(moistPercent); Serial.print("%");
  }  
  // END SOIL MOISTURE HANDLING

}