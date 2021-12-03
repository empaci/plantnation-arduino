#include <Wire.h>
#define I2C_SLAVE_ADDR  0x26            // i2c slave address (38)

 void setup() 
 {
     Wire.begin();
     Serial.begin(9600);
 }

 void loop() 
 {
      byte num;

     // read 1 byte, from address 0x26
     Wire.requestFrom(I2C_SLAVE_ADDR, 1);

     while(Wire.available()) {
          num = Wire.read();
     }
     
     Serial.print("Received: ");
     Serial.println(num,DEC);
 }
