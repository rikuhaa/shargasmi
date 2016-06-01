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

void makeSubMove(Column col, Row row, bool nowOccupied) 
{

	moveBuf.change.square.column = col;
	moveBuf.change.square.row = row;
	moveBuf.change.nowOccupied = nowOccupied;

	handleMoveBoardChange(
		&moveBuf, &board, &game);

}

void makeNormalMove(Column startCol, Row startRow, Column endCol, Row endRow) 
{

	makeSubMove(startCol, startRow, false);

	makeSubMove(endCol, endRow, true);

}

void makeCapturingMove(Column capturingStartCol, Row capturingStartRow, 
	Column toBeCapturedCol, Row toBeCapturedRow) 
{

	makeSubMove(capturingStartCol, capturingStartRow, false);

	makeSubMove(toBeCapturedCol, toBeCapturedRow, false);

	// the game state must know which one was the inactive piece
	makeSubMove(toBeCapturedCol, toBeCapturedRow, true);

}

void makeCastlingMove(Row castlingInRow, 
	Column kingStartCol, Column rookStartCol, 
	Column kingEndCol, Column rookEndCol)
{

	makeSubMove(kingStartCol, castlingInRow, false);

	makeSubMove(rookStartCol, castlingInRow, false);

	makeSubMove(kingEndCol, castlingInRow, true);
	// also the rook cames back to the board
	makeSubMove(rookEndCol, castlingInRow, true);

}

void whiteCastleShort() 
{
	// white short
	makeCastlingMove(Row1, ColE, ColH, ColG, ColF);
}

void whiteCastleLong() 
{
	// white long 
	makeCastlingMove(Row1, ColE, ColA, ColC, ColD);
}

void blackCastleShort() 
{
	// black shart
	makeCastlingMove(Row8, ColE, ColH, ColG, ColF);
}

void blackCastleLong()
{
	// black long
	makeCastlingMove(Row8, ColE, ColA, ColC, ColD);
}

void boardEqualsFen(BoardState* boardState, char* fenStr)
{	
	FEN testFen;

	exportFen(&testFen, boardState);

	TEST_ASSERT_EQUAL_STRING(fenStr,
		&testFen.piecePlaces);

}

void gameEqualsPgnWithLan(ChessGame* game, char* pgnStr)
{
	char pgnBuf[10000];

	int resLength = writePgnMoves(pgnBuf, game, writeMoveLan, NULL);

	pgnBuf[resLength] = '\0';

	TEST_ASSERT_EQUAL_STRING(pgnStr, pgnBuf);

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

void test_simle_pawn_castlings(void) {

	char* startFen = 
		"r3k2r/pppbqpbp/2n2np1/3pp3/3PP3/2N2NP1/PPPBQPBP/R3K2R w KQkq - 2 9";

	importFenToTest(startFen);

	// white long 
	makeCastlingMove(Row1, ColE, ColA, ColC, ColD);

	// black short
	makeCastlingMove(Row8, ColE, ColH, ColG, ColF);

	boardEqualsFen(&board,
		"r4rk1/pppbqpbp/2n2np1/3pp3/3PP3/2N2NP1/PPPBQPBP/2KR3R");

	importFenToTest(startFen);

	// white short
	makeCastlingMove(Row1, ColE, ColH, ColG, ColF);

	// black long
	makeCastlingMove(Row8, ColE, ColA, ColC, ColD);

	boardEqualsFen(&board,
		"2kr3r/pppbqpbp/2n2np1/3pp3/3PP3/2N2NP1/PPPBQPBP/R4RK1");

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

void test_sicilian_dragon_yugoslav_16_moves(void)
{

	// 1. e4
	makeNormalMove(ColE, Row2, ColE, Row4);

	// 1. .. c5
	makeNormalMove(ColC, Row7, ColC, Row5);

	// 2. Nf3
	makeNormalMove(ColG, Row1, ColF, Row3);

	// 2. .. d6
	makeNormalMove(ColD, Row7, ColD, Row6);

	// 3. d4 cxd4
	makeNormalMove(ColD, Row2, ColD, Row4);
	makeCapturingMove(ColC, Row5, ColD, Row4);

	// 4. Nxd4 Nf6
	makeCapturingMove(ColF, Row3, ColD, Row4);
	makeNormalMove(ColG, Row8, ColF, Row6);

	// 5. Nc3 g6
	makeNormalMove(ColB, Row1, ColC, Row3);
	makeNormalMove(ColG, Row7, ColG, Row6);

	// main line dragon variation, check pgn and fen

	gameEqualsPgnWithLan(&game,
		"1. e2-e4 c7-c5 \n2. Ng1-f3 d7-d6 \n"
		"3. d2-d4 c5xd4 \n4. Nf3xd4 Ng8-f6 \n"
		"5. Nb1-c3 g7-g6 \n");

	boardEqualsFen(&board,
		"rnbqkb1r/pp2pp1p/3p1np1/8/3NP3/2N5/PPP2PPP/R1BQKB1R");

	// continue with the yugoslav with 9.O-O-O, B76

	// 6. Be3 Bg7
	makeNormalMove(ColC, Row1, ColE, Row3);
	makeNormalMove(ColF, Row8, ColG, Row7);

	// 7. f3 O-O
	makeNormalMove(ColF, Row2, ColF, Row3);
	blackCastleShort();

	// 8. Qd2 Nc6
	makeNormalMove(ColD, Row1, ColD, Row2);
	makeNormalMove(ColB, Row8, ColC, Row6);

	// 9. O-O-O
	whiteCastleLong();

	gameEqualsPgnWithLan(&game,
		"1. e2-e4 c7-c5 \n2. Ng1-f3 d7-d6 \n"
		"3. d2-d4 c5xd4 \n4. Nf3xd4 Ng8-f6 \n"
		"5. Nb1-c3 g7-g6 \n6. Bc1-e3 Bf8-g7 \n"
		"7. f2-f3 O-O \n8. Qd1-d2 Nb8-c6 \n"
		"9. O-O-O ");

	// whole fen: 
	// r1bq1rk1/pp2ppbp/2np1np1/8/3NP3/2N1BP2/PPPQ2PP/2KR1B1R b - - 0 9

	boardEqualsFen(&board,
		"r1bq1rk1/pp2ppbp/2np1np1/8/3NP3/2N1BP2/PPPQ2PP/2KR1B1R");

}