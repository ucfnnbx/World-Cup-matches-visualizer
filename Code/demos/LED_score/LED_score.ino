#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

#include <FastLED.h>
#include <Servo.h>
#define LED_PIN 14
#define NUM_LEDS 24

CRGB leds[NUM_LEDS];
Servo myservo;

int a = 0;
int b = 0;
int pos;
String current = "false";
String used = "false";

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;

ESP8266WebServer server(80);
const char* mqtt_server = "mqtt.cetools.org";
WiFiClient espClient;            //handle wifi messages
PubSubClient client(espClient);  //handle MQTT messages, pass wificlient to connect
StaticJsonDocument<200> doc;     // Allocate the JSON document

void setup() {

  Serial.begin(115200);
  delay(100);

  startWifi();
  startWebserver();

  // start MQTT server
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);

  myservo.attach(12);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}



void startWifi() {
  // connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // when not connected keep trying until you are
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Exit the while loop means have a connection
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address of Huzzah
}

void startWebserver() {
  // when connected and IP address obtained start HTTP server
  server.begin();
  Serial.println("HTTP server started");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  StaticJsonDocument<1024> doc;  // Allocate the JSON document
  String myString = String((char*)payload);
  //char json[length] = myString;

  deserializeJson(doc, myString);  // Deserialize the JSON document

  // Fetch values.
  int home_score = doc["home_score"];
  int away_score = doc["away_score"];
  String home_team_en = doc["home_team_en"];
  String away_team_en = doc["away_team_en"];
  String finished = doc["finished"];
  String datatime = doc["datetime"];

  a = home_score;
  b = away_score;
  current = finished;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect with clientID, username and password
    if (client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      client.subscribe("student/ucfnnbx/whatscup/match");  //subscribe to the topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}





void LEDgoals1() {
  Serial.println("yy6");
  FastLED.clearData();
  if (a > 0) {
    //FastLED.clearData();
    Serial.println("yy7");
    // fill_solid(leds, a, CRGB::Black);
    for (int i = 0; i <= a -1; i++) {
      Serial.println("yy8");
      leds[i] = CRGB(100, 100, 254);
      Serial.println(i);
      FastLED.setBrightness(10);
      FastLED.show();
      delay(200);
    }
  }
  // if (a == 0) {
  //   Serial.println("yy9");
  //   //FastLED.clearData();
  //   for (int i = 0; i <= 14; i++) {
  //     Serial.println("yy10");
  //     leds[i] = CRGB(0, 0, 0);
  //     FastLED.show();
  //     delay(200);
   // }
  //}
}
void LEDgoals2() {
  FastLED.clearData();
  if (b > 0) {
    //FastLED.clearData();
    for (int x = 15; x <= 14 + b; x++) {
      leds[x] = CRGB(255, 100, 100);
      FastLED.setBrightness(10);
      FastLED.show();
      delay(200);
    }
  }

  // if (b == 0) {
  //   //FastLED.clearData();
  //   for (int x = 15; x <= 24; x++) {
  //     //change 24 when create the final version
  //     leds[x] = CRGB(0, 0, 0);
  //     delay(200);
  //     FastLED.show();
  //   }
  // }
}

void servoFlag1() {
  myservo.write(0);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo.write(0);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo.write(0);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo.write(45);
}


void servoFlag2() {
  myservo.write(0);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo.write(0);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo.write(0);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo.write(135);
}


void loop() {

  // handler for receiving requests to webserver
  server.handleClient();      //?
  if (!client.connected()) {  //check if connected to MQTT
    reconnect();
  }

  client.loop();
  Serial.println("yy1");
  Serial.println(a);

  if ((current != used) && (a < b)) {
    servoFlag1();
    used = current;
    delay(1000);
    Serial.println("yy2");
    //update the used one into new one
  }
  if ((current != used) && (a > b)) {
    servoFlag2();
    used = current;
    delay(1000);
    Serial.println("yy3");
    Serial.println(a);
    //update the used one into new one
  }
  if ((current != used) && (a == b)) {
    myservo.write(0);
    used = current;
    delay(1000);
    Serial.println("yy4");
    //update the used one into new one
  }
  Serial.println("yy5");
  delay(1000);
  LEDgoals1();

  LEDgoals2();
  Serial.println("yy11");
}