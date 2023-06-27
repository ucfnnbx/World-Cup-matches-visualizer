#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

#include <SPI.h>
#include "LCD_Driver.h"
#include "LCD_GUI_Paint.h"
#include "LCD_Images.h"

#include <FastLED.h>
#include <Servo.h>
#define LED_PIN     14
#define NUM_LEDS    16

CRGB leds[NUM_LEDS];
Servo myservo;

int pos;

const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;

const char* mqtt_server = "mqtt.cetools.org";
WiFiClient espClient;//handle wifi messages
PubSubClient client(espClient);//handle MQTT messages, pass wificlient to connect
//StaticJsonDocument<200> doc;// Allocate the JSON document

void setup() {
  Serial.begin(115200);
  delay(100);
  initialiseLCDScreen();
  //startWifi(); 

  // start MQTT server
  //client.setServer(mqtt_server, 1884);
  //client.setCallback(callback);

  myservo.attach(12);

}  

void loop() {
  
  String response = "{\"home_score\": 6, \"away_score\": 5,\"home_team_en\": \"Ecuador\", \"away_team_en\": \"Netherlands\", \"finished\": \"false\", \"datetime\": \"2022-11-21 13:02\"}";

  StaticJsonDocument<1024> doc;// Allocate the JSON document
  
  deserializeJson(doc, response); // Deserialize the JSON document

  // Fetch values.
  String home_score_str = doc["home_score"];
  int home_score = home_score_str.toInt(); 
  String away_score_str = doc["away_score"];
  int away_score = away_score_str.toInt(); 

  String home_team_en = doc["home_team_en"];
  String away_team_en = doc["away_team_en"];  
  String finished = doc["finished"];
  String datatime = doc["datetime"];

  initialiseLCDScreen();
  delay(200);
  LCD_DrawFlags(home_team_en, away_team_en);
  SPI.end();

  delay(200);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  NeoPixelsDisplayLeft(home_score);
  NeoPixelsDisplayRight(away_score);

  if(finished == "true"){
    servoFlag1();
    servoFlag2();
  }
  delay(5000);
//  if (!client.connected()) {//check if connected to MQTT
//    reconnect();
//  }
  
//When a new sensor reading is received from Arduino, it will be parsed and 
  //sent over MQTT, then the LCD screen is updated accordingly
 
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
  Serial.println(WiFi.localIP()); //IP address of Huzzah
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  StaticJsonDocument<1024> doc;// Allocate the JSON document
  String myString = String((char*)payload);
  
  deserializeJson(doc, myString); // Deserialize the JSON document

  // Fetch values.
  int home_score = doc["home_score"]; 
  int away_score = doc["away_score"];
  String home_team_en = doc["home_team_en"];
  String away_team_en = doc["away_team_en"];  
  String finished = doc["finished"];
  String datatime = doc["datetime"];

  
  //Paint_DrawImage(gImage_England, 0, 0, 120, 240);
  //Paint_DrawImage(gImage_England, 120, 0, 120, 240);
  //drawFlagOnScreen(home_team_en, away_team_en);
    
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
      client.subscribe("student/ucfnnbx/whatscup/matches");//subscribe to the topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void NeoPixelsDisplayLeft(int score){
  if (score>0){
  for (int i = 0; i <= score-1; i++) {
    leds[i] = CRGB ( 0, 0, 254);
    FastLED.setBrightness(90);
    FastLED.show();
    delay(200);
  }
  }
  else{
      for (int i = 0; i <= 8; i++) {
    leds[i] = CRGB ( 0, 0, 0);
    FastLED.show();
    delay(200);
 }
  }
}

void NeoPixelsDisplayRight(int score){
 if (score>0){
  for (int x = 8; x < 8+score; x++) {
    leds[x] = CRGB ( 255, 0, 0);
    FastLED.setBrightness(90);
    FastLED.show();
  delay(200);
  }
 }
 else{
   for (int x = 8; x < 16; x++) {
     //change 24 when create the final version
    leds[x] = CRGB ( 0, 0, 0);
    FastLED.show();
    delay(200); 
  }
 }
}

void servoFlag1(){
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


void servoFlag2(){
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




void initialiseLCDScreen(){
  Config_Init();
  LCD_Init();
  
  LCD_SetBacklight(1000);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
  Paint_Clear(BLACK);
}


void LCD_DrawFlags(String home_flag, String away_flag){
    const int COUNTRY_NUM = 4;
    
    const unsigned char** flagImagesArray = new const unsigned char*[COUNTRY_NUM];
    flagImagesArray[0] = gImage_Brazil;
    flagImagesArray[1] = gImage_Netherlands;
    flagImagesArray[2] = gImage_Senegal;
    flagImagesArray[3] = gImage_Ecuador;
    
    String countryNamesArray[] = { "Brazil", "Netherlands", "Senegal", "Ecuador" };

    int home_index = 31;
    int away_index = 31;
    
    for(int i = 0; i < COUNTRY_NUM; ++i)
    {
      Serial.println("i=");
      Serial.println(i);
      if(countryNamesArray[i] == home_flag){
        home_index = i;
      }
      if(countryNamesArray[i] == away_flag){
        away_index = i;
      }
    }
    Serial.println("home_index = ");
    Serial.println(home_index);

    Serial.println("away_index = ");
    Serial.println(away_index);
    
    Paint_DrawImage(flagImagesArray[home_index], 0, 0, 120, 240);
    Paint_DrawImage(flagImagesArray[away_index], 120, 0, 120, 240);
    }
