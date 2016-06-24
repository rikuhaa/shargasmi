#ifndef chessrunner_H
#define chessrunner_H

#include "./chesstypes.h"
#include "./chessclock.h"

#ifdef __cplusplus
extern "C" {
#endif

// TODO should these all take 
// the game etc structs
// or should this class maybe 
// hold them as statics...

typedef enum outputstate {
	NullStateMsg,
	OutStateSetup,
	OutStateMeta,
	OutStateConfig,
	OutStatePlayWhite,
	OutStatePlayBlack,
	OutStatePlayPaused,
	OutStateAfterReset
} ChessOutState;

typedef enum outputmess {
	NullOutputMsg,
	ChessMoveDone,
	ChessClockChanged,
	PgnPrinted,
	FenPrinted,
	PromotionRolledToKnight,
	PromotionRolledToRook,
	PromotionRolledToBishop,
	PromotionRolledToQueen,
	ChessClockBlackPressed,
	ChessClockWhitePressed,
	ChessPausePressed,
	ChessResetPressed,
	CloserToStartPos
} ChessOutMessage;

typedef enum chessmode {
  Setup,
  Play,
  Meta,
  Config
} ChessMode;

typedef enum chessrunnerstate {
	AfterReset,
	Running,
	Paused
} ChessRunnerState;

typedef enum chessaction {
	PrintPgn,
	PrintPgnLong,
	PrintFen,
	// reset clears state
	// and next play begins from clear state
	ResetAction,
	// Play and pause should probably be combined
	// to play/pause (or this can maybe be run 
	// in the side sending the signals)
	PlayAction,
	// pause does not clear state
	// next play begins from non-cleared state
	PauseAction,
	// this could maybe be named more generic
	// and handled like this only under very specific 
	// conditions...
	RollPromotionAction,
	// clock change actions
	WhiteClockPressed,
	BlackClockPressed
} ChessAction;

// TODO
// this has to be done in some more clever way that makes 
// it also easier to clear the error state
// eg. a flag set with possibilty for different errors
// like 'wrongStartPos' that is cleared when trying to 
// start game and set if still wrong (so if it is ok on 
// next start attempt, also the error flag stays cleared)
typedef enum chesserrtype {
	NoError,
	OccupiedDontMatchState,
	UnknownMove
} ChessErrorType;

typedef struct chessstate ChessState;

// TODO ..?
typedef struct chesssched {

	void (*scheduleTask) ( void (*task) (ChessState*),
		long delayMillis);

} ChessScheduler;

typedef struct chessstate {

	char* tempStrBuffer;

	int tempStrBufferLen;

	ChessGame game;
	
	BoardState board;

	MoveBuffer moveBuf;

	ChessMode currMode;

	ChessRunnerState currState;

	bool (*isOccupied)(Row row, Column column);

	void (*outputStateHandler)(ChessOutState, ChessOutMessage);

	void (*outputPrinter) (char*);

	void (*errorHandler) (ChessErrorType, char*);

	ChessClock chessClock;

} ChessState;

extern void initEmptyChessState(
	ChessState *state, char* tempStrBuffer, int tempStrBufferLen,
	ChessMove *moves, int maxMovesPerGame,
	ChessTimeStamp (*getRunningMillis) (void));

extern void setRunnerMode(
	ChessState *state, ChessMode mode);

extern void doAction(
	ChessState *state, ChessAction action);

extern void handleSquareChange(
	ChessState *state, SquareChange *change);


#ifdef __cplusplus
}
#endif

#endif // chessrunner_H
