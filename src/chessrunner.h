#ifndef chessrunner_H
#define chessrunner_H

#include "./chesstypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// TODO should these all take 
// the game etc structs
// or should this class maybe 
// hold them as statics...

typedef enum chessmode {
  Setup,
  Play
} ChessMode;

typedef enum chessaction {
	PrintPgn,
	PrintFen,
	Reset
} ChessAction;

typedef enum chesserrtype {
	UnknownMove
} ChessErrorType;

typedef struct chessstate {

	ChessGame game;
	BoardState board;

	MoveBuffer moveBuf;

	ChessMode currMode;

	void (*commandPrinter) (char*);

	void (*errorHandler) (ChessErrorType, char*);

	int (*moveCommentFormatter) (char*, ChessGame*, int);

	long (*timeStamper) (void);

} ChessState;

extern void setMode(
	ChessState *state, ChessMode mode);

extern void doAction(
	ChessState *state, ChessAction action);

extern void handleSquareChange(
	ChessState *state, SquareChange *change);


#ifdef __cplusplus
}
#endif

#endif // chessmoves_H
