#include "chesstypes.h"

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

bool isSameSquare(BoardPos *posA, BoardPos *posB)
{
	return (posA->column == posB->column) && (posA->row == posB->row);
}

void setNoCastlingAvailability(CastlingAvailability *castlingAvail)
{
  *castlingAvail = 0;
}


void initChessGame(ChessGame *game, 
  ChessMove *moves, int maxMoves)
{
  game->moves = moves;
  game->maxMovesPerGame = maxMoves;
  game->finMovesCount = 0;
}