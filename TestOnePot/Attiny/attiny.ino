/*
  TODO LIST
    Add sleep function to read sensor every X minutes
    Make sure to send 32 bit of data OR use another library to send more
*/

#include <TinyWireS.h>       // Requires fork by Rambo with onRequest support
//#include <DHT.h>       // dht library to read the temperature and air humidity
#include <TinyDHT.h>

// data positions in the array
#define TEMPERATURE 0
#define SOIL_HUMIDITY 1
#define LIGHT 2

// pin to connect the sensors 
#define PIN_LIGHT_1 PA1
//#define PIN_LIGHT_2 PB2
#define PIN_SOIL_HUMIDITY PA2
#define PIN_TEMPERATURE PA3

// variable for the temperature sensor
#define DHTTYPE DHT11

// variable for the soilsensor
const int AIR_VALUE = 620;
const int WATER_VALUE = 310;

const int I2CSlaveAddress = 6;      // I2C Address, between 5-119.

DHT dht(PIN_TEMPERATURE, DHTTYPE);

int data[3];                    // Where the Data is stored (8 bit unsigned)
int place = 0;

void setup()
{
  TinyWireS.begin(I2CSlaveAddress);      // Begin I2C Communication
  TinyWireS.onRequest(transmit);         // When requested, call function transmit()
}

void loop()
{
  //tws_delay(300000) // 5 minutes in miliseconds 
  readData();
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
      hh = 11; //data[TEMPERATURE];          // Send last recorded value for current sensor
      break;
    case 2:
      hh = 10; //data[SOIL_HUMIDITY];       // Send last recorded value for current sensor
      break;
    case 3:
      hh = data[LIGHT];       // Send last recorded value for current sensor
      break;
  }
  TinyWireS.send(hh);
  place = place + 1;
  if (place > 3) place = 0;
}

void readData()
{
  data[TEMPERATURE] = readTemp();
  tws_delay(20);
  data[SOIL_HUMIDITY] = readSoilHumidity(PIN_SOIL_HUMIDITY);
  tws_delay(20);
  data[LIGHT] = readLight(PIN_LIGHT_1); //,PIN_LIGHT_2);
  tws_delay(20);
}

int readTemp() {
  return (int)dht.readTemperature() >> 2;
}

int readSoilHumidity(int pin) {
  int soilMoistureValue = (int)analogRead(pin) >> 2; // since it is 10 bit we shift the last 2 digits (equivalent to dividing by 4)
  //int soilMoisturePercentage = map(soilMoistureValue, AIR_VALUE, WATER_VALUE, 0, 100);
  //return soilMoisturePercentage;
  return soilMoistureValue;
}

int readLight(int pin1, int pin2) { //we are using two lightsensor
  int lightValue_1 = (int)analogRead(pin1) >> 2;
  int lightValue_2 = (int)analogRead(pin2) >> 2;
  int max_light = lightValue_1 > lightValue_2? lightValue_1 : lightValue_2;
  return max_light;
}

int readLight(int pin) { //one sensor option
  int lightValue = (int) analogRead(pin) >> 2;
  return lightValue;
}
