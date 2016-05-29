#include "chesstypes.h"

Piece getForStartPos(BoardPos* pos) {
  if ( pos->row == blackHighPieceStartRow ) {
    return highPieceStartBlack[pos->column];
  } else if ( pos->row == blackPawnStartRow ) {
    return BlackPawn;
  } else if ( pos->row == whiteHighPieceStartRow ) {
    return highPieceStartWhite[pos->column];
  } else if ( pos->row == whitePawnStartRow ) {
    return WhitePawn;
  } else {
    return Empty;
  }
}

void setupEmptyBoard(BoardState* boardState) {
  int row;
  int column;
  for ( row = 0; row < ROWS; row++ ) {
    for ( column = 0; column < COLUMNS; column++ ) {
      boardState->squareStates[row][column] = Empty;
    }
  }
}

void setupStartPos(BoardState* boardState)  {
  boardState->active = White;
  boardState->halfMoveClock = 0;
  boardState->fullMoveCount = 1;
  boardState->enpassantAvailable.column = ColA;
  boardState->enpassantAvailable.row = Row1;
  boardState->canCastleRooks = 0 | WhiteKingSide | WhiteQueenSide |
    BlackKingSide | BlackQueenSide;
  BoardPos pos;
  int row;
  int column;
  for ( row = 0; row < ROWS; row++ ) {
    for ( column = 0; column < COLUMNS; column++ ) {
      pos.row = row;
      pos.column = column;
      boardState->squareStates[row][column] = getForStartPos(&pos);
    }
  }
}

bool isCastlingAvailable(
  CastlingAvailability castlingAvail, CastlingAvailOption castlingOpt)
{
  return (castlingAvail & castlingOpt) > 0;
}

void setCastlingAvailability(
  CastlingAvailability* castlingAvail, CastlingAvailOption castlingOpt, bool available)
{
  if ( available ) {
    *castlingAvail = (*castlingAvail) | castlingOpt;
  } else {
    *castlingAvail = (*castlingAvail) & (~castlingOpt);
  }
}