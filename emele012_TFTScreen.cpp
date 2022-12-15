#include "TFTScreen.h"
#include <SPI.h>

tftScreen::tftScreen (unsigned char a, unsigned char b, unsigned char c) 
{
    DC = a;
    RES = b;
    CS = c;
    areaH = 160;
    areaW = 130;
    pinMode(DC, OUTPUT);
    pinMode(RES, OUTPUT);
    pinMode(CS, OUTPUT);
}

void tftScreen::init() 
{
  SPI.begin();
   /* *********
    *  SPI on *
    ********* */
  digitalWrite(CS, HIGH);
  delay(100);
  digitalWrite(CS, LOW);
  /* *********
    *  Reset *
    ******** */
  digitalWrite(RES, LOW);
  delay(1);
  digitalWrite(RES, HIGH);
  delay(150);
  /* ******************
   *  Exit Sleep Mode *
   ****************** */
  digitalWrite(DC, LOW);
  SPI.transfer(SPLOUT);
  digitalWrite(DC, HIGH);
  delay(150);
  /* *************
   *  Color Mode *
   ************* */
  digitalWrite(DC, LOW);
  SPI.transfer(COLMOD);
  digitalWrite(DC, HIGH);
  SPI.transfer(0x05);

  tftOn();
}

void tftScreen::tftOn()
{
  /* *********
   *  TFT On *
   ********* */
  digitalWrite(DC, LOW);
  SPI.transfer(DISPON);
  digitalWrite(DC, HIGH);
}

void tftScreen::setBox(char rs, char re, char cs, char ce)
{
  //variables will be used to know the size of the box
  //allowing for more efficient fill
  areaW = ce-cs+1;
  areaH = re-rs+1;
  /* **********
   *  Row Set *
   ********** */
  digitalWrite(DC, LOW);
  SPI.transfer(SROW);
  digitalWrite(DC, HIGH);
  SPI.transfer(0x00);
  SPI.transfer(rs);
  SPI.transfer(0x00);
  SPI.transfer(re);
  /* **********
   *  Col Set *
   ********** */
  digitalWrite(DC, LOW);
  SPI.transfer(SCOL);
  digitalWrite(DC, HIGH);
  SPI.transfer(0x00);
  SPI.transfer(cs);
  SPI.transfer(0x00);
  SPI.transfer(ce);
}

void tftScreen::tftFill(unsigned int color)
{
  digitalWrite(DC, LOW);
  SPI.transfer(MEMWRITE);
  digitalWrite(DC, HIGH);
  //fills current box with pixels of type color
  for(unsigned int i = 0; i <= areaH*areaW; i++)
  {
      SPI.transfer(color>>8);
      SPI.transfer(color&0x00FF);
  }
 
}

//converst 2D array img onto the tft. Each cell is bs pixlels wide and tall on the screen. startXY are the position relative to the tft.
void tftScreen::gameBoard(char img[20][10], char bS, unsigned char startX, unsigned char startY)
{//bS is block Size
  //bbbb bggg gggr rrrr
//bbbb bggg gggr rrrr
//                   BLACK 0, Orange1, Lime2,  RED3,   BLUE4, Yellow5,SkyBlue6, Purple7, 
  const int clrs[9] = {0x0, 0x03FF, 0x07D0, 0x001F, 0xF800, 0x07FF, 0xFFD0, 0xF07F };
  
  for(int r = 0; r < 20; r++)//go to each array
  {
    for(int w = 0; w < 10; w++)//go to eacg cell in the current array
    {
//      if(img[r][w])//if cell has non-zero datat
//      {
        setBox(startX + bS*w, startX + bS*w +bS-1, startY + bS*r, startY + bS*r+bS-1);//create a box of dimension bs * bs
        tftFill(clrs[img[r][w] & 0x7]);                                                //fill that box with the data stored in the cell
//      }
//      else
      //{
//        setBox(startX + bS*w, startX + bS*(w+1)-1, startY + bS*r, startY + bS*(r+1)-1);//create a box of dimension bs * bs
//        tftFill(0xFFFFFF);
      //}
    }
  }
}

//converst 2D array img onto the tft. Each cell is bs pixlels wide and tall on the screen. startXY are the position relative to the tft.
void tftScreen::arrayToScreen(char img[10][30], char bS, unsigned char startX, unsigned char startY)
{//bS is block Size

  int clrs[3] = {0x0000, 0xFFFF, 0x001F};
  for(int r = 0; r < 10; r++)//go to each array
  {
    for(int w = 0; w < 30; w++)//go to eacg cell in the current array
    {
      if(img[r][w])//if cell has non-zero datat
      {
        setBox(startX + bS*w, startX + bS*(w+1), startY + bS*r, startY + bS*(r+1));//create a box of dimension bs * bs
        tftFill(clrs[img[r][w]]);                                                //fill that box with the data stored in the cell
      }
//      else
      
    }
  }
}