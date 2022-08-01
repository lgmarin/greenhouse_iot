#include <webServer.h>

AsyncWebServer    server(HTTP_PORT);

/*
*     HTML PRE-PROCESSORS
*/
String indexProcessor(const String& var){
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
  else if(var == "CONN_STATUS"){
    if (getMode() == "Conectado")
    {
      return "fc1505";
    }
    return "000000";
  }
  return String();
}

String configProcessor(const String& var){
  if(var == "MODE"){
    return getMode();
  }
  else if(var == "SSID"){
    return getSSID();
  }
    else if(var == "HOST_NAME"){
    return getHostName();
  }
  else if(var == "DEVICE_IP"){
    return getIpAddress();
  }
  else if (var == "MAC")
  {
    return getMacAddress();
  }
  else if (var == "AP_MODE")
    if(Device_config.ap_mode)
    {
      return "checked";
    } 
    else
    {
      return "";
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

String wifiProcessor(const String& var){
  if(var == "HOST_NAME"){
    return getHostName();
  }
  else if(var == "DEVICE_IP"){
    return getIpAddress();
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
      //request->send_P(200, "text/plain", "success");
      request->redirect("/config");

    request->send_P(400, "text/plain", "error");
  }

  request->send_P(400, "text/plain", "error");
}

void deleteWifiConfigHandler(AsyncWebServerRequest *request)
{
  if(removeWifiConfig())
    //request->send_P(200, "text/plain", "success");
    request->redirect("/config");

  request->send_P(400, "text/plain", "error");
}

void deleteConfigHandler(AsyncWebServerRequest *request)
{
  if(removeDeviceConfig())
    //request->send_P(200, "text/plain", "success");
    request->redirect("/config");

  request->send_P(400, "text/plain", "error");
}

void connectHandler(AsyncWebServerRequest *request) 
{
  bool result = false;

  if (request->hasParam("auto-ip") && request->hasParam("wifi_ssid") && request->hasParam("wifi_password")) 
  {
    result = connectToWifi(request->getParam("wifi_ssid")->value(), request->getParam("wifi_password")->value());
  } 
  else if (request->hasParam("ip") && request->hasParam("gateway") && request->hasParam("mask") && request->hasParam("wifi_ssid") && request->hasParam("wifi_password"))
  {
    setStaticIp(request->getParam("ip")->value(), request->getParam("gateway")->value(), request->getParam("mask")->value());
    result = connectToWifi(request->getParam("wifi_ssid")->value(), request->getParam("wifi_password")->value());
  }

  if (result)
    request->send(200, "application/json", "{\"status\": \"connected\" }");

  request->send(400, "application/json", "{\"status\": \"error\" }");
}

void saveWifiHandler(AsyncWebServerRequest *request)
{
  bool result = false;

  if (request->hasParam("auto-ip") && request->hasParam("wifi_ssid") && request->hasParam("wifi_password")) 
  {
    result = saveWifiCredentials(request->getParam("wifi_ssid")->value(), request->getParam("wifi_password")->value());
  } 
  else if (request->hasParam("ip") && request->hasParam("gateway") && request->hasParam("mask") && request->hasParam("wifi_ssid") && request->hasParam("wifi_password"))
  {
    result = saveWifiCredentials(request->getParam("wifi_ssid")->value(), request->getParam("wifi_password")->value(), false, \
                                 request->getParam("ip")->value(), request->getParam("gateway")->value(), request->getParam("mask")->value());
  }

  if (result)
    //request->send(200, "application/json", "{\"status\": \"saved\" }");
    request->redirect("/");

  request->send(400, "application/json", "{\"status\": \"error\" }");  
}

void addServerHandlers()
{
  //  *******    INDEX PAGE HANDLERS

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse* response = request->beginResponse(LittleFS, "/index.html.gz", "text/html", false, indexProcessor);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
    //request->send(LittleFS, "/index.html", "text/html", false, indexProcessor);
  });

  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->redirect("/index.html");
  }).setFilter(ON_AP_FILTER);

  server.on("/fwlink", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->redirect("/index.html");
  }).setFilter(ON_AP_FILTER);

  server.on("/read-sensors", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", readSensorsJSON());
  });


  //  *******    CONFIG PAGE HANDLERS

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse* response = request->beginResponse(LittleFS, "/config.html.gz", "text/html", false, configProcessor);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);    
    //request->send(LittleFS, "/config.html", "text/html", false, configProcessor);
  });

  server.on("/delete-config", HTTP_GET, deleteConfigHandler);

  server.on("/delete-wifi", HTTP_GET, deleteWifiConfigHandler);

  server.on("/update-config", HTTP_GET, updateConfigHandler);

  //  *******    WIFI PAGE HANDLERS
  
  server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse* response = request->beginResponse(LittleFS, "/wifi.html.gz", "text/html", false, wifiProcessor);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);    
    //request->send(LittleFS, "/wifi.html", "text/html", false, wifiProcessor);
  });

  server.on("/scan-wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", scanNetworks());
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{\"status\": \"" + String(WiFi.status()) + "\", \"network\": \""+ WiFi.SSID() + "\"}");
  });

  server.on("/connect-wifi", HTTP_GET, connectHandler);

  server.on("/save-wifi", HTTP_GET, saveWifiHandler);

  server.onNotFound(notFoundHandler);
}


void setupWebServer()
{
  addServerHandlers();

  Serial.print(F("\n[INFO]: Initializing WebServer..."));
  server.begin();

  server.serveStatic("/", LittleFS, "/");
}