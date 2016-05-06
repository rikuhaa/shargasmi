#include "./chesstypes.h"
#include "./chessutils.h"

Piece getPiece(BoardState* boardState, BoardPos* boardPos)
{
  return boardState->squareStates[boardPos->row][boardPos->column];
}

Piece swapPiece(Piece newPiece, BoardState* boardState, BoardPos* boardPos)
{
  Piece oldPiece = 
    boardState->squareStates[boardPos->row][boardPos->column];
  boardState->squareStates[boardPos->row][boardPos->column] = newPiece;
  return oldPiece;
}

bool belongsToPlayer(Player player, Piece piece) {
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

char getPieceSANName(Piece piece) {

  if ( piece == WhitePawn || piece == BlackPawn ) {
    return '\0';
  } else if ( piece == WhiteKnight || piece == BlackKnight ) {
    return 'N';
  } else if ( piece == WhiteQueen || piece == BlackQueen ) {
    return 'Q';
  } else if ( piece == WhiteBishop || piece == BlackBishop ) {
    return 'B';
  } else if ( piece == WhiteRook || piece == BlackRook ) {
    return 'R';
  } else if ( piece == WhiteKing || piece == BlackKing ) { 
    return 'K';
  } else {
    // error
    return 'X';
  }

}

