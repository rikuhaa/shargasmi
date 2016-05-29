#include "unity.h"
#include "chesstypes.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_is_castling_available(void)
{

	CastlingAvailability castl = 0 | WhiteKingSide | BlackQueenSide;

	TEST_ASSERT(isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));
	
	castl = 0 | WhiteQueenSide;

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, WhiteQueenSide));

	castl = 0;

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));

	castl = 0 | WhiteQueenSide | WhiteKingSide |
		BlackKingSide | BlackQueenSide;

	TEST_ASSERT(isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, WhiteQueenSide));


	castl = 0 | WhiteQueenSide |
		BlackKingSide | BlackQueenSide;

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, WhiteQueenSide));

}

void test_set_castling_availability(void)
{

	CastlingAvailability castl = 0;

	setCastlingAvailability(&castl, WhiteKingSide, true);
	setCastlingAvailability(&castl, BlackQueenSide, true);

	TEST_ASSERT(isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));

	setCastlingAvailability(&castl, WhiteKingSide, false);
	setCastlingAvailability(&castl, BlackQueenSide, true);
	setCastlingAvailability(&castl, WhiteQueenSide, false);
	setCastlingAvailability(&castl, BlackKingSide, false);

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));

	setCastlingAvailability(&castl, WhiteKingSide, false);
	setCastlingAvailability(&castl, BlackQueenSide, false);
	setCastlingAvailability(&castl, WhiteQueenSide, false);
	setCastlingAvailability(&castl, BlackKingSide, false);

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));

	setCastlingAvailability(&castl, WhiteKingSide, true);
	setCastlingAvailability(&castl, BlackQueenSide, true);
	setCastlingAvailability(&castl, WhiteQueenSide, true);
	setCastlingAvailability(&castl, BlackKingSide, true);

	TEST_ASSERT(isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, WhiteQueenSide));

	setCastlingAvailability(&castl, WhiteKingSide, false);
	setCastlingAvailability(&castl, BlackQueenSide, false);
	setCastlingAvailability(&castl, WhiteQueenSide, false);
	setCastlingAvailability(&castl, BlackKingSide, true);

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));

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