#include <Wire.h>

//1 GND Ground
//2 VCC Power source (5V +/-10%)
//3 SDA I2C(5V) Data line
//4 SCL I2C(5V) Clock line
 
void setup()
 
{
 
  Wire.begin();                // join i2c bus (address optional for master)
 
  Serial.begin(9600);          // start serial communication at 9600bps
 
}
 
 
 
int reading = 0;
 
int count=0;
 
char buf[3];
 
 
 
void loop()
 
{
 
  // step 3: instruct sensor to return a particular echo reading
 
  Wire.beginTransmission(0x0a);
 
  Wire.write(byte(0x4c));      // sets register pointer to echo #1 register (0x02)
 
  Wire.endTransmission(false);      // stop transmitting
 
  // step 4: request reading from sensor
 
  Wire.requestFrom(0x0a, 32);    // request 2 bytes from slave device #112
 
 
 
  // step 5: receive reading from sensor
 
  count=0;
 
  while(count<32)
 
  {
 
  if(2 <= Wire.available())    // if two bytes were received
 
  {
 
    reading = Wire.read();  // receive high byte (overwrites previous reading)
 
    reading+= Wire.read()<<8;
 
    Serial.print(reading);   // print the reading
 
    Serial.print(" ");
 
    count+=2;
 
  }
 
  }
  //while(Wire.available()<1);
  //reading = Wire.read();
 
  Serial.println();
  delay(60000);
}
