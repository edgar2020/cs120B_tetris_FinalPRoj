#include "game.h"
#include "TFTScreen.h"


game::game()
{
}
game::~game()
{
  for(int i = 0; i < NUM_PIECES; ++i)
  {
      for(int j = 0; j <NUM_ROTATIONS; ++j)
      {
        for(int k = 0; k < PIECE_HEIGHT; ++j)
        {
          delete[] allPieces[i][j][k];
        }
        delete[] allPieces[i][j];        
      }
    delete[] allPieces[i];
  }
  delete[] allPieces;
}

void game::clearBoard()
{
  
  for(char i = 0; i < BOARD_HEIGHT; i++)
  {
    for(char j = 0; j < BOARD_WIDTH; j++)
    {
      board[i][j] = 0;
    }
  }
}

void game::drawGame(tftScreen *t)
{
  t->gameBoard(board, 4, GAME_X_POS+5, GAME_Y_POS);
}
void game::drawBoard(tftScreen *t)
{
  //clear background
    t->setBox(30, 104, 35, 105);
    t->tftFill(0x000000);
    //LEFT pillar
    t->setBox(GAME_X_POS, GAME_X_POS+4, GAME_Y_POS, GAME_Y_POS+BOARD_HEIGHT*4+5);
    t->tftFill(DRK_BLUE);
    //right piller
    t->setBox(GAME_X_POS+(BOARD_WIDTH+2)*4-1, GAME_X_POS+(BOARD_WIDTH+2)*4+3, GAME_Y_POS, GAME_Y_POS+BOARD_HEIGHT*4+5);
    t->tftFill(DRK_BLUE);
    //bottom piller
    t->setBox(GAME_X_POS, GAME_X_POS+(BOARD_WIDTH+2)*4+3, GAME_Y_POS+BOARD_HEIGHT*4+1, GAME_Y_POS+BOARD_HEIGHT*4+5);
    t->tftFill(DRK_BLUE);
    
    drawGame(t);
}

void game::createPiece(char r)
{
  curPiece = r; 
  curX = pieceInitX;
  curY = pieceInitY;
  curR = 0;
}
void game::drawPiece( char X,  char Y, unsigned char P, unsigned char R)
{
  
  for(char i = 0; i < PIECE_HEIGHT; i++)
  {
    for(char j = 0; j <PIECE_WIDTH; j++)
    {
      if(allPieces[P][R][i][j])
      {
        board[Y+i][X+j] = allPieces[P][R][i][j];
      }
    }
  }
}

void game::drawPiece( char X, char Y, unsigned char R)
{
  
  for(char i = 0; i < PIECE_HEIGHT; i++)
  {
    for(char j = 0; j <PIECE_WIDTH; j++)
    {
      if(allPieces[curPiece][R][i][j])
      {
        board[Y+i][X+j] = allPieces[curPiece][R][i][j];
      }
    }
  }
}

void game::drawPiece( char X, char Y, unsigned char R, bool Permanent)
{
  
  for(char i = 0; i < PIECE_HEIGHT; i++)
  {
    for(char j = 0; j <PIECE_WIDTH; j++)
    {
      if(allPieces[curPiece][R][i][j])
      {
        board[Y+i][X+j] = (0x8 | allPieces[curPiece][R][i][j]);
      }
    }
  }
}


void game::erasePiece( char X,  char Y, unsigned char R)
{
  for(char i = 0; i < PIECE_HEIGHT; i++)
  {
    for(char j = 0; j <PIECE_WIDTH; j++)
    {
      if(allPieces[curPiece][R][i][j])
      {
        board[Y+i][X+j] = 0;
      }
    }
  }
}

void game::updateBoard(unsigned char JS)
{
//  Serial.println("rtes");
  count++;
  unsigned char tempX = curX;
  unsigned char tempY = curY;
  unsigned char tempR = curR;
  
//give out of bounds error, need to fix by checking if a move is valid
  if( (JS == 0  ) && validMove(curX, curY, (curR+1)%4, 3))
  {
   curR = (curR+1)%4;
  }
  else if(JS == 1 && validMove(curX+1, curY, curR, 2))
  {
    curX++;
  }
  else if(JS == 3 && validMove(curX-1, curY, curR, 1))
  {
    curX--;
  }

//adjust fall speed/ quick fall
  if(JS == 2 )
  {
    fallSpeed = 0;
  }
  else
  {
    fallSpeed = 7;
  }
  
  
  if(count >= fallSpeed && !validMove(curX, curY+1, curR, 0))//save position and make new piece
  {
   setPieceStuck(1);
   count=0;
  }
  else if( count >= fallSpeed && validMove(curX, curY+1, curR, 0))
  {
    count=0;
    curY++;
  }
  erasePiece(tempX, tempY, tempR); 
  if(!broadcastStuckPiece())
  {
     
    drawPiece(curX, curY, curR);
  }
  else 
  {
    drawPiece(curX, curY, curR, (bool)1);
  }
  
}

//direct 0 = down, 1 = left, 2 = right, 3 = rotate
bool game::validMove( char X,  char Y, unsigned char R, unsigned char MV)
{
  for(char i = 0; i < PIECE_HEIGHT; i++)
  {
    for(char j = 0; j < PIECE_WIDTH; j++)
    {
      if(allPieces[curPiece][R][i][j])//part of the piece exists here
      {
        /**
         * Boundries
         */
        if(j+X<0)//if hits left wall
          return 0;
        if(j+X>9)//if hits right wall
          return 0;
        if(i+Y>19)//if hits bottom
          return 0;
        if(i+Y<0)//if roof (only happens if rotate instatly upon loading block
        {
//          setPieceStuck(1);
          return 0;
        }
        /**
         * Other Blocks
         */
        if(board[Y+i][X+j] & 8)
        {
          if(!MV)
            setPieceStuck(1);
          return 0;
        }
      }
    }
  }
  return 1;
}


bool game::broadcastStuckPiece()
{
  return pieceStuck;
}
void game::setPieceStuck(bool b)
{
  pieceStuck = b;
}
bool game::checkGameOver()
{
  for(int i = 0; i < BOARD_WIDTH; i++)
  {
    if(board[0][i] & 0x8)
      return 1;
  }
  return 0;
}

char game::clearLines()
{

  char numLines = 0;
  unsigned char score = 0;
  char row = 19;
  char c = 0;
  while(row >= 0 && !lineFull(row))//find first row to clear
  {
    row--;
  }
  if(lineFull(row))
  {
    score = 1;
    numLines=1;
    for(char i = 1; ((row-i >= 0) && (i < 4)); i++)//find how many rows to clear
    {
      if(lineFull(row - i))
      {
        numLines++;
        score*=2;
      }
    }

    for(char i = row; (i-numLines)>= 0; i--)//move rows down
    {
      for(char j = 0; j < BOARD_WIDTH; j++)
      {
        board[i][j] = board[i-numLines][j]; 
      }
    }
    for(char i = 0; i < numLines; i++)//create new empty rows
    {
      for(char j = 0; j < BOARD_WIDTH; j++)
      {
        board[i][j] = 0; 
      }
    }
    
    return score;
  }
  else
    return 0;  
}

bool game::lineFull(char l)
{
  char c = 0;
  for(char i = 0; i < BOARD_WIDTH; i++)
  {
    if(board[l][i] & 0x8)
      c++;
  }
  if(c == 10)
  {
//    board[1][1] = 0b0101;
    return true;
  }
  else
    return false;
}