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
byte data[3][3][2];

//averages of the value for each pot
int avg_temp[2];
int avg_soil[2];
int avg_light[2]; 

int n_readings = 0;

int n_pots = 2;
int I2CSlaveAddress[2] = {5, 6};      // I2C Address.

// to store the date in the cloud
const char* ssid = "";    // wifi network
const char* password = "";     // wifi pasword 
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
  while (readTiny(I2CSlaveAddress) < 255) {
    Serial.print("WT"); // wait for first byte
  }
  for (int i = 0; i < n_pots; i++) {
    for (int j = 0; j < 3; i++) {
      data[j][n_readings][i] = readTiny(I2CSlaveAddress[i]); // what happen when we read an address that doesn't exist? could we use this to give the "sensation" of adding a new pot?
    }
  }
  n_reading += 1;
  for (int h = 0; h < n_pots; h++) {
    for (int k = 0; k< 3; k++) {
      Serial.print(data[k][h]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
  if (n_readings >= 3) {
    //calcolate the average
    for (int j=0; j < n_readings; j++) {
      for (int i = 0; i < n_pots; i++) {
        avg_temp[i] = 
        avg_hum[i] =
        avg_light[i] =
      }
    }
    for (int i = 0; i < n_pots; i++) {
        sendData(avg_soil[i], avg_hum[i], avg_light[i], I2CSlaveAddress[i]);
    }
    n_readings = 0;
  }
  client.flush(); 
  
  delay(300000);
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
