#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_BMP085.h>
WiFiClientSecure client;

//Things to change
const char * ssid = "Path of the wind";
const char * password = "Chayanis";

String GOOGLE_SCRIPT_ID = "AKfycbxoW1Zdg5mXwqR-tTgIYhaS6oTMZSUzkNGi3NLNQNxPJGvOSNakaZckjjGCv7GbtZ8rnQ"; 
//https://script.google.com/macros/s/AKfycbykhRYyuenN6P1WYPeYQ7RqIR2l4j9ubVpODGJgVl2iRMCxsFfCEbxoXxpW7v1X7dqmeg/exec

Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_BMP085 bmp;
void setup() {
  Serial.begin(115200);
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Started");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("Ready to go");
  Wire.begin();
  Wire.setClock(100000);
  sht31.begin(0x44);
  bmp.begin();
}

void loop() {
  float tempnum = sht31.readTemperature();
  float moisnum = sht31.readHumidity();
  float presnum = bmp.readPressure();
  String tempsr(tempnum);
  String moissr(moisnum);
  String pressr(presnum);
  sendData("temperature="+tempsr+"&moisture="+moissr+"&pressure="+pressr);
  delay(10000);
}

void sendData(String params) {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
   //Serial.print(url);
    //Serial.print("Making a request");
    http.begin(url.c_str()); //Specify the URL and certificate
    int httpCode = http.GET();  
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    http.end();
    //Serial.println(": done "+httpCode);
}
