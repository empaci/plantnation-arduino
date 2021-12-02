//-----------------------------------------------
// gas service id : AKfycbwEDnw4kalWVDqxJqxfBWVClaedfJ4NlNVwYW6HvJJB65a59jdH
//-----------------------------------------------

#include <SPI.h>
#include <WiFiNINA.h>
#include<DHT.h>

#define DHTPIN 2 // A2 on arduino mkr 1010
#define DHTTYPE DHT11 
 
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "";    // wifi network
const char* password = "";     // wifi pasword 
const char* host = "script.google.com";
const int httpsPort = 443;
const int httpPort = 80;

// Use WiFiSSLClient
WiFiSSLClient client;
// SHA1 fingerprint of the certificate, don't care with your GAS service
//const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GAS_ID = "AKfycbwEDnw4kalWVDqxJqxfBWVClaedfJ4NlNVwYW6HvJJB65a59jdHwd";   // GAS service id

void setup() {
  dht.begin();  // sensor
  Serial.begin(115200); //Serial
  Serial.println();

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


void loop() {
  int h = 1; //(int)dht.readHumidity();
  int t = 4; //(int)dht.readTemperature();
  Serial.print("Temp = ");
  Serial.print(t);
  Serial.print(" HUM= ");
  Serial.println(h);
  sendData(t, h);

  //client.flush(); 
 
  delay(2000);
}

// Function for Send data into Google Spreadsheet
void sendData(int tem, int hum) {
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  /*if (client.verify(fingerprint, host)) {
  Serial.println("certificate matches");
  } else {
  Serial.println("certificate doesn't match");
  }
  */
  String string_temperature =  String(tem, DEC); 
  String string_humidity =  String(hum, DEC); 
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
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
  //client.stop();
} 
