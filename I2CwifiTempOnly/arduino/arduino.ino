//-----------------------------------------------
// gas service id : AKfycbwEDnw4kalWVDqxJqxfBWVClaedfJ4NlNVwYW6HvJJB65a59jdH
//-----------------------------------------------

#include <Wire.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include<DHT.h>

#define DHTPIN 2 // A2 on arduino mkr 1010
#define DHTTYPE DHT11 
 
DHT dht(DHTPIN, DHTTYPE);

byte temp;

const char* ssid = "AndroidAP";    // wifi network
const char* password = "uxnk1607aA";     // wifi pasword 
const char* host = "script.google.com";
const int httpsPort = 443;
const int httpPort = 80;

const int I2CSlaveAddress = 6;      // I2C Address.

// Use WiFiSSLClient
WiFiSSLClient client;
String GAS_ID = "AKfycbwEDnw4kalWVDqxJqxfBWVClaedfJ4NlNVwYW6HvJJB65a59jdH";   // GAS service id

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  dht.begin();
  
  //connecting to internet
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  Serial.print("L");
  while (readTiny(I2CSlaveAddress) < 255) {
    Serial.print("WT"); // wait for first byte
  }
  temp = readTiny(I2CSlaveAddress);
  Serial.println(temp,DEC);

  //convert temp in int?

  sendData(temp, 0, 0);
  client.flush();
  
  delay(600);
}

byte readTiny(int address) {
  byte res;
  long entry = millis();
  Wire.requestFrom(address, 1);                  // The TinyWire library only allows for one byte to be requested at a time
  while (Wire.available() == 0 && (millis() - entry) < 100)  Serial.print("W");
  if  (millis() - entry < 100) res = Wire.read();
  return res;
}

// Function for Send data into Google Spreadsheet
void sendData(int tem, int hum, int light) {
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String string_temperature =  String(tem, DEC); 
  String string_humidity =  String(hum, DEC); 
  String string_light =  String(light, DEC);
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity + "&light=" + string_light;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: ESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  client.stop();
} // Function for Send data into Google Spreadsheet
void sendData(int tem, int hum, int light) {
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String string_temperature =  String(tem, DEC); 
  String string_humidity =  String(hum, DEC); 
  String string_light =  String(light, DEC);
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity + "&light=" + string_light;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: ESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  client.stop();
} 
