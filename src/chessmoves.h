#ifndef chessmoves_H
#define chessmoves_H

#include "./chesstypes.h"

/*
enum MoveState {

  INIT,
  ONE_MOVING,
  CAPTURING,
  CASTLING,
  

}
*/

extern ChessMove *handleMoveBoardChange(
  MoveBuffer *moveBuf, BoardState *boardState, ChessGame *currGame);

extern void clearMoveBuffer(MoveBuffer *moveBuf);

#endif // chessmoves_H
