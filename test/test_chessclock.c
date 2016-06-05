#include "unity.h"
#include "chessclock.h"
#include "chesstypes.h"

static ChessClock clock;

static ChessTimeStamp currTimeStamp;

ChessTimeStamp timeStamper()
{
	return currTimeStamp;
}

void setUp(void)
{
	currTimeStamp = 0;
	initChessClock(&clock, &timeStamper);
}

void tearDown(void)
{
}

void test_chess_clock_simple_sequence(void)
{
	resetChessClock(&clock);

	setActivePlayer(&clock, White);

	startChessClock(&clock);

	currTimeStamp = 1234;

	pressPlayerTurnEnd(&clock, White);

	currTimeStamp = 2234;

	TEST_ASSERT_EQUAL_INT(12, 
		getPlayerClockElapsed(&clock, White));

	TEST_ASSERT_EQUAL_INT(Black,
		getClockActivePlayer(&clock));

	pressPlayerTurnEnd(&clock, Black);

	currTimeStamp = 3571;

	TEST_ASSERT_EQUAL_INT(White,
		getClockActivePlayer(&clock));

	TEST_ASSERT_EQUAL_INT(10,
		getPlayerClockElapsed(&clock, Black));

	TEST_ASSERT_EQUAL_INT(35,
		getPauseStrippedTotal(&clock));

	TEST_ASSERT_EQUAL_INT(25,
		getPlayerClockElapsed(&clock, White));

}