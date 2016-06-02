#ifndef chesstypes_H
#define chesstypes_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COLUMNS 8
#define ROWS 8
#define MAX_MOVES_PER_GAME 500

// 8 per row, 7 times '/' and '/0' to end the string
#define FEN_POS_MAX_CHARS 72
// there is now real upper limit, but this is a reasonable max
// ie. the move count could need N digits but in practice it does not
#define FEN_WHOLE_STR_MAX_CHARS 100 

/*
typedef enum {
  false,
  true
} bool;
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
  Row row;
  Column column;
} BoardPos;

extern bool isSameSquare(BoardPos *posA, BoardPos *posB);

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

  Column castlingFirstReturned;

} MoveBuffer;

static const Row blackHighPieceStartRow = Row8;
static const Row blackPawnStartRow = Row7;

static const Row whiteHighPieceStartRow = Row1;
static const Row whitePawnStartRow = Row2;

static const Row blackPiecePromotionRow = Row1;
static const Row whitePiecePromotionRow = Row8;

static const Row whiteEnpassantRow = Row4;
static const Row blackEnpassantRow = Row5;

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

typedef unsigned int CastlingAvailability;

typedef enum castlingavailopt {
  WhiteKingSide = 0x01,
  WhiteQueenSide = 0x02,
  BlackKingSide = 0x04,
  BlackQueenSide = 0x08,
} CastlingAvailOption;

extern bool isCastlingAvailable(
  CastlingAvailability castlingAvail, CastlingAvailOption castlingOpt);

extern void setCastlingAvailability(
  CastlingAvailability *castlingAvail, 
  CastlingAvailOption castlingOpt, bool available);

extern void setNoCastlingAvailability(CastlingAvailability *castlingAvail);

typedef struct {
  Piece squareStates[ROWS][COLUMNS];
  
  Player active;
  
  CastlingAvailability canCastleRooks;
  
  BoardPos enpassantAvailable;
  
  unsigned int halfMoveClock;
  
  unsigned int fullMoveCount;

} BoardState;

typedef struct {

  char piecePlaces[FEN_POS_MAX_CHARS];

  Player activePlayer;

  CastlingAvailability canCastleRooks;

  BoardPos enpassantAvailable;

  unsigned int halfMoveClock;

  unsigned int fullMoveCount;

} FEN;

#ifdef __cplusplus
}
#endif

#endif // chesstypes_H
