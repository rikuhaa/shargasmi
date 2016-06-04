#include "./chesstypes.h"
#include "./chessutils.h"
#include "./chessmoves.h"
#include "./chessrunner.h"

#include <stddef.h>

// helper functions
void printPgn(ChessState*);
void printFen(ChessState*);
void bufferToOut(ChessState*);

bool testSquares(ChessState*, bool (*condition)(ChessState*, Row, Column));

bool startGameOccChecker(ChessState*, Row, Column);
bool checkNoneOccupied(ChessState*, Row, Column);
bool checkNonEmptySquaresOccupied(ChessState*, Row, Column);

void handleStartAction(ChessState*);

/*
typedef struct chessstate {

	ChessGame game;
	BoardState board;

	MoveBuffer moveBuf;

	ChessMode currMode;

	bool
	
	void (*outputPrinter) (char*);

	void (*errorHandler) (ChessErrorType, char*);

	int (*moveCommentFormatter) (char*, ChessGame*, int);

	long (*timeStamper) (void);

	char* tempStrBuffer;

	int tempStrBufferLen;

} ChessState;
*/

void initEmptyChessState(
	ChessState *state, char *tempStrBuffer, int tempStrBufferLen) 
{

	setupEmptyBoard(&(state->board));
	state->game.finMovesCount = 0;

	clearMoveBuffer(&(state->moveBuf));
	state->currMode = Play;
	state->currState = AfterReset;

	state->isOccupied = NULL;
	state->outputPrinter = NULL;
	state->errorHandler = NULL;
	state->moveCommentFormatter = NULL;
	state->timeStamper = NULL;

	// TODO check that tempStrBufferLen is 
	// at least relatively sane

	state->tempStrBuffer = tempStrBuffer;
	state->tempStrBufferLen = tempStrBufferLen;
}

void setRunnerMode(
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

		case PlayAction:
			handleStartAction(state);
			break;

	}
}

void handleResetAction(ChessState *state) 
{
	state->currState = AfterReset;
	// TODO if playing:
	// - reset clock?
	// - move the pgn to old 
	// games buffer
	// - 

	// most of this can though also be 
	// done just before continuing the game

	state->game.finMovesCount = 0;
	clearMoveBuffer(&(state->moveBuf));
}

void handlePauseAction(ChessState *state) 
{
	// pause the clock somehow
	// or maybe implement most of the clock
	// functionality by move start times
	// to begin with
	state->currState = Paused;
}

void handleStartAction(ChessState *state)
{
	
	if ( state->currState == AfterReset ) {
		if ( state->currMode == Play ) {
		
			setupStartPos(&(state->board));

			// reset clock to zero too, and 
			// then begin from that when 
			// changing to Running again 
			// (when continuing from Paused 
			// the clock is also started
			// but not reset)

		} else if ( state->currMode == Setup ) {

			setupEmptyBoard(&(state->board));

		}
	}

	bool ok = testSquares(state, &checkNonEmptySquaresOccupied);

	if ( ok ) {
		state->currState = Running;
	} else {
		// TODO signal error somehow
		// could just use the error handler
	}

	// this should check that 
	// board state looks ok (versus occupied)
	// and then start the timer
	// and get the game going

}

bool testSquares(ChessState* state, bool (*condition)(ChessState*, Row, Column))
{
	Row row;
	Column col;

	bool wasOk = true;

	for ( row = Row1; row <= Row8; row++ ) {
		for ( col = ColA; col <= ColH; col++ ) {
			bool subRes = 
				(*condition)(state, row, col);
			if ( ! subRes ) {
				wasOk = false;
				break;
			}
		}
	}

	return wasOk;
}

bool checkNonEmptySquaresOccupied(ChessState* state, Row row, Column col)
{

	bool occupied = (*(state->isOccupied))(row, col);
	Piece inSquare = getPieceRowCol(&(state->board), row, col);
	// TODO could only check that piece is not unknown
	if ( inSquare == Empty ) {
		return ! occupied;
	} else {
		return occupied;
	}
}

bool startGameOccChecker(ChessState* state, Row row, Column col)
{
	BoardPos pos;
	pos.column = col;
	pos.row = row;
	bool occupied = (*(state->isOccupied))(row, col);
	if ( getPieceForStartPos(&pos) == Empty ) {
		return ! occupied;
	} else {
		return occupied;
	}
}


bool checkNoneOccupied(ChessState* state, Row row, Column col)
{
	return ! (*(state->isOccupied))(row, col);
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

	if ( state->currState == Running ) {

		state->moveBuf.change.square.column = change->square.column;
		state->moveBuf.change.square.row = change->square.row;
		state->moveBuf.change.nowOccupied = change->nowOccupied;

		if ( state->currMode == Play ) {
			bool fullMoveReady = handleMoveBoardChange(
				&(state->moveBuf), &(state->board), &(state->game));
			if ( fullMoveReady ) {
				// TODO: if not promotion..?
				int writtenChars = 
					writeMoveUci(state->tempStrBuffer, 
						&(state->game.moves[state->game.finMovesCount - 1]));
				state->tempStrBuffer[writtenChars] = '\0';
				bufferToOut(state);

			}
		} else if ( state->currMode == Setup ) {
			handleSetupBoardChange(
				&(state->moveBuf), &(state->board));
			// should here also the fen be printed
			// on all ready full moves?
		}

	}
}
