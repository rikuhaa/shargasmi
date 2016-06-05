#include "chessclock.h"

ChessElapsedSecTenths getFromLastStart(ChessClock* clock)
{
	return ((*(clock->getRunningMillis))() - clock->runningStart) / 100;
}

ChessElapsedSecTenths getRunningIncrement(ChessClock* clock, bool clear)
{
	ChessTimeStamp runningInc = clock->elapsedBeforePause;
	if ( ! clock->isPaused ) {
		runningInc += getFromLastStart(clock);
	}
	if ( clear ) {
		clock->runningStart = (*(clock->getRunningMillis))();
		clock->elapsedBeforePause = 0;
	}
	return runningInc;
}

void initChessClock(ChessClock *clock, ChessTimeStamp (*getRunningMillis) (void) )
{
	clock->getRunningMillis = getRunningMillis;
	resetChessClock(clock);
}

void resetChessClock(ChessClock *clock)
{
	clock->whiteElapsed = 0;
	clock->blackElapsed = 0;
	clock->activePlayer = White;
	clock->elapsedBeforePause = 0;
	clock->isPaused = true;
	clock->runningStart = 0; // not started yet
}

void setActivePlayer(ChessClock *clock, Player player)
{
	clock->activePlayer = player;
}

void startChessClock(ChessClock *clock)
{
	if ( clock->isPaused ) {
		clock->runningStart = (*(clock->getRunningMillis))();
		clock->isPaused = false;
	} // else: no-op if already running
}

void pauseChessClock(ChessClock *clock)
{
	if ( ! clock->isPaused ) {
		clock->elapsedBeforePause += getFromLastStart(clock);
		clock->isPaused = true;
	} // else: no-op if already paused
}

void pressPlayerTurnEnd(ChessClock* clock, Player player)
{
	if ( player == clock->activePlayer ) {
		
		ChessElapsedSecTenths addToClock = getRunningIncrement(clock, true);

		if ( player == White ) {
			clock->whiteElapsed += addToClock;
			clock->activePlayer = Black;
		} else {
			clock->blackElapsed += addToClock;
			clock->activePlayer = White;
		}
	}
	// else: pressing own button twice is no-op or not possible
}

Player getClockActivePlayer(ChessClock* clock)
{
	return clock->activePlayer;
}

ChessElapsedSecTenths getPauseStrippedTotal(ChessClock* clock)
{
	return clock->whiteElapsed + clock->blackElapsed + 
		getRunningIncrement(clock, false);
}

ChessElapsedSecTenths getPlayerClockElapsed(ChessClock* clock, Player player)
{
	if ( player == Black ) {
		ChessElapsedSecTenths runningBlackInc = 0;
		if ( clock->activePlayer == Black ) {
			runningBlackInc = getRunningIncrement(clock, false);
		}
		return clock->blackElapsed + runningBlackInc;
	} else {
		ChessElapsedSecTenths runningWhiteInc = 0;
		if ( clock->activePlayer == White ) {
			runningWhiteInc = getRunningIncrement(clock, false);
		}
		return clock->whiteElapsed + runningWhiteInc;
	}
}

bool isClockPaused(ChessClock* clock)
{
	return clock->isPaused;
}