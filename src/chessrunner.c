#include "./chesstypes.h"
#include "./chessutils.h"
#include "./chessmoves.h"
#include "./chessrunner.h"

void setMode(
  ChessState *state,
  ChessMode mode) 
{
	// clean up some data structs?
	// do some other state keeping
	clearMoveBuffer(&(state->moveBuf));
	state->currMode = mode;
}

void doAction(
	ChessState *state,
	ChessAction action)
{
	// TODO
}

void handleSquareChange(
	ChessState *state,
	SquareChange *change)
{
	state->moveBuf.change.square.column = change->square.column;
	state->moveBuf.change.square.row = change->square.row;
	state->moveBuf.change.nowOccupied = change->nowOccupied;

	if ( state->currMode == Play ) {
		handleMoveBoardChange(
			&(state->moveBuf), &(state->board), &(state->game));
	} else if ( state->currMode == Setup ) {
		handleSetupBoardChange(
			&(state->moveBuf), &(state->board));
	}
}
