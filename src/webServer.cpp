#include <webServer.h>

AsyncWebServer server(HTTP_PORT);

// Request Handlers
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setupRoutes()
{
    // Setup LittleFS Routes
    server.serveStatic("/", LittleFS, "/");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/index.html", "text/html", false);
    });

    server.on("/connect-wifi", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/wifi.html", "text/html", false);
    }).setFilter(ON_AP_FILTER);

    server.on("/scan-wifi", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "application/json", scanNetworks());
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "application/json", "{\"status\": \"" + String(WiFi.status()) + "\", \"network\": \""+ WiFi.SSID() + "\"}");
    });

    server.on("/connect", HTTP_GET, [] (AsyncWebServerRequest *request) {
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
}