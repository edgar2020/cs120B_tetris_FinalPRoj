#include "eeprom.h"

eeprom::eeprom()
{
  Wire.begin();
//  internAddr = inter;
}

// https://www.arduino.cc/reference/en/language/functions/communication/wire/write/
void eeprom::storeDataInt(int addr, int data)
{
  Wire.beginTransmission(sAddr);
    Wire.write(addr >> 8); 
    Wire.write(addr & 0xFF);
    
      Wire.write(data>>8 & 0xFF);
      Wire.write(data & 0xFF);
    
  Wire.endTransmission();
  //need delay(will add it as PWM machine)
}

void eeprom::storeDataLong(int addr, char data[])
{
  Wire.beginTransmission(sAddr);
    Wire.write(addr >> 8); 
    Wire.write(addr & 0xFF);

    Wire.write(data, 4);
//      Wire.write(data>>24 & 0xFF);
//      Wire.write(data>>16 & 0xFF);
//      Wire.write(data>>8 & 0xFF);
//      Wire.write(data & 0xFF);
    
  Wire.endTransmission();
  //need delay(will add it as PWM machine)
}


//https://www.arduino.cc/reference/en/language/functions/communication/wire/read/
//template <typename T> T 
int eeprom::fetchDataInt(int addr)
{
    int temp = 0;
    //updates last memory register
    Wire.beginTransmission(sAddr);
      Wire.write(addr >> 8); 
      Wire.write(addr & 0xFF);
    Wire.endTransmission();
    
    Wire.requestFrom(sAddr, 2);
    while(Wire.available()) {//from documentation
        temp<<8;
        temp |= Wire.read();    // Receive a byte as character
      }
   return temp;
}

unsigned long eeprom::fetchDataLong(int addr)
{
    unsigned long temp = 0;
    //updates last memory register
    Wire.beginTransmission(sAddr);
      Wire.write(addr >> 8); 
      Wire.write(addr & 0xFF);
    Wire.endTransmission();
    
    Wire.requestFrom(sAddr, 4);

    temp = temp | Wire.read();
    temp = temp << 8 | Wire.read();
    temp = temp << 8 | Wire.read();
    temp = temp << 8 | Wire.read();
//    while(Wire.available()) {//from documentation
//        temp<<8;
//        temp |= Wire.read();    // Receive a byte as character
//      }
//    for(char i = 0; i < 4; i ++)
//    {
//      temp<<8;
//      temp |= Wire.read();    // Receive a byte as character
//    }
   return temp;
}

void eeprom::resetDataInt(int addr)
{
  storeDataInt(addr, 0x0);
}
void eeprom::resetDataLong(int addr)
{
  storeDataLong(addr, 0x0);
}