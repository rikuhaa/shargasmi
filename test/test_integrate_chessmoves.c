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

void makeSubMove(Column col, Row row, bool nowOccupied) {

	moveBuf.change.square.column = col;
	moveBuf.change.square.row = row;
	moveBuf.change.nowOccupied = nowOccupied;

	handleMoveBoardChange(
		&moveBuf, &board, &game);

}

void makeNormalMove(Column startCol, Row startRow, Column endCol, Row endRow) {

	makeSubMove(startCol, startRow, false);

	makeSubMove(endCol, endRow, true);

}

void makeCapturingMove(Column capturingStartCol, Row capturingStartRow, 
	Column toBeCapturedCol, Row toBeCapturedRow) {

	makeSubMove(capturingStartCol, capturingStartRow, false);

	makeSubMove(toBeCapturedCol, toBeCapturedRow, false);

	// the game state must know which one was the inactive piece
	makeSubMove(toBeCapturedCol, toBeCapturedRow, true);

}

void boardEqualsFen(BoardState* boardState, char* fenStr)
{	
	FEN testFen;

	exportFEN(&testFen, boardState);

	TEST_ASSERT_EQUAL_STRING(fenStr,
		&testFen.piecePlaces);

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

void test_simle_pawn_opening(void) {

	// white king pawn two squares forward
	makeNormalMove(ColE, Row2, ColE, Row4);

	// black king pawn too
	makeNormalMove(ColE, Row7, ColE, Row5);

	BoardPos pos;
	pos.column = ColE;
	pos.row = Row4;

	Piece inE4 = getPiece(&board, &pos);

	TEST_ASSERT_EQUAL_INT(WhitePawn, inE4);

	pos.row = Row5;

	Piece inE5 = getPiece(&board, &pos);

	TEST_ASSERT_EQUAL_INT(BlackPawn, inE5);

	// original squares must be empty
	pos.row = Row2;

	Piece inE2 = getPiece(&board, &pos);

	TEST_ASSERT_EQUAL_INT(Empty, inE2);

	pos.row = Row7;

	Piece inE7 = getPiece(&board, &pos);

	TEST_ASSERT_EQUAL_INT(Empty, inE7);

	// two moves should have been made

	TEST_ASSERT_EQUAL_INT(2, game.finMovesCount);

}

void test_simle_pawn_opening_sacrifice(void) {

	// white king pawn two squares forward
	makeNormalMove(ColE, Row2, ColE, Row4);

	// black queen moves to position where it can be captured
	makeNormalMove(ColD, Row7, ColD, Row5);

	BoardPos pos;
	pos.column = ColE;
	pos.row = Row4;

	Piece inE4 = getPiece(&board, &pos);

	TEST_ASSERT_EQUAL_INT(WhitePawn, inE4);

	pos.row = Row5;
	pos.column = ColD;

	Piece inD5 = getPiece(&board, &pos);

	TEST_ASSERT_EQUAL_INT(BlackPawn, inD5);

	// original squares must be empty
	pos.row = Row2;
	pos.column = ColE;

	Piece inE2 = getPiece(&board, &pos);

	TEST_ASSERT_EQUAL_INT(Empty, inE2);

	pos.row = Row7;
	pos.column = ColD;

	Piece inD7 = getPiece(&board, &pos);

	TEST_ASSERT_EQUAL_INT(Empty, inD7);

	// two moves should have been made

	TEST_ASSERT_EQUAL_INT(2, game.finMovesCount);

	makeCapturingMove(ColE, Row4, ColD, Row5);

	// third move should have been made

	TEST_ASSERT_EQUAL_INT(3, game.finMovesCount);

	pos.row = Row5;
	pos.column = ColD;

	Piece inD5afterCapture = getPiece(&board, &pos);

	TEST_ASSERT_EQUAL_INT(WhitePawn, inD5afterCapture);

	pos.row = Row4;
	pos.column = ColE;

	Piece inE4afterCapture = getPiece(&board, &pos);

	TEST_ASSERT_EQUAL_INT(Empty, inE4afterCapture);

	boardEqualsFen(&board,
		"rnbqkbnr/ppp1pppp/8/3P4/8/8/PPPP1PPP/RNBQKBNR");
}

void test_put_back_to_board_sequence(void)
{
	moveBuf.change.square.column = ColE;
	moveBuf.change.square.row = Row8;

	moveBuf.change.nowOccupied = false;

	handleMoveBoardChange(
		&moveBuf, &board, &game);

	TEST_ASSERT_EQUAL_INT(BlackKing, moveBuf.firstLifted.piece);
	TEST_ASSERT_EQUAL_INT(ColE, moveBuf.firstLifted.startPos.column);
	TEST_ASSERT_EQUAL_INT(Row8, moveBuf.firstLifted.startPos.row);

	TEST_ASSERT_EQUAL_INT(Empty, moveBuf.secondLifted.piece);

	TEST_ASSERT_EQUAL_INT(0, game.finMovesCount);

	// put the king back to the starting square
	moveBuf.change.square.column = ColE;
	moveBuf.change.square.row = Row8;

	moveBuf.change.nowOccupied = true;

	handleMoveBoardChange(
		&moveBuf, &board, &game);

	// test that moves are ignored

	TEST_ASSERT_EQUAL_INT(Empty, moveBuf.secondLifted.piece);
	TEST_ASSERT_EQUAL_INT(Empty, moveBuf.firstLifted.piece);

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

  	// assert buffer empited
  	TEST_ASSERT_EQUAL_INT(Empty, moveBuf.firstLifted.piece);
	TEST_ASSERT_EQUAL_INT(Empty, moveBuf.secondLifted.piece);

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

  	// assert buffer empited
  	TEST_ASSERT_EQUAL_INT(Empty, moveBuf.firstLifted.piece);
	TEST_ASSERT_EQUAL_INT(Empty, moveBuf.secondLifted.piece);

}