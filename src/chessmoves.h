#ifndef chessmoves_H
#define chessmoves_H

#include "./chesstypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// this can be called to tell that the pieces currently 
// on the move should be considered discarded off the board
extern void handleSetupMoveTimeout(
  MoveBuffer *moveBuf, BoardState *boardState);

extern bool handleSetupBoardChange(
	MoveBuffer *moveBuf, BoardState *boardState);

extern bool handleMoveBoardChange(
  MoveBuffer *moveBuf, BoardState *boardState, ChessGame *currGame);

extern void handleMoveFinished(
  ChessGame *currGame, BoardState *boardState,
  MoveType type, Column startCol, Row startRow, 
  Column endCol, Row endRow,
  Piece activePiece, Piece passivePiece);

extern void clearMoveBuffer(MoveBuffer *moveBuf);

extern bool isMoveBufferEmpty(MoveBuffer *moveBuf);

extern bool isPendingPromotionMarkSet(MoveBuffer *moveBuf);

extern void clearPendingPromotionMark(MoveBuffer *moveBuf);

extern void setPendingPromotionMark(MoveBuffer *moveBuf);

#ifdef __cplusplus
}
#endif

#endif // chessmoves_H
