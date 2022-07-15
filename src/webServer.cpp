#include <webServer.h>

AsyncWebServer    server(HTTP_PORT);
AsyncEventSource  events("/events");

// Request Handlers
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


String index_processor(const String& var){
  if(var == "HOST_NAME"){
    return getHostName();
  }
  else if(var == "DEVICE_IP"){
    return WiFi.localIP().toString();
  }
  else if(var == "DHT_T"){
    return String(airTemp());
  }
  else if(var == "DHT_H"){
    return String(airHumidity());
  }
  else if(var == "SOIL_H"){
    return String(soilHumidity());
  }
  return String();
}


String config_processor(const String& var){
  if(var == "MODE"){
    switch (WiFi.getMode())
    {
    case WIFI_STA:
      return "Conectado : Cliente";
      break;
    case WIFI_AP:
      return "Ponto de Acesso";
      break;       
    default:
      return "";
      break;
    }
  }
  else if(var == "SSID"){
    return WiFi.SSID();
  }
    else if(var == "HOSTNAME"){
    return getHostName();
  }
  else if(var == "DEVICE_IP"){
    return WiFi.localIP().toString();
  }
  else if (var == "MAC")
  {
    return WiFi.macAddress();
  }
  else if (var == "AIR_VAL")
  {
    return String(Device_config.air_value);
  }
  else if (var == "WAT_VAL")
  {
    return String(Device_config.wat_value);
  }  
  return String();
}


void setupEvents(){
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.print(F("\n[INFO]: Event Listener client reconnected."));
    }
    client->send("ESP_EVENT", NULL, millis(), 10000);
  });

  server.addHandler(&events);
}


void setupWebServer()
{
  server.serveStatic("/", LittleFS, "/");

  //  *******    INDEX PAGE HANDLERS

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html", false);
  });

  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->redirect("/index.html");
    delay(100);
  }).setFilter(ON_AP_FILTER);

  server.on("/fwlink", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->redirect("/index.html");
    delay(100);
  }).setFilter(ON_AP_FILTER);

  //  *******    CONFIG PAGE HANDLERS

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/config.html", "text/html", false, config_processor);
  });

  server.on("/delete-config", HTTP_GET, [](AsyncWebServerRequest *request){
    if(removeDeviceConfig())
      request->send_P(200, "text/plain", "success");
    request->send_P(400, "text/plain", "error");
  });

  server.on("/delete-wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    if(removeWifiConfig())
      request->send_P(200, "text/plain", "success");
    request->send_P(400, "text/plain", "error");
  });

  server.on("/update-config", HTTP_POST, [] (AsyncWebServerRequest *request) {
    bool ap_mode = false;

    if (request->hasParam("hostname") && request->hasParam("air_v") && request->hasParam("wat_v")) 
    {
      if (request->hasParam("ap_mode"))
      {
        ap_mode = true;
      }

      if (storeDeviceConfig(request->getParam("hostname")->value(), request->getParam("air_v")->value(), \
                            request->getParam("wat_v")->value(), ap_mode))
      {
        request->send_P(200, "text/plain", "success");
      }
      request->send_P(400, "text/plain", "error");
    }
    request->send_P(400, "text/plain", "error");
  });

  //  *******    WIFI PAGE HANDLERS

  server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/wifi.html", "text/html", false);
  }).setFilter(ON_AP_FILTER);

  server.on("/scan-wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", scanNetworks());
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{\"status\": \"" + String(WiFi.status()) + "\", \"network\": \""+ WiFi.SSID() + "\"}");
  });

  server.on("/connect", HTTP_POST, [] (AsyncWebServerRequest *request) {
    bool result = false;

    if (request->hasParam("auto-ip") && request->hasParam("ssid") && request->hasParam("password")) {
      result = connectToWifi(request->getParam("ssid")->value(), request->getParam("password")->value());
    } 
    else if (request->hasParam("ip") && request->hasParam("gateway") && request->hasParam("mask") && request->hasParam("ssid") && request->hasParam("password"))
    {
      setStaticIp(request->getParam("ip")->value(), request->getParam("gateway")->value(), request->getParam("mask")->value());
      result = connectToWifi(request->getParam("ssid")->value(), request->getParam("password")->value());
    }

    if (result)
      request->send(200, "application/json", "{\"status\": \"connected\" }");

    request->send(200, "application/json", "{\"status\": \"error\" }");
  });

  server.onNotFound(notFound);

  setupEvents();

  server.begin();
  Serial.print(F("\n[INFO]: Initializing WebServer..."));
}


void eventsLoop()
{
  events.send(readSensorsJSON().c_str(),"sensor_readings",millis());
}
