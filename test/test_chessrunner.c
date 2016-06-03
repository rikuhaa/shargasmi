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
	currTimeStamp = 0;
}

void setUp(void)
{
	initEmptyChessState(&state, tempBuff, 10000);
	currStrInd = 0;
	state.outputPrinter = &outputPrinter;
	state.timeStamper = &timeStamper;
	state.isOccupied = &isOccupied;
}

void tearDown(void)
{
}

void test_simple_game(void)
{
	setMode(&state, Play);

	

}