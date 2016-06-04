#include "unity.h"
#include "chessmoves.h"
#include "chesstypes.h"
#include "chessutils.h"
#include "chessrunner.h"

#include <string.h>
#include <stdio.h>

static ChessState state;

static char outputStrs[1000][10000];
static int currStrInd;

static char tempBuff[10000];

static long currTimeStamp;


void makeSquareChange(Row row, Column col, bool nowOccupied) 
{
	SquareChange change;
	change.square.column = col;
	change.square.row = row;
	change.nowOccupied = nowOccupied;

	handleSquareChange(
		&state, &change);

}

void testOutputReceivedMatches(char** expStrs, int count)
{

	TEST_ASSERT_EQUAL_INT(count, currStrInd);

	for ( int i = 0; i < count; i++ ) {
		TEST_ASSERT_EQUAL_STRING(
			expStrs[i], outputStrs[i]);
	}

}

long timeStamper()
{
	currTimeStamp += 1000;
	return currTimeStamp;
}

bool isOccupied(Row row, Column col) 
{
	return getPieceRowCol(
		&(state.board), row, col) !=
		Empty;
}

void outputPrinter(char *toOutput)
{
	strncpy(outputStrs[currStrInd], toOutput, 10000);
	currStrInd++;
}

void setUp(void)
{
	initEmptyChessState(&state, tempBuff, 10000);
	currStrInd = 0;
	currTimeStamp = 0;
	state.outputPrinter = &outputPrinter;
	state.timeStamper = &timeStamper;
	state.isOccupied = &isOccupied;
}

void tearDown(void)
{
}

void test_simple_game(void)
{
	char *expStrs[] = {"e2e4", "1. e2-e4 ",
		"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"};

	setRunnerMode(&state, Play);

	doAction(&state, PlayAction);

	makeSquareChange(Row2, ColE, false);
	makeSquareChange(Row4, ColE, true);

	doAction(&state, PrintPgn);

	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 3);

}