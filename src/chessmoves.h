#ifndef chessmoves_H
#define chessmoves_H

#include "./chesstypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
enum MoveState {

  INIT,
  ONE_MOVING,
  CAPTURING,
  CASTLING,
  

}
*/

extern void handleMoveBoardChange(
  MoveBuffer *moveBuf, BoardState *boardState, ChessGame *currGame);

extern void clearMoveBuffer(MoveBuffer *moveBuf);

#ifdef __cplusplus
}
#endif

#endif // chessmoves_H
