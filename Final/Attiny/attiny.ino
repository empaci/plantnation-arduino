/*
This sketch runs on an ATtiny85 connected to an Arduino Uno running a receiver sketch
*/
/*
  TODO LIST
    Add sleep function to read sensor every X minutes
    Make sure to send 32 bit of data OR use another library to send more
  
*/

#include <TinyWireS.h>       // Requires fork by Rambo with onRequest support
// #include <avr/wdt.h>         // watchdog
#include <dht.h>       // dht library to read the temperature and air humidity

// data positions in the array
#define TEMPERATURE 0
#define SOIL_HUMIDITY 1
#define LIGHT 2

// pin to connect the sensors 
#define PIN_LIGHT_1 PB1
#define PIN_LIGHT_2 PB2
#define PIN_SOIL_HUMIDITY PB3
#define PIN_TEMPERATURE PB4

// variable for the temperature sensor
#define DHTTYPE DHT11
DHT dht(PIN_TEMPERATURE, DHTTYPE);

// variable for the soilsensor
const int AIR_VALUE 620
const int WATER_VALUE 310

const int I2CSlaveAddress = 5;      // I2C Address, between 5-119.

int data[3];                    // Where the Data is stored (8 bit unsigned)
int place = 0;
unsigned long start;

void setup()
{
  TinyWireS.begin(I2CSlaveAddress);      // Begin I2C Communication
  TinyWireS.onRequest(transmit);         // When requested, call function transmit()
  // wdt_enable(WDTO_500MS);               // Watchdog
}

void loop()
{
  readData();
  tws_delay(1000);
  //wdt_reset();                          // feed the watchdog
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
    case 2:
      hh = data[SOIL_HUMIDITY];       // Send last recorded value for current sensor
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
  data[TEMPERATURE] = readTemperature();
  tws_delay(20);
  data[SOIL_HUMIDITY] = readSoilHumidity(PIN_SOIL_HUMIDITY);
  tws_delay(20);
  data[LIGHT] = readLight(PIN_LIGHT_1,PIN_LIGHT_2);
  tws_delay(20);
}

int readTemperature() {
  return (int)dht.readTemperature();
}

int readSoilHumidity(pin) {
  soilMoistureValue = analogRead(pin) >> 2; // since it is 10 bit we shift the last 2 digits
  soilMoisturePercentage = map(soilMoistureValue, AIR_VALUE, WATER_VALUE, 0, 100);
  return (int)soilMoiturePercentage;
}

int readLight(pin1, pin2) { //we are using two lightsensor
  lightValue_1 = analogRead(pin1) >> 2;
  lightValue_2 = analogRead(pin2) >> 2;
  if(lightValue_1 < lightValue_2){return (int)lightValue_2;}
  else{return (int)lightValue_1;}
}
