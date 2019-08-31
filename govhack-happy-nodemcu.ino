#include <Arduino.h>
#include <ReadinessIoWebsockets.h>
#include <ESP8266WiFi.h>
#include "config.h"

const uint8_t  LED_PIN   = LED_BUILTIN;  // The GPIO that your status LED is attached to
const int      RELAY_PIN = D6;           // The GPIO that your relay is attached to
const uint16_t BAUD      = 9600;         // Debugging serial baud rate

const uint16_t TURN_ON_AFTER = 10000;    // How long to wait before turning LED on

const uint8_t  HAPPY_LED_PIN = 16;
const uint8_t  NEUTRAL_LED_PIN = 5;
const uint8_t  SAD_LED_PIN = 4;

ReadinessIoWebsockets ws(SENSOR_ID,CHANNEL_ID,TOPIC,VERSION,FORMAT);

void wsCallback(WStype_t type, uint8_t * payload, size_t length) {

  if(type == WStype_DISCONNECTED){
      Serial.printf("[WSc] Disconnected\n");
  } else if(type == WStype_CONNECTED){
      Serial.printf("[WSc] Connected to url: %s\n", payload);
  } else if(type == WStype_TEXT){
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload);
      String reading = root["mood"];
      logic(reading);
   }
}

void logic(String mood) {
  resetLEDs();
  
  if(mood == "sad"){
    digitalWrite(SAD_LED_PIN, HIGH);
    Serial.println("Received sad");
  }
  if(mood == "neutral"){
    digitalWrite(NEUTRAL_LED_PIN, HIGH);  
    Serial.println("Received neutral");
  }
  if(mood == "happy"){
    digitalWrite(HAPPY_LED_PIN, HIGH);  
    Serial.println("Received happy");
  }
}

void resetLEDs() {
  digitalWrite(SAD_LED_PIN, LOW);
  digitalWrite(NEUTRAL_LED_PIN, LOW);
  digitalWrite(HAPPY_LED_PIN, LOW);
}

void setup() {
  delay(1000);
  Serial.begin(BAUD);
  Serial.println("Begin");
  pinMode(LED_PIN, OUTPUT);  

  pinMode(SAD_LED_PIN, OUTPUT);  
  pinMode(NEUTRAL_LED_PIN, OUTPUT);  
  pinMode(HAPPY_LED_PIN, OUTPUT);  

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  ws.connect();
  ws.subscribe(wsCallback);

}

void loop() {
  ws.loop();
}
