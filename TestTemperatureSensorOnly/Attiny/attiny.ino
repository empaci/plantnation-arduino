/*
  TEST run the system with only 1 sensor  
*/

#include <TinyWireS.h>       // Requires fork by Rambo with onRequest support
#include <dht.h>       // dht library to read the temperature and air humidity

#define DHTTYPE DHT11
#define TEMPERATURE 0
#define PIN_TEMPERATURE PB4

const int I2CSlaveAddress = 6;      // I2C Address, between 5-119.

int temp;
int data[3]
unsigned long start;
DHT dht(PIN_TEMPERATURE, DHTTYPE);

void setup()
{
  TinyWireS.begin(I2CSlaveAddress);      // Begin I2C Communication
  TinyWireS.onRequest(transmit);         // When requested, call function transmit()

  dht.begin();
}

void loop()
{
  readData();
  tws_delay(1000); 
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
  if (place > 2) place = 0;
}

void readData()
{
  data[TEMPERATURE] = readTemperature(PIN_TEMPERATURE);
}

int readTemperature(pin) {
  return (int)dht.readTemperature(); //returns a float sp we have to cast it to int
  //int chk = DHT.read11(pin);
  //return DHT.temperature;
}
