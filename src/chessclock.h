#ifndef chessclock_H
#define chessclock_H

#include "./chesstypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long long ChessTimeStamp;

typedef struct chessclock {

	ChessTimeStamp (*getRunningMillis) (void);

	ChessTimeStamp whiteElapsed;

	ChessTimeStamp blackElapsed;

	Player activePlayer;

	ChessTimeStamp elapsedBeforePause;

	ChessTimeStamp runningStart;

	bool isPaused;

} ChessClock;

extern void initChessClock(ChessClock*, 
	ChessTimeStamp (*getRunningMillis) (void));

extern void resetChessClock(ChessClock*);

extern void setActivePlayer(ChessClock*, Player);

extern void startChessClock(ChessClock*);

extern void pauseChessClock(ChessClock*);

extern void pressPlayerTurnEnd(ChessClock*, Player);

extern Player getClockActivePlayer(ChessClock*);

extern ChessTimeStamp getPauseStrippedTotal(ChessClock*);

extern ChessTimeStamp getPlayerClockElapsed(ChessClock*, Player);

extern bool isClockPaused(ChessClock*);

#ifdef __cplusplus
}
#endif

#endif // chessclock_H
