#include <stdio.h>
#include "./chesstypes.h"
#include "./chessutils.h"

Piece getPiece(BoardState* boardState, BoardPos* boardPos)
{
  return boardState->squareStates[boardPos->row][boardPos->column];
}

Piece getPieceRowCol(BoardState* boardState, Row row, Column column) {
  return boardState->squareStates[row][column];
}

Piece swapPiece(Piece newPiece, BoardState* boardState, BoardPos* boardPos)
{
  Piece oldPiece = 
    boardState->squareStates[boardPos->row][boardPos->column];
  boardState->squareStates[boardPos->row][boardPos->column] = newPiece;
  return oldPiece;
}

bool belongsToPlayer(Player player, Piece piece) 
{
  if ( player == Black ) {
    return piece == BlackQueen || piece == BlackKing ||
      piece == BlackBishop || piece == BlackRook || piece == BlackKnight ||
      piece == BlackPawn;
  } else if ( player == White ) {
    return piece == WhiteQueen || piece == WhiteKing ||
      piece == WhiteBishop || piece == WhiteRook || piece == WhiteKnight ||
      piece == WhitePawn;
  } else {
    return false;
  }
}

/*
*
*/
char* getSAN(ChessMove* move)
{
  static char test = 'X';
  return &test;
}

char getPieceFENName(Piece piece) 
{

  switch(piece) {
    case WhitePawn:
      return 'P';
    case BlackPawn:
      return 'p';
    case WhiteKnight:
      return 'N';
    case BlackKnight:
      return 'n';
    case WhiteBishop:
      return 'B';
    case BlackBishop:
      return 'b';
    case WhiteRook:
      return 'R';
    case BlackRook:
      return 'r';
    case WhiteQueen:
      return 'Q';
    case BlackQueen:
      return 'q';
    case WhiteKing:
      return 'K';
    case BlackKing:
      return 'k';
    default:
      // error?
      return 'X';
  }

}

Piece getPieceFromFENName(char name) 
{

  switch(name) {
    case 'P':
      return WhitePawn;
    case 'p':
      return BlackPawn;
    case 'N':
      return WhiteKnight;
    case 'n':
      return BlackKnight;
    case 'B':
      return WhiteBishop;
    case 'b':
      return BlackBishop;
    case 'R':
      return WhiteRook;
    case 'r':
      return BlackRook;
    case 'Q':
      return WhiteQueen;
    case 'q':
      return BlackQueen;
    case 'K':
      return WhiteKing;
    case 'k':
      return BlackKing;
    default:
      // error?
      return Unknown;
  }

}

char getRowName(Row row) 
{

  switch(row) {
    case Row1:
      return '1';
    case Row2:
      return '2';
    case Row3:
      return '3';
    case Row4:
      return '4';
    case Row5:
      return '5';
    case Row6:
      return '6';
    case Row7:
      return '7';
    case Row8:
      return '8';
    default:
      // error?
      return 'X';
  }

}

char getColumnName(Column column) {

  switch(column) {
    case ColA:
      return 'a';
    case ColB:
      return 'b';
    case ColC:
      return 'c';
    case ColD:
      return 'd';
    case ColE:
      return 'e';
    case ColF:
      return 'f';
    case ColG:
      return 'g';
    case ColH:
      return 'h';
    default:
      // error?
      return 'X';
  }

}

void importFEN(FEN* fen, BoardState* boardState) 
{

  boardState->active = fen->activePlayer;
  boardState->canCastleRooks = fen->canCastleRooks;

  boardState->halfMoveClock = fen->halfMoveClock;
  boardState->fullMoveCount = fen->fullMoveCount;

  // this is set to A1 (never legal en passant) if not available
  boardState->enpassantAvailable.row = fen->enpassantAvailable.row;
  boardState->enpassantAvailable.column = fen->enpassantAvailable.column;

  Row currRow = Row8;
  Column currCol = ColA;

  int fenInd = 0;

  while ( fenInd < FEN_POS_MAX_CHARS && 
    ! (currCol > ColH && currRow <= Row1) ) {

    char nextFenCh = fen->piecePlaces[fenInd];

    Piece toPlace = Unknown;
    if ( nextFenCh == '/' ) {
      currRow--;
      currCol = ColA;
    } else if ( '1' <= nextFenCh && nextFenCh <= '9' ) {
      int numOfEmpty = nextFenCh - '0';

      toPlace = Empty;
      for ( int i = 0; i < numOfEmpty - 1; i++) {
        boardState->squareStates[currRow][currCol] = toPlace;
        currCol++;
      }
      // last empty is filled with normal piece placing
    } else {
      toPlace = getPieceFromFENName(nextFenCh);
    }

    if ( toPlace != Unknown ) {
      boardState->squareStates[currRow][currCol] = toPlace;
      currCol++;
    }

    fenInd++;
  }

  // TODO import also active player, 

  // maybe even 
  // castling and en passant availability
  // even though that's not the main goal
  // ie. actually checking rules is not the point
  // and moves that look like castling or en passant
  // can be sent without knowing if they were legal

}

void exportFEN(FEN* fen, BoardState* boardState) {

  fen->activePlayer = boardState->active;
  fen->canCastleRooks = boardState->canCastleRooks;

  fen->halfMoveClock = boardState->halfMoveClock;
  fen->fullMoveCount = boardState->fullMoveCount;

  // this is set to A1 (never legal en passant) if not available
  fen->enpassantAvailable.row = boardState->enpassantAvailable.row;
  fen->enpassantAvailable.column = boardState->enpassantAvailable.column;

  int currEmptyCount = 0;
  int fenInd = 0;
  for ( int row = Row8; row >= Row1; row--) { 
    for ( int col = ColA; col <= ColH; col++) {

      Piece pieceInPos = getPieceRowCol(boardState, row, col);

      if ( pieceInPos == Empty ) {
        currEmptyCount++;
      } else {

        if ( currEmptyCount != 0 ) {
          char emptyCountChar = '0' + currEmptyCount;
          fen->piecePlaces[fenInd] = emptyCountChar;
          fenInd++;
          currEmptyCount = 0;
        }

        char pieceChar = getPieceFENName(pieceInPos);
        fen->piecePlaces[fenInd] = pieceChar;
        fenInd++;

      }

    }

    if ( currEmptyCount != 0 ) {
      char emptyCountChar = '0' + currEmptyCount;
      fen->piecePlaces[fenInd] = emptyCountChar;
      fenInd++;
      currEmptyCount = 0;
    }

    if ( row != Row1 ) {
      fen->piecePlaces[fenInd] = '/';
      fenInd++;
    }
  }

  fen->piecePlaces[fenInd] = '\0';

}
 
void exportFenToString(FEN* fen, char* fenString)
{
  // position
  int resStrIndex = 0;
  bool done = false;
  int fenPosIndex = 0;

  for ( fenPosIndex = 0; fenPosIndex < FEN_POS_MAX_CHARS &&
    !done; fenPosIndex++ ) {

    char next = fen->piecePlaces[fenPosIndex];
    if ( next != '\0' ) {
      fenString[resStrIndex] = next;
      resStrIndex++;
    } else {
      done = true;
    }
  }

  fenString[resStrIndex] = ' ';
  resStrIndex++;

  if ( fen->activePlayer == Black ) {
    fenString[resStrIndex] = 'b';
  } else {
    fenString[resStrIndex] = 'w';
  }
  resStrIndex++;

  fenString[resStrIndex] = ' ';
  resStrIndex++;

  // castling
  bool someCastling = false;

  // castling availability order: first upper case, then lower case, 
  // secondly first king side, then queen side

  // add available castling options in correct order
  if ( isCastlingAvailable(fen->canCastleRooks, WhiteKingSide) ) {
    fenString[resStrIndex] = 'K';
    resStrIndex++;
    someCastling = true;
  }
  
  if ( isCastlingAvailable(fen->canCastleRooks, WhiteQueenSide) ) {
    fenString[resStrIndex] = 'Q';
    resStrIndex++;
    someCastling = true;
  }

  if ( isCastlingAvailable(fen->canCastleRooks, BlackKingSide) ) {
    fenString[resStrIndex] = 'k';
    resStrIndex++;
    someCastling = true;
  }
  
  if ( isCastlingAvailable(fen->canCastleRooks, BlackQueenSide) ) {
    fenString[resStrIndex] = 'q';
    resStrIndex++;
    someCastling = true;
  }

  // if there were no castling options, add '-' instead
  if ( !someCastling ) {
    fenString[resStrIndex] = '-';
    resStrIndex++;
  }

  fenString[resStrIndex] = ' ';
  resStrIndex++;

  // en passant

  if ( fen->enpassantAvailable.row == Row1 ) {
    // this row can never have legal en passant squeare, used as marker
    fenString[resStrIndex] = '-';
    resStrIndex++;
  } else {
    fenString[resStrIndex] = getColumnName(fen->enpassantAvailable.column);
    resStrIndex++;
    fenString[resStrIndex] = getRowName(fen->enpassantAvailable.row);
    resStrIndex++;
  }

  fenString[resStrIndex] = ' ';
  resStrIndex++;

  // half move clock

  char intCharBuf[6];
  int intBufInd = 0;
  done = false;

  sprintf(intCharBuf,
        "%i", fen->halfMoveClock);

  for ( intBufInd = 0; 
    intBufInd < 6 && !done; intBufInd++) {

    char next = intCharBuf[intBufInd];
    if ( next != '\0' ) {
      fenString[resStrIndex] = next;
      resStrIndex++;
    } else {
      done = true;
    }
  }

  fenString[resStrIndex] = ' ';
  resStrIndex++;

  // full move count

  done = false;

  sprintf(intCharBuf,
        "%i", fen->fullMoveCount);

  for ( intBufInd = 0; 
    intBufInd < 6 && !done; intBufInd++) {

    char next = intCharBuf[intBufInd];
    if ( next != '\0' ) {
      fenString[resStrIndex] = next;
      resStrIndex++;
    } else {
      done = true;
    }
  }

  fenString[resStrIndex] = '\0';

}

void importFenFromString(FEN* fen, char* fenString)
{

}