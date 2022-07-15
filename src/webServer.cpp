#include <webServer.h>

AsyncWebServer    server(HTTP_PORT);
AsyncEventSource  events("/events");

/*
*     HTML PRE-PROCESSORS
*/
String index_processor(const String& var){
  if(var == "HOST_NAME"){
    return getHostName();
  }
  else if(var == "DEVICE_IP"){
    return getIpAddress();
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
    return getMode();
  }
  else if(var == "SSID"){
    return getSSID();
  }
    else if(var == "HOSTNAME"){
    return getHostName();
  }
  else if(var == "DEVICE_IP"){
    return getIpAddress();
  }
  else if (var == "MAC")
  {
    return getMacAddress();
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

/*
*     REQUEST HANDLERS
*/
void notFoundHandler(AsyncWebServerRequest *request) {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";

  for (uint8_t i = 0; i < request->args(); i++) 
  {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }
  request->send(404, "text/plain", message);
}

void updateConfigHandler(AsyncWebServerRequest *request)
{
  bool ap_mode = false;

  if (request->hasParam("hostname") && request->hasParam("air_v") && request->hasParam("wat_v")) 
  {
    if (request->hasParam("ap_mode"))
      ap_mode = true;

    if (storeDeviceConfig(request->getParam("hostname")->value(), request->getParam("air_v")->value(), \
                          request->getParam("wat_v")->value(), ap_mode))
      request->send_P(200, "text/plain", "success");

    request->send_P(400, "text/plain", "error");
  }

  request->send_P(400, "text/plain", "error");
}

void deleteWifiConfigHandler(AsyncWebServerRequest *request)
{
  if(removeWifiConfig())
    request->send_P(200, "text/plain", "success");

  request->send_P(400, "text/plain", "error");
}

void deleteConfigHandler(AsyncWebServerRequest *request)
{
  if(removeDeviceConfig())
    request->send_P(200, "text/plain", "success");

  request->send_P(400, "text/plain", "error");
}

void connectHandler(AsyncWebServerRequest *request) 
{
  bool result = false;

  if (request->hasParam("auto-ip") && request->hasParam("ssid") && request->hasParam("password")) 
  {
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
}

void serverEventHandler(AsyncEventSourceClient *client)
{
  if(client->lastId())
  {
    Serial.print(F("\n[INFO]: Event Listener client reconnected."));
  }

  client->send("[ESP_EVENT]", NULL, millis(), 10000);
}

void addServerHandlers()
{
  //  *******    INDEX PAGE HANDLERS
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send(LittleFS, "/index.html", "text/html", false);
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html");
    response->addHeader("Content-Encoding", "text/html");
    request->send(LittleFS, "/index.html.gz", "text/html", false);    
  });

  server.on("/teste", HTTP_GET, [](AsyncWebServerRequest *request){
    String message = "WORKING!@";
    request->send(200, "text/plain", message);
  });

  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->redirect("/index.html");
  }).setFilter(ON_AP_FILTER);

  server.on("/fwlink", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->redirect("/index.html");
  }).setFilter(ON_AP_FILTER);

  //  *******    CONFIG PAGE HANDLERS
  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/config.html", "text/html", false, config_processor);
  });

  server.on("/delete-config", HTTP_GET, deleteConfigHandler);

  server.on("/delete-wifi", HTTP_GET, deleteWifiConfigHandler);

  server.on("/update-config", HTTP_POST, updateConfigHandler);

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

  server.on("/connect", HTTP_POST, connectHandler);

  server.onNotFound(notFoundHandler);
}


void setupWebServer()
{
  addServerHandlers();
  events.onConnect(serverEventHandler);
  server.addHandler(&events);  

  Serial.print(F("\n[INFO]: Initializing WebServer..."));
  server.begin();

  server.serveStatic("/", LittleFS, "/");
}


void eventsLoop()
{
  events.send(readSensorsJSON().c_str(),"sensor_readings",millis());
}
