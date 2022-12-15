#ifndef TFTSCREEN_H
#define TFTSCREEN_H

//Defining the operations
#define SPLOUT 0x11
#define DISPON 0x29
#define MEMWRITE 0x2C
#define COLMOD 0x3A
#define SROW  0x2A
#define SCOL 0x2B


using namespace std;

class tftScreen {

 private:
  unsigned char DC, RES, CS;
  unsigned char areaH, areaW;
  
  
//  const int colors[9] = {0x0, 0xFFFF, 0x001F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,0xFFFF, 0x01FF};
 public:
  tftScreen(unsigned char, unsigned char, unsigned char);
  void init();
  void tftOn();
  void tftFill(unsigned int);
  void setBox(char, char, char, char);
  
  void gameBoard(char[][10], char, unsigned char, unsigned char);
  void arrayToScreen(char[][30], char, unsigned char, unsigned char);
};

#endif