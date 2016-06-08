#include "unity.h"
#include "chessmoves.h"
#include "chesstypes.h"
#include "chessutils.h"
#include "chessrunner.h"
#include "chessclock.h"

#include <string.h>
#include <stdio.h>

static ChessMove moves[1000];

static ChessState state;

static char outputStrs[1000][10000];
static int currStrInd;

static char tempBuff[10000];

static ChessTimeStamp currTimeStamp;

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

unsigned long long timeStamper()
{
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
	initEmptyChessState(
		&state, tempBuff, 10000, 
		moves, 1000, &timeStamper);
	currStrInd = 0;
	currTimeStamp = 0;
	state.outputPrinter = &outputPrinter;
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


void test_simple_game_with_time(void)
{
	char *expStrs[] = {
		"e2e4", 
		"1. e2-e4 {[game: 10] [move: 10] [own: 10]} ",
		"e7e5",
		"1. e2-e4 {[game: 10] [move: 10] [own: 10]} "
		"e7-e5 {[game: 20] [move: 10] [own: 10]} \n",
		"b1c3",
		"1. e2-e4 {[game: 10] [move: 10] [own: 10]} "
		"e7-e5 {[game: 20] [move: 10] [own: 10]} \n"
		"2. Nb1-c3 {[game: 30] [move: 10] [own: 20]} "
	};

	setRunnerMode(&state, Play);

	doAction(&state, PlayAction);

	currTimeStamp = 1000;

	makeSquareChange(Row2, ColE, false);
	makeSquareChange(Row4, ColE, true);

	doAction(&state, WhiteClockPressed);

	doAction(&state, PrintPgnLong);

	testOutputReceivedMatches(
		expStrs, 2);

	currTimeStamp = 2000;

	makeSquareChange(Row7, ColE, false);
	makeSquareChange(Row5, ColE, true);

	doAction(&state, BlackClockPressed);

	doAction(&state, PrintPgnLong);

	testOutputReceivedMatches(
		expStrs, 4);

	currTimeStamp = 3000;

	makeSquareChange(Row1, ColB, false);
	makeSquareChange(Row3, ColC, true);

	doAction(&state, WhiteClockPressed);

	doAction(&state, PrintPgnLong);

	testOutputReceivedMatches(
		expStrs, 6);

	TEST_ASSERT_EQUAL_INT(Black, 
		getClockActivePlayer(&(state.chessClock)));

}


void test_morphy_duke_carl_game(void)
{
	char *expStrs[] = {
		// 1
		"e2e4",
		"e7e5",
		"g1f3",
		"d7d6",
		// 3
		"d2d4",
		"c8g4",
		"d4e5",
		"g4f3",
		// 5
		"d1f3",
		"d6e5",
		// first pgn/fen checkpoint
		"1. e2-e4 e7-e5 \n2. Ng1-f3 d7-d6 \n3. d2-d4 Bc8-g4 \n"
		"4. d4xe5 Bg4xf3 \n5. Qd1xf3 d6xe5 \n",
		"rn1qkbnr/ppp2ppp/8/4p3/4P3/5Q2/PPP2PPP/RNB1KB1R w KQkq - 0 6",
		// 6
		"f1c4",
		"g8f6",
		"f3b3",
		"d8e7",
		// 8
		"b1c3",
		"c7c6",
		"c1g5",
		"b7b5",
		// 10 w
		"c3b5",
		// second pgn/fen checkpoint
		"1. e2-e4 e7-e5 \n2. Ng1-f3 d7-d6 \n3. d2-d4 Bc8-g4 \n"
		"4. d4xe5 Bg4xf3 \n5. Qd1xf3 d6xe5 \n6. Bf1-c4 Ng8-f6 \n"
		"7. Qf3-b3 Qd8-e7 \n8. Nb1-c3 c7-c6 \n9. Bc1-g5 b7-b5 \n"
		"10. Nc3xb5 ",
		"rn2kb1r/p3qppp/2p2n2/1N2p1B1/2B1P3/1Q6/PPP2PPP/R3K2R b KQkq - 0 10",
		// 10 b
		"c6b5",
		"c4b5",
		"b8d7",
		// 12
		"e1c1",
		"a8d8",
		"d1d7",
		"d8d7",
		// 14
		"h1d1",
		"e7e6",
		// second pgn/fen checkpoint
		"1. e2-e4 e7-e5 \n2. Ng1-f3 d7-d6 \n3. d2-d4 Bc8-g4 \n"
		"4. d4xe5 Bg4xf3 \n5. Qd1xf3 d6xe5 \n6. Bf1-c4 Ng8-f6 \n"
		"7. Qf3-b3 Qd8-e7 \n8. Nb1-c3 c7-c6 \n9. Bc1-g5 b7-b5 \n"
		"10. Nc3xb5 c6xb5 \n11. Bc4xb5 Nb8-d7 \n12. O-O-O Ra8-d8 \n"
		"13. Rd1xd7 Rd8xd7 \n14. Rh1-d1 Qe7-e6 \n",
		"4kb1r/p2r1ppp/4qn2/1B2p1B1/4P3/1Q6/PPP2PPP/2KR4 w k - 2 15",
		// 15
		"b5d7",
		"f6d7",
		"b3b8",
		"d7b8",
		"d1d8",
		// game end pgn/fen checkpoint
		"1. e2-e4 e7-e5 \n2. Ng1-f3 d7-d6 \n3. d2-d4 Bc8-g4 \n"
		"4. d4xe5 Bg4xf3 \n5. Qd1xf3 d6xe5 \n6. Bf1-c4 Ng8-f6 \n"
		"7. Qf3-b3 Qd8-e7 \n8. Nb1-c3 c7-c6 \n9. Bc1-g5 b7-b5 \n"
		"10. Nc3xb5 c6xb5 \n11. Bc4xb5 Nb8-d7 \n12. O-O-O Ra8-d8 \n"
		"13. Rd1xd7 Rd8xd7 \n14. Rh1-d1 Qe7-e6 \n15. Bb5xd7 Nf6xd7 \n"
		"16. Qb3-b8 Nd7xb8 \n17. Rd1-d8 ",
		"1n1Rkb1r/p4ppp/4q3/4p1B1/4P3/8/PPP2PPP/2K5 b k - 1 17"
	};

	setRunnerMode(&state, Play);

	doAction(&state, PlayAction);

	// w1
	makeSquareChange(Row2, ColE, false);
	makeSquareChange(Row4, ColE, true);

	// b1
	makeSquareChange(Row7, ColE, false);
	makeSquareChange(Row5, ColE, true);

	// w2
	makeSquareChange(Row1, ColG, false);
	makeSquareChange(Row3, ColF, true);

	// b2
	makeSquareChange(Row7, ColD, false);
	makeSquareChange(Row6, ColD, true);

	// w3
	makeSquareChange(Row2, ColD, false);
	makeSquareChange(Row4, ColD, true);

	// b3
	makeSquareChange(Row8, ColC, false);
	makeSquareChange(Row4, ColG, true);

	// w4
	makeSquareChange(Row4, ColD, false);
	makeSquareChange(Row5, ColE, false);
	makeSquareChange(Row5, ColE, true);

	// b4
	makeSquareChange(Row4, ColG, false);
	makeSquareChange(Row3, ColF, false);
	makeSquareChange(Row3, ColF, true);

	// w5
	makeSquareChange(Row1, ColD, false);
	makeSquareChange(Row3, ColF, false);
	makeSquareChange(Row3, ColF, true);

	// b5
	makeSquareChange(Row6, ColD, false);
	makeSquareChange(Row5, ColE, false);
	makeSquareChange(Row5, ColE, true);

	// first checkpoint, after five full rounds
	doAction(&state, PrintPgn);
	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 12);

	// w6
	makeSquareChange(Row1, ColF, false);
	makeSquareChange(Row4, ColC, true);

	// b6
	makeSquareChange(Row8, ColG, false);
	makeSquareChange(Row6, ColF, true);

	// w7
	makeSquareChange(Row3, ColF, false);
	makeSquareChange(Row3, ColB, true);

	// b7
	makeSquareChange(Row8, ColD, false);
	makeSquareChange(Row7, ColE, true);

	// w8
	makeSquareChange(Row1, ColB, false);
	makeSquareChange(Row3, ColC, true);

	// b8
	makeSquareChange(Row7, ColC, false);
	makeSquareChange(Row6, ColC, true);

	// w9
	makeSquareChange(Row1, ColC, false);
	makeSquareChange(Row5, ColG, true);

	// b9
	makeSquareChange(Row7, ColB, false);
	makeSquareChange(Row5, ColB, true);

	// w10
	// take to-be-captured piece out of board first,
	// should be okay in this way too
	makeSquareChange(Row5, ColB, false);
	makeSquareChange(Row3, ColC, false);
	makeSquareChange(Row5, ColB, true);

	// second checkpoint, after white's tenth
	doAction(&state, PrintPgn);
	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 12 + 11);

	// b10
	makeSquareChange(Row6, ColC, false);
	makeSquareChange(Row5, ColB, false);
	makeSquareChange(Row5, ColB, true);

	// w11
	makeSquareChange(Row4, ColC, false);
	makeSquareChange(Row5, ColB, false);
	makeSquareChange(Row5, ColB, true);

	// b11
	makeSquareChange(Row8, ColB, false);
	makeSquareChange(Row7, ColD, true);

	// w12
	makeSquareChange(Row1, ColE, false);
	makeSquareChange(Row1, ColA, false);
	makeSquareChange(Row1, ColD, true);
	makeSquareChange(Row1, ColC, true);

	// b12
	makeSquareChange(Row8, ColA, false);
	makeSquareChange(Row8, ColD, true);

	// w13
	makeSquareChange(Row1, ColD, false);
	makeSquareChange(Row7, ColD, false);
	makeSquareChange(Row7, ColD, true);

	// b13
	// take to-be-captured piece out of board first,
	// should be okay in this way too
	makeSquareChange(Row7, ColD, false);
	makeSquareChange(Row8, ColD, false);
	makeSquareChange(Row7, ColD, true);

	// w14
	makeSquareChange(Row1, ColH, false);
	makeSquareChange(Row1, ColD, true);

	// b14
	makeSquareChange(Row7, ColE, false);
	makeSquareChange(Row6, ColE, true);

	// pgn / fen checkpoint after 14 (non-zero half move clock)

	doAction(&state, PrintPgn);
	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 12 + 11 + 11);

	// w15
	// take to-be-captured piece out of board first,
	// should be okay in this way too
	makeSquareChange(Row7, ColD, false);
	makeSquareChange(Row5, ColB, false);
	makeSquareChange(Row7, ColD, true);

	// b15
	makeSquareChange(Row6, ColF, false);
	makeSquareChange(Row7, ColD, false);
	makeSquareChange(Row7, ColD, true);

	// w16
	makeSquareChange(Row3, ColB, false);
	makeSquareChange(Row8, ColB, true);

	// b16
	// take to-be-captured piece out of board first,
	// should be okay in this way too
	makeSquareChange(Row8, ColB, false);
	makeSquareChange(Row7, ColD, false);
	makeSquareChange(Row8, ColB, true);

	// w17
	makeSquareChange(Row1, ColD, false);
	makeSquareChange(Row8, ColD, true);

	// this is mate, game ends here

	// pgn / fen checkpoint for the match end position

	doAction(&state, PrintPgn);
	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 12 + 11 + 11 + 7);


}

void test_setup_mode(void)
{
	char *expStrs[] = {
		"4k3/8/8/8/8/8/4P3/4K3 w - - 0 1",
		"4k3/6P1/8/8/4r3/5K2/8/8 w - - 0 1",
		"4k3/P5P1/8/8/4r3/5K2/1pp4p/8 w - - 0 1"
	};

	setRunnerMode(&state, Setup);

	doAction(&state, PlayAction);

	// should be empty board ready to be set-up with pieces

	// bring in the kings
	makeSquareChange(Row8, ColE, true);
	makeSquareChange(Row1, ColE, true);

	// one pawn to white
	makeSquareChange(Row2, ColE, true);

	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 1);

	// bring in some more black pieces and then make white 
	// pawn ready to be promoted

	// black rook
	makeSquareChange(Row8, ColH, true);
	// move rook to other square
	makeSquareChange(Row8, ColH, false);
	makeSquareChange(Row4, ColE, true);

	// move white king
	makeSquareChange(Row1, ColE, false);
	makeSquareChange(Row3, ColF, true);

	// move white pawn to potential promote square
	makeSquareChange(Row2, ColE, false);
	makeSquareChange(Row7, ColG, true);
	
	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 2);

	// bring in more pawns and get them to promote positions
	makeSquareChange(Row7, ColB, true);
	makeSquareChange(Row7, ColB, false);
	makeSquareChange(Row2, ColB, true);

	makeSquareChange(Row7, ColB, true);
	makeSquareChange(Row7, ColB, false);
	makeSquareChange(Row2, ColH, true);

	makeSquareChange(Row7, ColA, true);
	makeSquareChange(Row7, ColA, false);
	makeSquareChange(Row2, ColC, true);

	makeSquareChange(Row2, ColD, true);
	makeSquareChange(Row2, ColD, false);
	makeSquareChange(Row7, ColA, true);

	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 3);

}

void test_continue_from_setup_and_promotions(void)
{

	char *expStrs[] = {
		"4k3/P5P1/8/8/4r3/5K2/1pp4p/8 w - - 0 1",
		"g7g8q",
		"e8d7",
		"1. g7-g8=Q Ke8-d7 \n",
		"6Q1/P2k4/8/8/4r3/5K2/1pp4p/8 w - - 1 2",
		"a7a8n",
		"b2b1b",
		"g8g1",
		"1. g7-g8=Q Ke8-d7 \n2. a7-a8=N b2-b1=B \n"
		"3. Qg8-g1 ",
		"N7/3k4/8/8/4r3/5K2/2p4p/1b4Q1 b - - 1 3",
		"h2g1r",
		"1. g7-g8=Q Ke8-d7 \n2. a7-a8=N b2-b1=B \n"
		"3. Qg8-g1 h2xg1=R \n",
		"N7/3k4/8/8/4r3/5K2/2p5/1b4r1 w - - 0 4",
	};

	setRunnerMode(&state, Setup);

	doAction(&state, PlayAction);

	// should be empty board ready to be set-up with pieces

	// bring in the kings
	makeSquareChange(Row8, ColE, true);
	makeSquareChange(Row1, ColE, true);

	// one pawn to white
	makeSquareChange(Row2, ColE, true);

	// bring in some more black pieces and then make white 
	// pawn ready to be promoted

	// black rook
	makeSquareChange(Row8, ColH, true);
	// move rook to other square
	makeSquareChange(Row8, ColH, false);
	makeSquareChange(Row4, ColE, true);

	// move white king
	makeSquareChange(Row1, ColE, false);
	makeSquareChange(Row3, ColF, true);

	// move white pawn to potential promote square
	makeSquareChange(Row2, ColE, false);
	makeSquareChange(Row7, ColG, true);

	// bring in more pawns and get them to promote positions
	makeSquareChange(Row7, ColB, true);
	makeSquareChange(Row7, ColB, false);
	makeSquareChange(Row2, ColB, true);

	makeSquareChange(Row7, ColB, true);
	makeSquareChange(Row7, ColB, false);
	makeSquareChange(Row2, ColH, true);

	makeSquareChange(Row7, ColA, true);
	makeSquareChange(Row7, ColA, false);
	makeSquareChange(Row2, ColC, true);

	makeSquareChange(Row2, ColD, true);
	makeSquareChange(Row2, ColD, false);
	makeSquareChange(Row7, ColA, true);

	// test that the board is setup as expected
	// with many chances for promotion

	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 1);

	// change from setup mode to play mode

	setRunnerMode(&state, Play);

	doAction(&state, PlayAction);

	// start doing moves, white should be the first active player

	makeSquareChange(Row7, ColG, false);
	makeSquareChange(Row8, ColG, true);

	makeSquareChange(Row8, ColE, false);
	makeSquareChange(Row7, ColD, true);

	doAction(&state, PrintPgn);
	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 5);

	// test promotions to other types than to queen

	makeSquareChange(Row7, ColA, false);
	makeSquareChange(Row8, ColA, true);

	// roll from queen to knight
	doAction(&state, RollPromotionAction);

	makeSquareChange(Row2, ColB, false);
	makeSquareChange(Row1, ColB, true);

	// roll from queen to bishop
	doAction(&state, RollPromotionAction);
	doAction(&state, RollPromotionAction);
	doAction(&state, RollPromotionAction);

	makeSquareChange(Row8, ColG, false);
	makeSquareChange(Row1, ColG, true);

	doAction(&state, PrintPgn);
	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 10);

	makeSquareChange(Row2, ColH, false);
	makeSquareChange(Row1, ColG, false);
	makeSquareChange(Row1, ColG, true);

	doAction(&state, RollPromotionAction);
	doAction(&state, RollPromotionAction);

	doAction(&state, PrintPgn);
	doAction(&state, PrintFen);

	testOutputReceivedMatches(
		expStrs, 13);

}

void test_reset_with_setup_and_play(void)
{

}