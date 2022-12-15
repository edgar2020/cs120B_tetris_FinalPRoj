#include "TFTScreen.h"
//I am concerned that this approach will drain my memory very fast
//I am using muliple dimensional arrays very freely and if I am
// not careful I WILL run out of dynamic memory
#define DRK_BLUE 0xFFF000
class game
{
  public:
    static const char BOARD_HEIGHT = 20;
    static const char BOARD_WIDTH = 10;
    static const char NUM_PIECES = 7;
    static const char NUM_ROTATIONS = 4;
    static const char PIECE_WIDTH = 5;
    static const char PIECE_HEIGHT = 5;
    
    static const unsigned char GAME_X_POS = 42;
    static const unsigned char GAME_Y_POS = 38;
    

    
  private:
    //Square, Line, L, J, S, Z, T pieces in that order
    char pieceInitX = 3;
    char pieceInitY = -2;
    const unsigned char allPieces [NUM_PIECES][NUM_ROTATIONS][PIECE_HEIGHT][PIECE_WIDTH] =  {
      // Square Piece
      {
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 5, 5, 0},
          {0, 0, 5, 5, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 5, 5, 0},
          {0, 0, 5, 5, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 5, 5, 0},
          {0, 0, 5, 5, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 5, 5, 0},
          {0, 0, 5, 5, 0},
          {0, 0, 0, 0, 0}
        }
      },

      // Line Piece
      {
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 6, 6, 6, 6},
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 6, 0, 0},
          {0, 0, 6, 0, 0},
          {0, 0, 6, 0, 0},
          {0, 0, 6, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {6, 6, 6, 6, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 6, 0, 0},
          {0, 0, 6, 0, 0},
          {0, 0, 6, 0, 0},
          {0, 0, 6, 0, 0},
          {0, 0, 0, 0, 0}
        }
      },
      // L piece
      {
        
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 1, 1, 1, 0},
          {0, 1, 0, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 1, 1, 0, 0},
          {0, 0, 1, 0, 0},
          {0, 0, 1, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 1, 0},
          {0, 1, 1, 1, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 1, 0, 0},
          {0, 0, 1, 0, 0},
          {0, 0, 1, 1, 0},
          {0, 0, 0, 0, 0}
        }
      },
      // J piece
      {
        
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 4, 4, 4, 0},
          {0, 0, 0, 4, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 4, 0, 0},
          {0, 0, 4, 0, 0},
          {0, 4, 4, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 4, 0, 0, 0},
          {0, 4, 4, 4, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 4, 4, 0},
          {0, 0, 4, 0, 0},
          {0, 0, 4, 0, 0},
          {0, 0, 0, 0, 0}
        },
      },
      // Z piece
      {
        
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 3, 3, 0, 0},
          {0, 0, 3, 3, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 3, 0, 0},
          {0, 3, 3, 0, 0},
          {0, 3, 0, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 3, 3, 0, 0},
          {0, 0, 3, 3, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 3, 0},
          {0, 0, 3, 3, 0},
          {0, 0, 3, 0, 0},
          {0, 0, 0, 0, 0}
        }
      },
      // S piece
      {
        
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 2, 2, 0},
          {0, 2, 2, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 2, 0, 0, 0},
          {0, 2, 2, 0, 0},
          {0, 0, 2, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 2, 2, 0},
          {0, 2, 2, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 2, 0, 0},
          {0, 0, 2, 2, 0},
          {0, 0, 0, 2, 0},
          {0, 0, 0, 0, 0}
        }
      },
      // T piece
      {
        
        {
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 7, 7, 7, 0},
          {0, 0, 7, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 7, 0, 0},
          {0, 7, 7, 0, 0},
          {0, 0, 7, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 7, 0, 0},
          {0, 7, 7, 7, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0}
        },
        {
          {0, 0, 0, 0, 0},
          {0, 0, 7, 0, 0},
          {0, 0, 7, 7, 0},
          {0, 0, 7, 0, 0},
          {0, 0, 0, 0, 0}
        }
      }
    };

    char board[BOARD_HEIGHT][BOARD_WIDTH] =
        {
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 2},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//5
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//10
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//15
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
          {4, 0, 0, 0, 0, 0, 0, 0, 0, 3},//20
        };

//      tftScreen tft;
    unsigned char curPiece;
    char curX, curY;
    unsigned curR;

    unsigned char fallSpeed = 7;
    unsigned char count = 0;

    bool pieceStuck = false;

    public:
    game();
    ~game();
    void clearBoard();
    void drawGame(tftScreen *);
    void createPiece(char);
    void drawPiece(char, char, unsigned char);
    void drawPiece(char, char, unsigned char, bool);
    void drawPiece(char, char, unsigned char, unsigned char);
    void drawBoard(tftScreen *);
    void updateBoard(unsigned char);
    void erasePiece( char,  char, unsigned char);
    bool validMove( char,  char, unsigned char, unsigned char);
    
    bool broadcastStuckPiece();
    void setPieceStuck(bool b);
    bool checkGameOver();

    char clearLines();
    bool lineFull(char l);
    
};