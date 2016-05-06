#include "unity.h"
#include "chessmoves.h"
#include "chesstypes.h"
#include "chessutils.h"

static BoardState board;
static MoveBuffer moveBuf;
static ChessGame game;

void setUp(void)
{
	setupStartPos(&board);

	moveBuf.firstLifted.piece = Empty;
	moveBuf.secondLifted.piece = Empty;

	game.finMovesCount = 0;

}

void tearDown(void)
{
}

void test_record_first_moving_piece(void) 
{

	moveBuf.change.square.column = ColE;
	moveBuf.change.square.row = Row7;

	moveBuf.change.nowOccupied = false;

	handleMoveBoardChange(
  		&moveBuf, &board, &game);

	TEST_ASSERT_EQUAL_INT(BlackPawn, moveBuf.firstLifted.piece);
	TEST_ASSERT_EQUAL_INT(ColE, moveBuf.firstLifted.startPos.column);
	TEST_ASSERT_EQUAL_INT(Row7, moveBuf.firstLifted.startPos.row);
	
	TEST_ASSERT_EQUAL_INT(Empty, moveBuf.secondLifted.piece);

	TEST_ASSERT_EQUAL_INT(0, game.finMovesCount);

}

void test_simple_move_sequence(void) 
{

	moveBuf.change.square.column = ColB;
	moveBuf.change.square.row = Row1;

	moveBuf.change.nowOccupied = false;

	handleMoveBoardChange(
  		&moveBuf, &board, &game);

	TEST_ASSERT_EQUAL_INT(WhiteKnight, moveBuf.firstLifted.piece);
	TEST_ASSERT_EQUAL_INT(ColB, moveBuf.firstLifted.startPos.column);
	TEST_ASSERT_EQUAL_INT(Row1, moveBuf.firstLifted.startPos.row);
	
	TEST_ASSERT_EQUAL_INT(Empty, moveBuf.secondLifted.piece);

	TEST_ASSERT_EQUAL_INT(0, game.finMovesCount);

	moveBuf.change.square.column = ColA;
	moveBuf.change.square.row = Row3;

	moveBuf.change.nowOccupied = true;

	handleMoveBoardChange(
  		&moveBuf, &board, &game);

	
	TEST_ASSERT_EQUAL_INT(1, game.finMovesCount);

	ChessMove lastMove = game.moves[game.finMovesCount -1];

	TEST_ASSERT_EQUAL_INT(WhiteKnight, lastMove.activePiece);
  	TEST_ASSERT_EQUAL_INT(Empty, lastMove.passivePiece);

  	TEST_ASSERT_EQUAL_INT(ColB, lastMove.startSquare.column);
  	TEST_ASSERT_EQUAL_INT(Row1, lastMove.startSquare.row);

  	TEST_ASSERT_EQUAL_INT(ColA, lastMove.endSquare.column);
  	TEST_ASSERT_EQUAL_INT(Row3, lastMove.endSquare.row);
  	
  	TEST_ASSERT_EQUAL_INT(Move, lastMove.type);
	
}

void test_simple_capture_sequence(void) 
{

	board.squareStates[Row6][ColD] = WhitePawn;

	moveBuf.change.square.column = ColE;
	moveBuf.change.square.row = Row7;

	moveBuf.change.nowOccupied = false;

	handleMoveBoardChange(
  		&moveBuf, &board, &game);

	TEST_ASSERT_EQUAL_INT(BlackPawn, moveBuf.firstLifted.piece);
	TEST_ASSERT_EQUAL_INT(ColE, moveBuf.firstLifted.startPos.column);
	TEST_ASSERT_EQUAL_INT(Row7, moveBuf.firstLifted.startPos.row);
	
	TEST_ASSERT_EQUAL_INT(Empty, moveBuf.secondLifted.piece);

	moveBuf.change.square.column = ColD;
	moveBuf.change.square.row = Row6;

	moveBuf.change.nowOccupied = false;

	handleMoveBoardChange(
  		&moveBuf, &board, &game);

	TEST_ASSERT_EQUAL_INT(BlackPawn, moveBuf.firstLifted.piece);
	TEST_ASSERT_EQUAL_INT(ColE, moveBuf.firstLifted.startPos.column);
	TEST_ASSERT_EQUAL_INT(Row7, moveBuf.firstLifted.startPos.row);

	TEST_ASSERT_EQUAL_INT(WhitePawn, moveBuf.secondLifted.piece);
	TEST_ASSERT_EQUAL_INT(ColD, moveBuf.secondLifted.startPos.column);
	TEST_ASSERT_EQUAL_INT(Row6, moveBuf.secondLifted.startPos.row);
	
	moveBuf.change.square.column = ColD;
	moveBuf.change.square.row = Row6;

	moveBuf.change.nowOccupied = true;

	handleMoveBoardChange(
  		&moveBuf, &board, &game);

	TEST_ASSERT_EQUAL_INT(1, game.finMovesCount);

	ChessMove lastMove = game.moves[game.finMovesCount -1];

	TEST_ASSERT_EQUAL_INT(BlackPawn, lastMove.activePiece);
  	TEST_ASSERT_EQUAL_INT(WhitePawn, lastMove.passivePiece);

  	TEST_ASSERT_EQUAL_INT(ColE, lastMove.startSquare.column);
  	TEST_ASSERT_EQUAL_INT(Row7, lastMove.startSquare.row);

  	TEST_ASSERT_EQUAL_INT(ColD, lastMove.endSquare.column);
  	TEST_ASSERT_EQUAL_INT(Row6, lastMove.endSquare.row);
  	
  	TEST_ASSERT_EQUAL_INT(Capture, lastMove.type);

}