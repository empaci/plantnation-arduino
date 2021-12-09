/*
The information should be sendt as follow:
  light: each hour we send how many minuts the plant have goten light . 
  moist: send the average of moist from the last three reading (15 min) each hour.
  temprature: send the average temprature from the last three reading (15 min) each hour, and the average from the last 12 hours.
*/

/*
  TODO LIST
    X Add wifi communication with the app
    Change the way the data from microcontroller are received
    Add functionality to work with n microcontrollers
    Add system to records data sensors (example every 5 reads take the average and save it a file)
*/

#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>

int place;
byte data[3][2];

int n_pots = 2;
int I2CSlaveAddress[2] = {6, 8};      // I2C Address.

// to store the date in the cloud
const char* ssid = "ONEPLUS_co_aphgdz";    // wifi network
const char* password = "hgdz8309";     // wifi pasword 
const char* host = "script.google.com";
const int httpsPort = 443;
WiFiSSLClient client;
String GAS_ID = "AKfycbwEDnw4kalWVDqxJqxfBWVClaedfJ4NlNVwYW6HvJJB65a59jdH";   // GAS service id


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Setup");

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
  while (readTiny(I2CSlaveAddress[0]) < 255) {
    Serial.print("WT"); // wait for first byte
  }
  for (int j = 0; j < 3; j++) {
      data[j][0] = readTiny(I2CSlaveAddress[0]);
  }
  while (readTiny(I2CSlaveAddress[1]) < 255) {
    Serial.print("WT"); // wait for first byte
  }
  for (int j = 0; j < 3; j++) {
      data[j][1] = readTiny(I2CSlaveAddress[1]);
  }
  for (int h = 0; h < n_pots; h++) {
    for (int k = 0; k< 3; k++) {
      Serial.print(data[k][h]);
      Serial.print(" ");
    }
    Serial.println(I2CSlaveAddress[h]);
  }
  Serial.println();

  for (int i = 0; i < n_pots; i++) {
    sendData(data[0][i], data[1][i], data[2][i], I2CSlaveAddress[i]);
  }
  client.flush(); 
  
  delay(5000);
}

byte readTiny(int address) {
  byte hh ;
  long entry = millis();
  Wire.requestFrom(address, 1);                  // The TinyWire library only allows for one byte to be requested at a time
  while (Wire.available() == 0 && (millis() - entry) < 100)  Serial.print("W");
  if  (millis() - entry < 100) hh = Wire.read();
  return hh;
}

void sendData(int tem, int hum, int light, int pot) {
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String string_temperature =  String(tem, DEC); 
  String string_humidity =  String(hum, DEC); 
  String string_light =  String(light, DEC);
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity + "&light=" + string_light+"&pot="+pot;
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
