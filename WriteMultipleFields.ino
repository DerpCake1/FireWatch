/*
  WriteMultipleFields
  
  Description: Writes values to fields 1,2,3,4 and status in a single ThingSpeak update every 20 seconds.
  
  Hardware: ESP32 based boards
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires installation of EPS32 core. See https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md for details. 
  - Select the target hardware from the Tools->Board menu
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/

#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <DHT.h>
#include <DHT_U.h>
#include <HTTPClient.h>
#include <Time.h>

#define DHTPIN 5
#define DHTTYPE DHT11
#define ENCODER_SW 18
#define ENCODER_DT 19
#define ENCODER_CLK 21
#define ENCODER_ROTS 32
#define TSR 0.07925

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
float temperature;
float humidity;
float avg_wind_speed;

volatile int encoderPos = 0; // Current position of the encoder
volatile bool encoderSwitchPressed = false; // Encoder switch state
int absoluteEncoderClicks = 0;

int startTime = 0;

void IRAM_ATTR encoderISR() {
  static bool lastCLK = LOW;
  bool currentCLK = digitalRead(ENCODER_CLK);

  // Check if the encoder is rotating
  if (lastCLK == LOW && currentCLK == HIGH) {
    if (digitalRead(ENCODER_DT) == HIGH) {
      encoderPos++;
    } else {
      encoderPos--;
    }
    absoluteEncoderClicks++;
  }
  lastCLK = currentCLK;
}

void IRAM_ATTR switchISR() {
  encoderSwitchPressed = !encoderSwitchPressed;
}

void setup() {
  Serial.begin(9600);  //Initialize serial
  dht.begin();
  
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  connect_to_wifi();

  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_SW, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_SW), switchISR, FALLING);
}

void loop() {
  connect_to_wifi();
  
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error when reading out the sensor");
    return;
  }
  // Print encoder position
  Serial.print("Encoder Position: ");
  Serial.println(encoderPos);

  float vel = calculateWindSpeed(startTime, millis(), absoluteEncoderClicks);
  startTime = millis();
  absoluteEncoderClicks = 0;

  // Print switch status
  if (encoderSwitchPressed) {
    Serial.println("Switch Pressed");
  } else {
    Serial.println("Switch Not Pressed");
  }

  Serial.print("Temperature (ºF): ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Wind Speed (mph): ");
  Serial.println(vel);
  // set the fields with the values
  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperature);
  ThingSpeak.setField(3, vel);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
    
  delay(20000); // Wait 20 seconds to update the channel again
}

void connect_to_wifi(){
// Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.disconnect();

      WiFi.mode(WIFI_STA);
      WiFi.enableSTA(true);

      WiFi.begin(SECRET_SSID, SECRET_PASS);
      WiFi.reconnect();
      delay(10000);
    }
  }
}

float calculateWindSpeed(int starttime, int currentTime, int encoderclicks){
  float t = (float)(currentTime - starttime) / 1000.0;
  float deg = 6.28 / (float) ENCODER_ROTS * (float) encoderclicks;

  float angular = deg / (float)t;
  float speed = 5.0f * angular / TSR;

  speed = speed / 12.0 / 5280.0 * 60.0 * 60.0;

  return speed;
}
