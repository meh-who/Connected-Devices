/*
   Post temperature and humidity data via http
*/

// temp library
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

// wifi library
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include "config.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();
byte degree[8] =
{
  0b00011,
  0b00011,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

// Use WiFiClient for HTTP, WiFiSSLClient for HTTPS
WiFiClient wifi;
//WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, SERVER_ADDRESS, SERVER_PORT);
int wifi_status = WL_IDLE_STATUS;


void setup() {
  Serial.begin(9600);

//  while (!Serial)
//    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  connectWiFi();
  
  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) { // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}

void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (! isnan(t)) { // check if 'is not a number'
    Serial.print("Temp *C = "); 
    Serial.println(t);  
  } else {
    Serial.println("Failed to read temperature");
  }

  if (! isnan(h)) { // check if 'is not a number'
    Serial.print("Hum. % = "); 
    Serial.println(h);
  } else {
    Serial.println("Failed to read humidity");
  }

  //send data to server via http
  sendDataToServer(t, h);
  
  Serial.println();
  delay(3000);
}

void sendDataToServer(float temp, float humi) {
  Serial.println("making POST request");
  String contentType = "application/x-www-form-urlencoded";
  
  String postData = "";
  // sensor data
  if (! isnan(temp)) {
    postData += "temperature=" + String(temp);
  }
  if (! isnan(humi)) {
    postData += "&humidity=" + String(humi);
  } 
  // device id
  postData += "&device=" + String(DEVICE_ID);

  client.post("/", contentType, postData);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}


void connectWiFi() {

  Serial.print("WiFi firmware version ");
  Serial.println(WiFi.firmwareVersion());
  
  // attempt to connect to WiFi network
  while (wifi_status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    wifi_status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // wait 3 seconds for connection
    delay(3000);
  }
  Serial.println("Connected to WiFi");
  printWiFiStatus();

}

void printWiFiStatus() {
  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
