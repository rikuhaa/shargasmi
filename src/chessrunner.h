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
	Reset,
	Start
} ChessAction;

typedef enum chesserrtype {
	UnknownMove
} ChessErrorType;

typedef struct chessstate {

	ChessGame game;
	
	BoardState board;

	MoveBuffer moveBuf;

	ChessMode currMode;

	bool (*isOccupied)(Row row, Column column);

	void (*outputPrinter) (char*);

	void (*errorHandler) (ChessErrorType, char*);

	int (*moveCommentFormatter) (char*, ChessGame*, int);

	long (*timeStamper) (void);

	char* tempStrBuffer;

	int tempStrBufferLen;

} ChessState;

extern void initEmptyChessState(
	ChessState *state, char* tempStrBuffer, int tempStrBufferLen);

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
