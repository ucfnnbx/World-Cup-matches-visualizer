//Code for ESP8266:
#include <ESP8266WiFi.h> //ESP8266 Library
#include <ESP8266HTTPClient.h> //ESP8266 Library
#include <ArduinoJson.h> //For phrasing JSON file download from https://github.com/bblanchon/ArduinoJson
 
const char* ssid = "anlydia"; //Enter your Wi-Fi SSID
const char* password = "anyining"; //Enter you Wi-Fi Password

String payload; //To store the JSON object as string
WiFiClient wifiClient;

void setup () {
 
  Serial.begin(115200); //initialise serial monitor to send data to Arduino
  WiFi.begin(ssid, password); //connect to the network specified above
 
  while (WiFi.status() != WL_CONNECTED) { //Wait till Wi-Fi is connected
    delay(1000);
    Serial.print("Connecting.."); //Print Connecting.. till connection is established  
  }
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) { //If Wi-Fi connected successfully 
    HTTPClient http;  //start a HTTPClinet as http 
    //####DO NOT USE THE SAME API as below
    http.begin(wifiClient, "http://api.cup2022.ir/api/v1/match");  //Enter our API 
    
    int httpCode = http.GET(); //pass a get request                                                                  
 
    if (httpCode > 0) { //Check the returning code
 
      payload = http.getString();   // Store the value on varibale Payload for debugging
      Serial.println(payload);   //Print the payload for debugging otherwise comment both lines
  }}}