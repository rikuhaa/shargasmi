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