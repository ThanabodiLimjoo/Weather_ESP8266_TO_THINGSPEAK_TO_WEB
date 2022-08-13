
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <ThingSpeak.h>

DHT dht(D4, 11);
WiFiClient client;
unsigned long myChannelNumber = 1739928;
const char * myWriteAPIKey = "8O6DD4EZQ6F6EBBF";
int statusCode = 0;
//const char* ssid = "IoTAD02";
//const char* pass = "ccsadmin";
const char* ssid = "JaNa";
const char* pass = "123456789";

void setup() {
  pinMode(D6, OUTPUT);
  Serial.begin(115200);
  wifi_init();
  ThingSpeak.begin(client);
  dht.begin();
  digitalWrite(D6,1);
}

void loop() {
  Client_Send();
  statusCode = ThingSpeak.readMultipleFields(myChannelNumber);
  if(statusCode == 200){
    int temp = ThingSpeak.getFieldAsInt(1); // Field 1
    int hum = ThingSpeak.getFieldAsInt(2); // Field 2
    int light = ThingSpeak.getFieldAsInt(3); // Field 3
    int led = ThingSpeak.getFieldAsInt(4); // Field 4
    digitalWrite(D6,led);
    Serial.println("Temp: " + String(temp));
    Serial.println("Hum: " + String(hum));
    Serial.println("Light: " + String(light));
    Serial.println("Light: " + String(led));
  }
  else{
    Serial.println("HTTP error code " + String(statusCode));
  }
Serial.println();
delay(5000); // no need to fetch too often
}

void wifi_init(){
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting... ");
    Serial.printf("Connection Status: %d\n", WiFi.status());
    delay(1000);
  }
  Serial.println("Wi-Fi connected.");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
}

void Client_Send(){
  float tempSend = dht.readTemperature();
  float humSend = dht.readHumidity();
  int lightSend = analogRead(A0);
  int ledSend = digitalRead(D6);
  ThingSpeak.setField(1, tempSend);
  ThingSpeak.setField(2, humSend);
  ThingSpeak.setField(3, lightSend);
  ThingSpeak.setField(4, ledSend);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
   }
}
