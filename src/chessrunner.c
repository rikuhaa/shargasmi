#include "./chesstypes.h"
#include "./chessutils.h"
#include "./chessmoves.h"
#include "./chessrunner.h"

#include <stddef.h>

// helper functions
void printPgnLong(ChessState*);
void printPgn(ChessState*);
void printFen(ChessState*);
void bufferToOut(ChessState*);

bool testSquares(ChessState*, bool (*condition)(ChessState*, Row, Column));

bool startGameOccChecker(ChessState*, Row, Column);
bool checkNoneOccupied(ChessState*, Row, Column);
bool checkNonEmptySquaresOccupied(ChessState*, Row, Column);

void handleStartAction(ChessState*);
void tryRollPromotion(ChessState*);

void testFlushPendingPromotionMove(ChessState *state);

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
	state->timeHandler = NULL;

	// TODO check that tempStrBufferLen is 
	// at least relatively sane

	state->tempStrBuffer = tempStrBuffer;
	state->tempStrBufferLen = tempStrBufferLen;
}

void setRunnerMode(
  ChessState *state,
  ChessMode mode) 
{
	testFlushPendingPromotionMove(state);
	// TODO pending promotion should probably 
	// be checked also here, and maybe even when pausing
	// to avoid losing the promotion move

	// clean up some data structs?
	// do some other state keeping
	clearMoveBuffer(&(state->moveBuf));
	state->currMode = mode;
}

void doAction(
	ChessState *state,
	ChessAction action)
{
	if ( action != RollPromotionAction ) {
		testFlushPendingPromotionMove(state);
	}

	switch ( action ) {

		case PrintPgnLong:
			printPgnLong(state);
			break;

		case PrintPgn:
			printPgn(state);
			break;

		case PrintFen:
			printFen(state);
			break;

		case PlayAction:
			handleStartAction(state);
			break;

		case RollPromotionAction:
			tryRollPromotion(state);
			break;
	}
}

void tryRollPromotion(ChessState *state)
{
	// promotion type can be rolled only
	// just after promotion move finished
	// and before any other move
	// has been started (empty buffer)

	if ( state->moveBuf.firstLifted.piece == Empty &&
		state->moveBuf.secondLifted.piece == Empty &&
		latestMoveIsPromotion(&(state->game)) ) {
		ChessMove *latestMove = getLatestFinMove(&(state->game));
		
		if ( latestMove->type == PromotionQueen ) {
			latestMove->type = PromotionKnight;
		} else if ( latestMove->type == PromotionKnight ) {
			latestMove->type = PromotionRook; 
		} else if ( latestMove->type == PromotionRook ) {
			latestMove->type = PromotionBishop;
		} else {
			latestMove->type = PromotionQueen;
		}
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

void printPgnLong(ChessState *state)
{
	int lenWritten = writePgnMoves(state->tempStrBuffer, &(state->game), 
  		writeMoveLan,
  		writeTimeStampInfo);
	if ( lenWritten >= state->tempStrBufferLen ) {
		// oops... should the methods take max?
		// for now let's go with that it is enough 
		// that we trust the buffer to be large enough...
	}
	state->tempStrBuffer[lenWritten] = '\0';
	bufferToOut(state);
}

void printPgn(ChessState *state)
{
	int lenWritten = writePgnMoves(state->tempStrBuffer, &(state->game), 
  		writeMoveLan,
  		NULL);
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

void printLatestMoveUci(ChessState *state)
{
	int writtenChars = 
		writeMoveUci(state->tempStrBuffer, 
			getLatestFinMove(&(state->game)));
	state->tempStrBuffer[writtenChars] = '\0';
	bufferToOut(state);
}

void handleSquareChange(
	ChessState *state,
	SquareChange *change)
{

	if ( state->currState == Running ) {

		testFlushPendingPromotionMove(state);

		state->moveBuf.change.square.column = change->square.column;
		state->moveBuf.change.square.row = change->square.row;
		state->moveBuf.change.nowOccupied = change->nowOccupied;

		if ( state->currMode == Play ) {
			bool fullMoveReady = handleMoveBoardChange(
				&(state->moveBuf), &(state->board), &(state->game));
			if ( fullMoveReady ) {
				// update the move timestamps (if available)
				// already here in any case
				ChessMove *latestMove = 
						getLatestFinMove(&(state->game));
				if ( state->timeHandler != NULL ) {
					Player currPlayer = White;
					if ( state->board.active == White ) {
						currPlayer = Black; // already updated for next
					}
					latestMove->playerElapsedClockTime =
						(*(state->timeHandler->getElapsedClockMillis))(currPlayer);
  					latestMove->runningGameTime = 
  						(*(state->timeHandler->getRunningMillis))();
				} else {
					latestMove->playerElapsedClockTime = 0;
					latestMove->runningGameTime = 0;
				}
				if ( ! latestMoveIsPromotion(&(state->game)) ) {
					printLatestMoveUci(state);
				} else {
					setPendingPromotionMark(&(state->moveBuf));
				}
			}
		} else if ( state->currMode == Setup ) {
			handleSetupBoardChange(
				&(state->moveBuf), &(state->board));
			// should here also the fen be printed
			// on all ready full moves?
		}

	}
}

void testFlushPendingPromotionMove(ChessState *state)
{
	if ( state->currMode == Play &&
		isPendingPromotionMarkSet(&(state->moveBuf)) &&
		latestMoveIsPromotion(&(state->game)) ) {

		clearPendingPromotionMark(&(state->moveBuf));

		ChessMove *latestMove = getLatestFinMove(&(state->game));

		Piece promotedToPiece = getPromotedToPiece(
			latestMove);

		swapPiece(promotedToPiece, 
			&(state->board), &(latestMove->endSquare));

		// promotion can be sent only now
		// because the promoted to type can be changed
		// after the move on board is finished
		printLatestMoveUci(state);
	}
}