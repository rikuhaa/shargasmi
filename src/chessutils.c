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

void importFEN(FEN* fen, BoardState* boardState) 
{

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

  // Row currRow = Row8;
  // Column currCol = ColA;

  // int fenInd = 0;

  // while ( fenInd < FEN_POS_MAX_CHARS && 
  //   ! (currCol > ColH && currRow <= Row1) ) {

  //   char nextFenCh = fen->piecePlaces[fenInd];

  //   Piece toPlace = Unknown;
  //   if ( nextFenCh == '/' ) {
  //     currRow--;
  //     currCol = ColA;
  //   } else if ( '1' <= nextFenCh && nextFenCh <= '9' ) {
  //     int numOfEmpty = nextFenCh - '0';

  //     toPlace = Empty;
  //     for ( int i = 0; i < numOfEmpty - 1; i++) {
  //       boardState->squareStates[currRow][currCol] = toPlace;
  //       currCol++;
  //     }
  //     // last empty is filled with normal piece placing
  //   } else {
  //     toPlace = getPieceFromFENName(nextFenCh);
  //   }

  //   if ( toPlace != Unknown ) {
  //     boardState->squareStates[currRow][currCol] = toPlace;
  //     currCol++;
  //   }

  //   fenInd++;
  // }


}