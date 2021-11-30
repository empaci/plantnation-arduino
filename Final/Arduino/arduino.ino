/*
This sketch runs on an Arduino connected to an ATtiny85/45 running ThreeSensors_Tiny.ino
The data received is HC-SR04 PING sensor data in centimeters
*/

/*
The information should be sendt as follow:
  light: each hour we send how many minuts the plant have goten light . 
  moist: send the average of moist from the last three reading (15 min) each hour.
  temprature: send the average temprature from the last three reading (15 min) each hour, and the average from the last 12 hours.
*/

/*
  TODO LIST
    Add wifi communication with the app
    Change the way the data from microcontroller are received
    Add functionality to work with n microcontrollers
    Add system to records data sensors (example every 5 reads take the average and save it a file)
*/

#include <Wire.h>

int place;
byte dist[3];
long entryP = 0;

const int I2CSlaveAddress = 5;      // I2C Address.


void setup()
{
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Setup");
}



void loop()
{
  Serial.print("L");
  while (readTiny(I2CSlaveAddress) < 255) {
    Serial.print("WT"); // wait for first byte
  }
  for (place = 0; place < 3; place++) {
    dist[place] = readTiny(I2CSlaveAddress);
  }
  for (int i = 0; i < 3; i++) {
    Serial.print(dist[i]);
    Serial.print(" ");
  }
  Serial.println();
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
