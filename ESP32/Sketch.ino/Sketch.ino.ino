#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

// Network credentials
const char* ssid = "ESP32_Plant_Monitor";
const char* password = "12345678";

AsyncWebServer server(80);

// Sensor values
float temperature = 0.0;
float humidity = 0.0;
float soilMoisture = 0.0;
float lightAmount = 0.0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 16, 17); // Adjust pins according to your setup

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Create the Wi-Fi access point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Serve static files
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<200> json;
    json["temperature"] = temperature;
    json["humidity"] = humidity;
    json["soilMoisture"] = soilMoisture;
    json["lightAmount"] = lightAmount;

    String response;
    serializeJson(json, response);
    request->send(200, "application/json", response);
  });

  server.on("/control", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("action", true)) {
      String action = request->getParam("action", true)->value();
      if (action == "water") {
        // Add watering logic
      } else if (action == "stop") {
        // Add stop watering logic
      } else if (action == "auto") {
        // Add auto mode logic
      } else if (action == "manual") {
        // Add manual mode logic
      }
    }
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');
    StaticJsonDocument<200> doc;
    deserializeJson(doc, data);

    temperature = doc["temperature"];
    humidity = doc["humidity"];
    soilMoisture = doc["soilMoisture"];
    lightAmount = doc["lightAmount"];
  }
}
