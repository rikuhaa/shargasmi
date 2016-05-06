#include "unity.h"
#include "chessutils.h"
#include "chesstypes.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_get_piece(void)
{
	BoardState board;
	BoardPos testPos;
	setupEmptyBoard(&board);

	board.squareStates[Row5][ColD] = WhiteQueen;
	board.squareStates[Row3][ColA] = BlackPawn;

	for ( int row = 0; row < ROWS; row++ ) {
		for ( int column = 0; column < COLUMNS; column++ ) {
			testPos.row = row;
			testPos.column = column;
			if ( row == Row5 && column == ColD ) {
				TEST_ASSERT_EQUAL_INT(WhiteQueen, 
					getPiece(&board, &testPos));
			} else if ( row == Row3 && column == ColA ) {
				TEST_ASSERT_EQUAL_INT(BlackPawn,
					getPiece(&board, &testPos));
			} else {
				TEST_ASSERT_EQUAL_INT(Empty,
					getPiece(&board, &testPos));
			}
		}
	}
}

void test_swap_piece(void)
{
	BoardState board;
	BoardPos testPos;
	setupStartPos(&board);

	testPos.column = ColB;
	testPos.row = Row1;
	Piece swapped1 = swapPiece(Empty, &board, &testPos);

	TEST_ASSERT_EQUAL_INT(Empty, board.squareStates[Row1][ColB]);
	TEST_ASSERT_EQUAL_INT(WhiteKnight, swapped1);

	testPos.column = ColC;
	testPos.row = Row3;
	Piece swapped2 = swapPiece(WhiteKnight, &board, &testPos);

	TEST_ASSERT_EQUAL_INT(WhiteKnight, board.squareStates[Row3][ColC]);
	TEST_ASSERT_EQUAL_INT(Empty, swapped2);

	// TODO more cases...

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
