/*
This sketch runs on an ATtiny85 connected to an Arduino Uno running a receiver sketch
*/
/*
  TODO LIST
    Add sleep function to read sensor every X minutes
    Add function to read ligth sensor
    Make sure to send 32 bit of data OR use another library to send more
  
*/

#include <TinyWireS.h>       // Requires fork by Rambo with onRequest support
#include <avr/wdt.h>         // watchdog
#include <dht.h>       // dht library to read the temperature and air humidity

#define TEMPERATURE 0
#define SOIL_HUMIDITY 1
#define LIGHT 2

// pin to connect the sensors 

#define PIN_LIGHT PB1
#define PIN_SOIL_HUMIDITY PB3
#define PIN_TEMPERATURE PB4


const int AIR_VALUE 620
const int WATER_VALUE 310

const int I2CSlaveAddress = 5;      // I2C Address, between 5-119.

unsigned int soilMoistureValue;
int data[3];                    // Where the Data is stored (8 bit unsigned)
int place = 0;
unsigned long start;
dht DHT;

void setup()
{
  TinyWireS.begin(I2CSlaveAddress);      // Begin I2C Communication
  TinyWireS.onRequest(transmit);         // When requested, call function transmit()

  wdt_enable(WDTO_500MS);               // Watchdog
}


void loop()
{
  readData();
  wdt_reset();                          // feed the watchdog
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
  delay(20);
  data[SOIL_HUMIDITY] = readSoilHumidity();
  //soilMoistureValue = readSoilHumidity();
  // data[SOIL_HUMIDITY] = map(soilMoistureValue, AIR_VALUE, WATER_VALUE, 0, 100);
  delay(20);
  data[LIGHT] = readLight();
  delay(20);
}

void readTemperature(PIN_TEMPERATURE) {
  
  int chk = DHT.read11(PIN_TEMPERATURE);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
}

int readSoilHumidity(PIN_SOIL_HUMIDITY) {
  soilMoistureValue = analogRead(PIN_SOIL_HUMIDITY);// >> 2; per togliere 2 bit in piu
}

int readLight(PIN_LIGHT) {
  return 0
}
