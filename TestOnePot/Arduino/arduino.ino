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

#include <Wire.h>

int place;
byte data[3];

const int I2CSlaveAddress = 6;      // I2C Address.


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Setup");
}


void loop()
{
  Serial.print("L");
  while (readTiny(I2CSlaveAddress) < 255) {
    Serial.print("WT"); // wait for first byte
  }
  for (int i = 0; i < 3; i++) {
    data[i] = readTiny(I2CSlaveAddress);
  }
  for (int j = 0; j < 3; j++) {
    Serial.print(data[j],DEC);
    Serial.print(" ");
  }
  Serial.println();
}

byte readTiny(int address) {
  byte hh ;
  long entry = millis();
  Wire.requestFrom(address, 1);                  // The TinyWire library only allows for one byte to be requested at a time
  while (Wire.available() == 0 && (millis() - entry) < 100)  Serial.print("W");
  if  (millis() - entry < 100) hh = Wire.read();
  return hh;
}
