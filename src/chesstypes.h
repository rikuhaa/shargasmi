#ifndef chesstypes_H
#define chesstypes_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COLUMNS 8
#define ROWS 8
#define MAX_MOVES_PER_GAME 500

/*
typedef enum {
  false,
  true
} bool;
*/

/*
const int COLUMNS = 8;
const int ROWS = 8;

const int MAX_MOVES_PER_GAME = 500;
*/

typedef enum player {
  Black,
  White,
  Neutral
} Player;

typedef enum column {
  ColA,
  ColB,
  ColC,
  ColD,
  ColE,
  ColF,
  ColG,
  ColH
} Column;

typedef enum row {
  Row1,
  Row2,
  Row3,
  Row4,
  Row5,
  Row6,
  Row7,
  Row8
} Row;

typedef enum piece {
  Empty,
  WhiteQueen,
  WhiteKing,
  WhiteBishop,
  WhiteRook,
  WhiteKnight,
  WhitePawn,
  BlackQueen,
  BlackKing,
  BlackBishop,
  BlackRook,
  BlackKnight,
  BlackPawn,
  Unknown
} Piece;

typedef struct {
  Piece squareStates[ROWS][COLUMNS];
  Player active;
} BoardState;

typedef struct {
  Row row;
  Column column;
} BoardPos;

typedef struct {

  Piece piece;
  BoardPos startPos;
  
} LiftedPiece;

typedef struct {

  BoardPos square;
  bool nowOccupied;
  
} SquareChange;

typedef struct {

  LiftedPiece firstLifted;
  LiftedPiece secondLifted;

  SquareChange change;

} MoveBuffer;

static const Row blackHighPieceStartRow = Row8;
static const Row blackPawnStartRow = Row7;

static const Row whiteHighPieceStartRow = Row1;
static const Row whitePawnStartRow = Row2;

static const Piece highPieceStartBlack[] = {BlackRook, BlackKnight, 
  BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook};

static const Piece highPieceStartWhite[] = {WhiteRook, WhiteKnight, 
  WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook};

typedef enum movetype {

  Move,
  Capture, // en passant is bit special but well covered here
  Castling,
  // these divisions are only to avoid adding extra field
  // to ChessMove struct when promoting with capture...
  PromotionQueen,
  PromotionRook,
  PromotionBishop,
  PromotionKnight

} MoveType;

// if there will be mem problems,
// this data struct could quite easily be packed

typedef struct {

  Piece activePiece;
  Piece passivePiece;

  BoardPos startSquare;
  BoardPos endSquare;

  MoveType type;

} ChessMove;

typedef struct {

  int finMovesCount;
  
  ChessMove moves[MAX_MOVES_PER_GAME];

} ChessGame;

extern void setupEmptyBoard(BoardState* boardState);

extern void setupStartPos(BoardState* boardState);

/*
* Takes a position on the board (row, colum) and returns
* which piece is on that square when the board is setup 
* correctly.
* Can return Empty
*/
extern Piece getForStartPos(BoardPos* pos);

#ifdef __cplusplus
}
#endif

#endif // chesstypes_H
