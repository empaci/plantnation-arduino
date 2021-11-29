/*
  TEST run the system with only 1 sensor  
*/

#include <Wire.h>

byte temp;
long entryP = 0;

const int I2CSlaveAddress = 6;      // I2C Address.

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
  temp = readTiny(I2CSlaveAddress);
  Serial.print(temp);
  Serial.println();
  delay(200);
}

byte readTiny(int address) {
  byte hh ;
  long entry = millis();
  Wire.requestFrom(address, 1);                  // The TinyWire library only allows for one byte to be requested at a time
  while (Wire.available() == 0 && (millis() - entry) < 100)  Serial.print("W");
  if  (millis() - entry < 100) hh = Wire.read();
  return hh;
}
