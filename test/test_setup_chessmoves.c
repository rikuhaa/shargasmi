#include "unity.h"
#include "chessmoves.h"
#include "chesstypes.h"
#include "chessutils.h"

static BoardState board;
static MoveBuffer moveBuf;

void setUp(void)
{
	setupEmptyBoard(&board);

	moveBuf.firstLifted.piece = Empty;
	moveBuf.secondLifted.piece = Empty;

}

void tearDown(void)
{
}

void importFenToTest(char* fenStr)
{
	FEN fen;
	importFenFromString(&fen, fenStr);
	importFen(&fen, &board);

	// should the game also now somehow 
	// that it was not started from normal 
	// start pos? maybe no
	// but maybe in pgn generation this 
	// could actually be taken into account
	// TODO

}

void makeSubChange(Column col, Row row, bool nowOccupied) 
{

	moveBuf.change.square.column = col;
	moveBuf.change.square.row = row;
	moveBuf.change.nowOccupied = nowOccupied;

	handleSetupBoardChange(
		&moveBuf, &board);

}

void boardEqualsFen(BoardState* boardState, char* fenStr)
{	
	FEN testFen;

	exportFen(&testFen, boardState);

	TEST_ASSERT_EQUAL_STRING(fenStr,
		&testFen.piecePlaces);

}

void test_add_pieces_to_starting_positions(void) 
{

	// white king
	makeSubChange(ColE, Row1, true);

	// black king
	makeSubChange(ColE, Row8, true);

	// white pawn
	makeSubChange(ColE, Row2, true);

	// black rook
	makeSubChange(ColA, Row8, true);

	// white bishop
	makeSubChange(ColC, Row1, true);

	// black pawn
	makeSubChange(ColH, Row7, true);

	boardEqualsFen(&board, "r3k3/7p/8/8/8/8/4P3/2B1K3");
}
