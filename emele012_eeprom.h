#ifndef EEPROM_H
#define EEPROM_H

using namespace std;
#include <Wire.h>


class eeprom {

 private:
                 //0101 0(A2)(A1)(A0)
  char sAddr = 0x50;
  
 public:
  eeprom();
  void storeDataInt(int, int);
  void storeDataLong(int, char data[]);

  int fetchDataInt(int);
  unsigned long fetchDataLong(int);
  
  
//  template <typename T> void storeData(int, int, T);//would like to addd ability to add names to scores(3 chars)

//  template <typename T> T fetchData(int addr, int numBytes);
  
  void resetDataInt(int addr);
  void resetDataLong(int addr);
};

#endif