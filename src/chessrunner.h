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
	PGN,
	FEN,
	Reset
} ChessAction;

extern void setMode(
  ChessMode mode);

extern void doAction(
	ChessAction action);

extern void handleSquareChange(
	SquareChange *change);


#ifdef __cplusplus
}
#endif

#endif // chessmoves_H
