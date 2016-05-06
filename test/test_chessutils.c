#include "unity.h"
#include "chessutils.h"
#include "chesstypes.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_setup_empty_board(void)
{
	BoardState board;
	setupEmptyBoard(&board);
	for ( int row = 0; row < ROWS; row++ ) {
		for ( int column = 0; column < COLUMNS; column++ ) {
			TEST_ASSERT_EQUAL_INT(Empty, board.squareStates[row][column]);
		}
	}
}

void test_setup_start_pos(void)
{
	BoardState board;
	BoardPos testPos;
	setupStartPos(&board);
	TEST_ASSERT_EQUAL_INT(White, board.active);
	for ( int row = 0; row < ROWS; row++ ) {
		for ( int column = 0; column < COLUMNS; column++ ) {
			testPos.row = row;
			testPos.column = column;
			// getForStartPos tested in another test
			TEST_ASSERT_EQUAL_INT(getForStartPos(&testPos), 
				board.squareStates[row][column]);
		}
	}
}

void test_belongs_to_player(void)
{
	TEST_ASSERT(belongsToPlayer(White, WhitePawn));
	TEST_ASSERT(belongsToPlayer(White, WhiteRook));
	TEST_ASSERT(belongsToPlayer(White, WhiteBishop));
	TEST_ASSERT(belongsToPlayer(White, WhiteKnight));
	TEST_ASSERT(belongsToPlayer(White, WhiteQueen));
	TEST_ASSERT(belongsToPlayer(White, WhiteKing));
	
	TEST_ASSERT(!belongsToPlayer(White, BlackPawn));
	TEST_ASSERT(!belongsToPlayer(White, BlackRook));
	TEST_ASSERT(!belongsToPlayer(White, BlackBishop));
	TEST_ASSERT(!belongsToPlayer(White, BlackKnight));
	TEST_ASSERT(!belongsToPlayer(White, BlackQueen));
	TEST_ASSERT(!belongsToPlayer(White, BlackKing));

	TEST_ASSERT(!belongsToPlayer(White, Empty));
	TEST_ASSERT(!belongsToPlayer(White, Unknown));	

	TEST_ASSERT(belongsToPlayer(Black, BlackPawn));
	TEST_ASSERT(belongsToPlayer(Black, BlackRook));
	TEST_ASSERT(belongsToPlayer(Black, BlackBishop));
	TEST_ASSERT(belongsToPlayer(Black, BlackKnight));
	TEST_ASSERT(belongsToPlayer(Black, BlackQueen));
	TEST_ASSERT(belongsToPlayer(Black, BlackKing));

	TEST_ASSERT(!belongsToPlayer(Black, WhitePawn));
	TEST_ASSERT(!belongsToPlayer(Black, WhiteRook));
	TEST_ASSERT(!belongsToPlayer(Black, WhiteBishop));
	TEST_ASSERT(!belongsToPlayer(Black, WhiteKnight));
	TEST_ASSERT(!belongsToPlayer(Black, WhiteQueen));
	TEST_ASSERT(!belongsToPlayer(Black, WhiteKing));

	TEST_ASSERT(!belongsToPlayer(Black, Empty));
	TEST_ASSERT(!belongsToPlayer(Black, Unknown));	

	TEST_ASSERT(!belongsToPlayer(Neutral, BlackPawn));
	TEST_ASSERT(!belongsToPlayer(Neutral, BlackRook));
	TEST_ASSERT(!belongsToPlayer(Neutral, BlackBishop));
	TEST_ASSERT(!belongsToPlayer(Neutral, BlackKnight));
	TEST_ASSERT(!belongsToPlayer(Neutral, BlackQueen));
	TEST_ASSERT(!belongsToPlayer(Neutral, BlackKing));

	TEST_ASSERT(!belongsToPlayer(Neutral, WhitePawn));
	TEST_ASSERT(!belongsToPlayer(Neutral, WhiteRook));
	TEST_ASSERT(!belongsToPlayer(Neutral, WhiteBishop));
	TEST_ASSERT(!belongsToPlayer(Neutral, WhiteKnight));
	TEST_ASSERT(!belongsToPlayer(Neutral, WhiteQueen));
	TEST_ASSERT(!belongsToPlayer(Neutral, WhiteKing));

	TEST_ASSERT(!belongsToPlayer(Neutral, Empty));
	TEST_ASSERT(!belongsToPlayer(Neutral, Unknown));	
	
}

void test_get_piece_for_start_pos(void)
{
	BoardPos forPos;
	int testCol;
	int testRow;

	forPos.row = Row1;
	forPos.column = ColA;
	TEST_ASSERT_EQUAL_INT(WhiteRook, getForStartPos(&forPos));
	forPos.column = ColB;
	TEST_ASSERT_EQUAL_INT(WhiteKnight, getForStartPos(&forPos));
	forPos.column = ColC;
	TEST_ASSERT_EQUAL_INT(WhiteBishop, getForStartPos(&forPos));
	forPos.column = ColD;
	TEST_ASSERT_EQUAL_INT(WhiteQueen, getForStartPos(&forPos));
	forPos.column = ColE;
	TEST_ASSERT_EQUAL_INT(WhiteKing, getForStartPos(&forPos));
	forPos.column = ColF;
	TEST_ASSERT_EQUAL_INT(WhiteBishop, getForStartPos(&forPos));
	forPos.column = ColG;
	TEST_ASSERT_EQUAL_INT(WhiteKnight, getForStartPos(&forPos));
	forPos.column = ColH;
	TEST_ASSERT_EQUAL_INT(WhiteRook, getForStartPos(&forPos));

	forPos.row = Row2;
	
	for ( testCol = 0; testCol < COLUMNS; testCol++ ) {
		forPos.column = testCol;
		TEST_ASSERT_EQUAL_INT(WhitePawn, getForStartPos(&forPos));
	}

	for ( testRow = Row3; testRow <= Row6; testRow++ ) {
		forPos.row = testRow;
		for ( testCol = 0; testCol < COLUMNS; testCol++ ) {
			forPos.column = testCol;
			TEST_ASSERT_EQUAL_INT(Empty, getForStartPos(&forPos));
		}
	}

	forPos.row = Row7;
	
	for ( testCol = 0; testCol < COLUMNS; testCol++ ) {
		forPos.column = testCol;
		TEST_ASSERT_EQUAL_INT(BlackPawn, getForStartPos(&forPos));
	}


	forPos.row = Row8;
	forPos.column = ColA;
	TEST_ASSERT_EQUAL_INT(BlackRook, getForStartPos(&forPos));
	forPos.column = ColB;
	TEST_ASSERT_EQUAL_INT(BlackKnight, getForStartPos(&forPos));
	forPos.column = ColC;
	TEST_ASSERT_EQUAL_INT(BlackBishop, getForStartPos(&forPos));
	forPos.column = ColD;
	TEST_ASSERT_EQUAL_INT(BlackQueen, getForStartPos(&forPos));
	forPos.column = ColE;
	TEST_ASSERT_EQUAL_INT(BlackKing, getForStartPos(&forPos));
	forPos.column = ColF;
	TEST_ASSERT_EQUAL_INT(BlackBishop, getForStartPos(&forPos));
	forPos.column = ColG;
	TEST_ASSERT_EQUAL_INT(BlackKnight, getForStartPos(&forPos));
	forPos.column = ColH;
	TEST_ASSERT_EQUAL_INT(BlackRook, getForStartPos(&forPos));

}
