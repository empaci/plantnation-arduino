/*
  TEST run the system with only 1 sensor  
*/

#include <TinyWireS.h>       // Requires fork by Rambo with onRequest support
#include <DHT.h>       // dht library to read the temperature and air humidity

#define DHTTYPE DHT11
#define TEMPERATURE 0
#define PIN_TEMPERATURE PB3

DHT dht(PIN_TEMPERATURE, DHTTYPE);

const int I2CSlaveAddress = 6;      // I2C Address, between 5-119.

int temp;
int data[3];
int place = 0;

void setup()
{
  TinyWireS.begin(I2CSlaveAddress);      // Begin I2C Communication
  TinyWireS.onRequest(transmit);         // When requested, call function transmit()
  dht.begin();
}

void loop()
{
  readData();
  //tws_delay(200); 
}

//-------------------------------------------------------------------

void transmit()
{
  byte hh;
  switch (place) {
    case 0:
      hh = 255;                       // start byte (header)
      break;
    case 1:
      hh = data[TEMPERATURE];          // Send last recorded value for current sensor
      break;
  }
  TinyWireS.send(hh);
  place = place + 1;
  if (place > 1) place = 0;
}

void readData()
{
  data[TEMPERATURE] = readTemp();
}

int readTemp() {
  return (int)dht.readTemperature() >> 2; //returns a float sp we have to cast it to int
}
