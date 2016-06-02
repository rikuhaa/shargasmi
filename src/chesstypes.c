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


void setNoCastlingAvailability(CastlingAvailability *castlingAvail)
{
  *castlingAvail = 0;
}