#include "./chesstypes.h"
#include "./chessutils.h"
#include "./chessmoves.h"
#include "./chessrunner.h"

// helper functions
void printPgn(ChessState*);
void printFen(ChessState*);
void bufferToOut(ChessState*);

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
	switch ( action ) {

		case PrintPgn:
			printPgn(state);
			break;

		case PrintFen:
			printFen(state);
			break;

	}
}

void printFen(ChessState *state)
{
	FEN fen;
	exportFen(&fen, &(state->board));

	exportFenToString(&fen, state->tempStrBuffer);
	
	bufferToOut(state);
}

void printPgn(ChessState *state)
{
	int lenWritten = writePgnMoves(state->tempStrBuffer, &(state->game), 
  		writeMoveLan,
  		state->moveCommentFormatter);
	if ( lenWritten >= state->tempStrBufferLen ) {
		// oops... should the methods take max?
		// for now let's go with that it is enough 
		// that we trust the buffer to be large enough...
	}
	state->tempStrBuffer[lenWritten] = '\0';
	bufferToOut(state);
}

void bufferToOut(ChessState *state)
{
	(*state->outputPrinter)(state->tempStrBuffer);
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
