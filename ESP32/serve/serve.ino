// #include <SSD1306.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include "WebPage.h"


//Initialize /////////////////////////////////

//OLED display.
// SSD1306 display(0x3c, 5, 4);

//Web server.
WebServer server(80);

//I/O pins.
int ledPin = 2;
int wifiSettingsResetPin = 14;

//Wifi AP ssid and password.
const char* ssid = "MyEspWebServerSetupAP";
const char* password = "";

///////////////////////////////////////////////


void setup()
{
  //Initialize display.
  // display.init();
  // display.flipScreenVertically();
  // display.setFont(ArialMT_Plain_10);

  //Display initial AP connection info on the OLED screen.
  DisplayAPInfo(ssid, password, "/");

  //Open a serial port.
  Serial.begin(921600);

  Serial1.begin(9600, SERIAL_8N1, 16, 17); //Serial1 is used to communicate with the PIC.

  //Print the AP info to the serial port.
  Serial.println("SSID: ");
  Serial.print(ssid);
  Serial.println("Password: ");
  Serial.print(password);

  //Initialize WiFiManager
  WiFiManager wm;
  if(digitalRead(wifiSettingsResetPin)) //If reset pin is HIGH.
    wm.resetSettings(); //Reset network settings.

  wm.setDebugOutput(false); //Disable debug info by default.

  //Create the initial AP used for setting the wifi configuration.
  bool response = wm.autoConnect(ssid, password);

  if (!response) {
    Serial.println("Failed to connect. Retrying connection ...");

    //Try again in 5s.
    ESP.restart();
    delay(5000);
  } else {
    Serial.println("Connected to wifi with IP: ");
    String ip = WiFi.localIP().toString();
    Serial.println(WiFi.localIP());

    DisplayAPInfo(ssid, password, ip);
  }

  //Set I/O.
  pinMode(ledPin, OUTPUT);

  //Initialize web server.
  initWebServer();
}
String data = "555"; //global
void loop(){
  server.handleClient();

  

if (Serial1.available()) {
    data = Serial1.readStringUntil('\n');
    parseData(data);
  }
  
  // Your code for other tasks
}


  int temperature = 1;
  int moisture = 2;
  int humidity = 3;
  int light = 4;
  void parseData(String data) {
  int tempIndex = data.indexOf("temp:");
  int moistIndex = data.indexOf("moist:");
  int humidIndex = data.indexOf("humid:");
  int lightIndex = data.indexOf("light:");

  if (tempIndex != -1 && moistIndex != -1 && humidIndex != -1 && lightIndex != -1) {
    String tempStr = data.substring(tempIndex + 5, data.indexOf(',', tempIndex));
    String moistStr = data.substring(moistIndex + 6, data.indexOf(',', moistIndex));
    String humidStr = data.substring(humidIndex + 6, data.indexOf(',', humidIndex));
    String lightStr = data.substring(lightIndex + 6, data.indexOf(',', lightIndex));

    temperature = tempStr.toInt();
    moisture = moistStr.toInt();
    moisture = moisture /10;
    humidity = humidStr.toInt();
    light = lightStr.toInt();
    light = light * 10;
    // humidity = humidity /10;
    
    Serial.println("Received Data:");
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Moisture: ");
    Serial.println(moisture);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Light: ");
    Serial.println(light);
  }
}

inline void DisplayAPInfo(String ssid, String password, String ip) {
//   display.clear();

//   display.setTextAlignment(TEXT_ALIGN_LEFT);
//   display.drawString(0, 0, "SSID: " + ssid);
//   display.drawString(0, 10, "Pass: " + password); 
//   display.drawString(0, 20, "IP: " + ip);

//   display.display(); 

    Serial.println("SSID: " + ssid);
    Serial.println("Pass: " + password);
    Serial.println("IP: " + ip);
    
}


//////////////// Web Server ///////////////////

void initWebServer() {
  //Serve the HTML page to client when they go to the root path.
  server.on("/", HTTP_GET, serveInitialPage);

  //Setup other endpoints.
  server.on("/toggleLED", HTTP_GET, toggleLED);
  server.on("/readSensor", HTTP_POST, readSensor);

  //Start web server.
  server.begin();
}


void serveInitialPage() {
  server.send(200, "text/html", WEBPAGE);
}

void toggleLED() {
  if (server.hasArg("action")) {
    String action = server.arg("action");

    if (action == "water") {
      Serial1.println("1");
      digitalWrite(ledPin, HIGH);
    }

    if (action == "stop") {
      Serial1.println("2");
      digitalWrite(ledPin, LOW);
    }
  }

  server.send(200, "text/plain", ""); //Send web page.
}
void readSensor() {
  // Prepare JSON document
  StaticJsonDocument<200> serJsonDoc;
  serJsonDoc["temperature"] = temperature;
  serJsonDoc["moisture"] = moisture;
  serJsonDoc["humidity"] = humidity;
  serJsonDoc["light"] = light;

  // Serialize JSON to string
  char jsonString[200];
  serializeJson(serJsonDoc, jsonString);

  // Send response
  server.send(200, "application/json", jsonString);
}