/*        Edgar(ventura) Melendrez emele012@ucr.edu
            Discussion Section: 021
           Assignment: Final Project
      
           I acknowledge all content contained herein,
        excluding template or example code, is my own original work.

        On 11/27/2022 at 7:14 I accidently deleted my entire project 
        To make the deadlines I have had to strip to some basic features
        in hopes I can achieve my complexities. Hopefully I suceeded

        I think I super suceeded
*/

#include <LiquidCrystal.h>
#include <SPI.h>

#include "Queue.h"
#include "TFTScreen.h"
//#include "pieces.h"
#include "game.h"
#include "RNG.h"
#include "eeprom.h"
//LCD
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//TFT
const unsigned char DC = 8; //A0 pin
const unsigned char RES = 9; //Reset pin
const unsigned char CS = 10; //Chip select pin
//#define SPLOUT 0x11
//#define DISPON 0x29
//#define MEMWRITE 0x2C
//#define COLMOD 0x3A

unsigned int HS_ADDR  = 0x0000;
unsigned int SEED_ADDR = 0x0100;

//gameVariables
char power = 0;
unsigned int highScore = 0;
unsigned int curScore = 0;
char gameOver = 0;

char jsxPin = A1, jsyPin = A0, jsBtnPin = 0;
int jsx, jsy;
char jsbtnPress;

enum JSDirect{UP, RIGHT, DOWN, LEFT, NONE} directions;

queue<int> storeData(2);


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

tftScreen tft = tftScreen(DC, RES, CS);
RNG r =RNG();
eeprom EE = eeprom();
game curGame = game();

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;

/**********************************************
 * WIll Read in the inputs and update 
 * enum JSDirections with input instruction
 * ********************************************/
enum readJS { readInputsOn };
int joystickSM(int state)
{
  switch(state)//transitions
  {
    case readInputsOn:
       state = readInputsOn;
       break;
  }
  
  switch(state)//actions
  {
    case readInputsOn:
      jsx = 1024-analogRead(jsxPin);
      jsy = 1024-analogRead(jsyPin);
      jsbtnPress = !digitalRead(jsBtnPin);
      

      //increment/decrement based on x component of js
      if(jsx <= 125) // js left, decrease
      {
         directions = LEFT;
      }
      else if (jsx >= 900) //js right increment
      {
        directions = RIGHT;
      }
      else if(jsy <= 125) //js down 
      {
        directions = DOWN;
      }
      else if(jsy >= 900) //js up
      {
        directions = UP;
      }
      else
      {
        directions = NONE;
      }
      
      break;
  }
//  Serial.print("dfs " );
//  Serial.println(directions);
   
  return state;
}
/**********************************************
 * Control power variable which controls 
 * the entire SM
 * ********************************************/
enum powerStates{idlePower, powerPress};
int powerSM(int state)
{
  static unsigned char c = 0;
  switch(state)//transitions
  {
    case idlePower:
      state = idlePower;
      if(jsbtnPress)
      {
        c=0;
        state = powerPress; 
      }
      break;
    case powerPress:
      if(jsbtnPress)
      {
        state = powerPress; 
        c++;
      }
      else
      {
        if(c >= 40)
        {power = !power;}
        c=0;
        state = idlePower;
      }
      break;
  }
  
  switch(state)//actions
  {
    case idlePower:
      break;
    case powerPress:
      break;
  }
  return state;
}

/**********************************************
 * Keep Tracks of what is the current game
 * state of Tetris
 * Updates the tft screen accordingly
 * ********************************************/
enum screens {gameOff, gameLoading1, gMain, gMainTr, gPlay, gOver, gOverTr } GS;
int projectScreens(int state)
{
  static unsigned char c = 0;
  switch(state)//transitions
  {
    case gameOff:
      if(!power)
      {
        state = gameOff;
      }
      else if(power)
      {
        c=0;
        state = gameLoading1;
        tft.tftFill(0x0000);
        tft.setBox(40, 100, 35, 105);
        tft.tftFill(0x7070);
      }
      break;
    case gameLoading1:
      if(!power)
      {
        tft.setBox(0, 128, 0, 128);
        tft.tftFill(0x0000);
        state = gameOff;
      }
      else if(c>=1)
      {
        c=0;
        tft.setBox(0, 128, 0, 128);
        tft.tftFill(0x0000);
        char image[10][30] =
        {
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 2, 0, 1, 1, 1, 0, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 2, 0, 1, 1, 1, 0, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 2, 0, 1, 1, 1, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };
        tft.setBox(40, 100, 35, 105);
        tft.tftFill(0xF000);
        tft.arrayToScreen(image, 4, 10, 2);
        state = gMain;
      }
      else
      {
        c++;
        state = gameLoading1;
      }
      break;
    case gMain:
      if(!power)
      {
        tft.setBox(0, 128, 0, 128);
        tft.tftFill(0x0000);
        state = gameOff;
      }
      else if(jsbtnPress)
      {
        state=gMainTr;
      }
      else
      {
        state=gMain;
      }
      break;
    case gMainTr:
      if(!power)
      {
        tft.setBox(0, 128, 0, 128);
        tft.tftFill(0x0000);
        state = gameOff;
      }
      else if(jsbtnPress)
      {
        state=gMainTr;
      }
      else
      {
        state=gPlay;
      }
      break;
    case gPlay:
      if(!power)
      {
        tft.setBox(0, 128, 0, 128);
        tft.tftFill(0x0000);
        state = gameOff;
      }
      else if(gameOver)
      {
        state=gOver;
      }
      else
      {
        state=gPlay;
      }
      break;
    case gOver:
      if(!power)
      {
        tft.setBox(0, 128, 0, 128);
        tft.tftFill(0x0000);
        state = gameOff;
      }
      else if(jsbtnPress)
      {
        state=gOverTr;
      }
      else
      {
        state = gOver;
      }
      break;
    case gOverTr:
      if(!power)
      {
        tft.setBox(0, 128, 0, 128);
        tft.tftFill(0x0000);
        state = gameOff;
      }
      else if(jsbtnPress)
      {
        state=gOverTr;
      }
      else
      {
        state=gameLoading1;
        c=0;
        state = gameLoading1;
        tft.tftFill(0x0000);
        tft.setBox(40, 100, 35, 105);
        tft.tftFill(0x7070);
      }
      break;
  }
  
  switch(state)//actions
  {
    case gameOff:
      break;
    case gameLoading1:
      break;
    case gMain:
      break;
    case gMainTr:
      break;
    case gPlay:
      break;
    case gOver:
      break;
  } 
  GS = state;
  return state;
}

/**********************************************
 * Controls the displays to the LCD screen
 * ********************************************/
enum lcdStates{lcdOff, lcdMain, lcdGame, lcdGameOver};
int lcdSM(int state)
{
  switch(state)//transitions
  {
    case lcdOff:
      if(!power)
      {
        lcd.clear();
        state = lcdOff;
      }
      else 
      {
        state = lcdMain;
        lcd.setCursor(0, 0);
        lcd.print("JS BTN to Play");
        lcd.setCursor(0, 1);
        lcd.print("Highscore:");
        highScore = EE.fetchDataInt(HS_ADDR);
        lcd.print(highScore);
      }
      break;
    case lcdMain:
      if(!power)
      {
        lcd.clear();
        state = lcdOff;
      }
      else if(GS == gPlay)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("HighScore:");
        lcd.print(highScore);
        state = lcdGame;
      }
      else
      {
        state = lcdMain;
      }
      break;
    case lcdGame:
      if(!power)
      {
        lcd.clear();
        state = lcdOff;
      }
      else if(gameOver)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("GameOver");
        lcd.setCursor(0,1);
        lcd.print("HS: ");
        lcd.print(highScore);
        lcd.print(" S: ");
        lcd.print(curScore);
        lcd.print("    ");
        state = lcdGameOver;
      }
      else
      {
        lcd.setCursor(0,1);
        lcd.print("Score: ");
        lcd.print(curScore);
        lcd.print("    ");
        state = lcdGame;
      }
      break;
    case lcdGameOver:
      if(!power)
      {
        lcd.clear();
        state = lcdOff;
      }
      else if(GS == gMain)
      {
        state = lcdMain;
        lcd.setCursor(0, 0);
        lcd.print("JS BTN to Play");
        lcd.setCursor(0, 1);
        lcd.print("Highscore:");
        highScore = EE.fetchDataInt(HS_ADDR);
        lcd.print(highScore);
      }
      else
      {
        state = lcdGameOver;
        
      }
      break;
      
  }
  switch(state)//actions
  {
    case lcdOff:
      break;
    case lcdMain:
      break;
    case lcdGame:
      break;
    case lcdGameOver:
      break;
  }
  return state;
}

/**********************************************
 * COntrols the logic of the game, it knows
 * when to start thanks to projectScreens
 * Takes over TFT display when in gameplay
 * ********************************************/
enum gamePlayStates{gameInit, createPiece, gamePlay, gameNot};
int gamePlaySM(int state)
{
  switch(state)//transitiond
  {
    case gameNot:
      if(!power)
      {
        state = gameNot;
      }
      else if(GS == gPlay)//initialize gamepaly
      {
        state = gameInit;
        //#TODO initialize board here
        curGame.clearBoard();
        curGame.drawBoard(&tft);
        
      }
      else
      {
        state = gameNot;
      }
      break;
    case gameInit://sole purpose is to load the game board
      if(!power)
      {
        state = gameNot;
      }
      else
      {
        
        state = createPiece; 
      }
      break;
    case createPiece:
      if(!power)
      {
        state = gameNot;
      }
      else 
      {
        state = gamePlay;
      }
      break;
    case gamePlay:
      if(!power)
      {
        state = gameNot;
      }
      else if(jsbtnPress)
      {
        state = gameInit;
        curGame.clearBoard();
        curGame.drawBoard(&tft);
      }
      else if(curGame.broadcastStuckPiece() && !curGame.checkGameOver() )//not game over yet
      {
        curScore += curGame.clearLines();
        if(curScore > highScore)
        {
          highScore = curScore;
        }
        state = createPiece;
      }
      else if(curGame.broadcastStuckPiece() && curGame.checkGameOver() )//game over
      {
        gameOver = 1;
        if(curScore >= highScore)
        {
          storeData.push(HS_ADDR);
        }
        storeData.push(SEED_ADDR);
        state = gameNot;
      }
      else
      {
        state = gamePlay;
      }
      break;
    
  }
  switch(state)//actions
  {
    case gameNot:
      break;
    case gameInit:
      curScore = 0;
      gameOver = 0;
      Serial.print("Get Seed");
        Serial.println(EE.fetchDataLong(SEED_ADDR), HEX);
      r.setSeed(EE.fetchDataLong(SEED_ADDR));
      break;
    case createPiece:
        //#TODO create piece here
        curGame.setPieceStuck(0);
        curGame.createPiece(r.newRNG()%7);
      break;
    case gamePlay:
//      Serial.println(directions);
      curGame.updateBoard(directions);
      curGame.drawGame(&tft);
      break;
  }
  return state;
}

enum eepromStates{EE_noUse, EE_SaveData, EE_Wait};
int EEPROM_SM(int state)
{
  switch(state) //transitions
  {
    case EE_noUse:
      if(storeData.isEmpty())
      {
        state = EE_noUse;
      }
      else
      {
        state = EE_SaveData;
      }
      break;
    case EE_SaveData:
      state = EE_Wait;
      break;
    case EE_Wait:
      if(storeData.isEmpty())
      {
        state = EE_noUse;
      }
      else
      {
        state = EE_SaveData;
      }
      break;
  }
  switch(state)//actions
  {
    case EE_noUse:
      break;
    case EE_SaveData:
      unsigned int temp = storeData.pop();
      if(temp == HS_ADDR)
      {
        EE.storeDataInt(HS_ADDR, highScore);
      }
      else if(temp == SEED_ADDR)
      {
        long t = r.getSeed();
        char temp[4] = {t >>24, t >>16 & 0xFF, t >> 8 & 0xFF, t & 0xFF};
        EE.storeDataLong(SEED_ADDR, temp);
      }
      break;
    case EE_Wait:
      //just wait 5 seconds
      break;
  }
  return state;
}



int delay_gcd;
const unsigned short tasksNum = 6;
task tasks[tasksNum];

void setup() {      
  
  Serial.begin(9600);//why does this break TFT screen; 
  unsigned char i = 0; 
  tasks[i].state = readInputsOn;
  tasks[i].period = 25;
  tasks[i].elapsedTime = 2000;
  tasks[i].TickFct = &joystickSM;
  i++;
  tasks[i].state = idlePower;
  tasks[i].period = 25;
  tasks[i].elapsedTime = 2000;
  tasks[i].TickFct = &powerSM;
  i++;
  tasks[i].state = gameOff;
  tasks[i].period = 25;
  tasks[i].elapsedTime = 2000;
  tasks[i].TickFct = &projectScreens;
  i++;
  tasks[i].state = lcdOff;
  tasks[i].period = 25;
  tasks[i].elapsedTime = 2000;
  tasks[i].TickFct = &lcdSM;
  i++;
  tasks[i].state = gameNot;
  tasks[i].period = 25;
  tasks[i].elapsedTime = 2000;
  tasks[i].TickFct = &gamePlaySM;
  i++;
  tasks[i].state = EE_noUse;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 2000;
  tasks[i].TickFct = &EEPROM_SM;
  i++;

  // Initialize the TFT screen
  tft.init();
  tft.tftFill(0x0000);  //set to black

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
 
//  r.setSeed(1);

//  EE.resetDataInt(HS_ADDR);
//  EE.storeDataInt(HS_ADDR, 2);
}
void loop() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
}