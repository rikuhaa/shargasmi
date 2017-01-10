#include "unity.h"
#include "chessutils.h"
#include "chesstypes.h"
#include "string.h"

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

void test_get_fen_name_from_piece(void)
{
	TEST_ASSERT_EQUAL_INT('P', getPieceFENName(WhitePawn));
	TEST_ASSERT_EQUAL_INT('N', getPieceFENName(WhiteKnight));
	TEST_ASSERT_EQUAL_INT('B', getPieceFENName(WhiteBishop));
	TEST_ASSERT_EQUAL_INT('R', getPieceFENName(WhiteRook));
	TEST_ASSERT_EQUAL_INT('Q', getPieceFENName(WhiteQueen));
	TEST_ASSERT_EQUAL_INT('K', getPieceFENName(WhiteKing));
	
	TEST_ASSERT_EQUAL_INT('p', getPieceFENName(BlackPawn));
	TEST_ASSERT_EQUAL_INT('n', getPieceFENName(BlackKnight));
	TEST_ASSERT_EQUAL_INT('b', getPieceFENName(BlackBishop));
	TEST_ASSERT_EQUAL_INT('r', getPieceFENName(BlackRook));
	TEST_ASSERT_EQUAL_INT('q', getPieceFENName(BlackQueen));
	TEST_ASSERT_EQUAL_INT('k', getPieceFENName(BlackKing));
	
	TEST_ASSERT_EQUAL_INT('X', getPieceFENName(Unknown));
}

void test_get_piece_from_fen_name(void)
{
	TEST_ASSERT_EQUAL_INT(WhitePawn, getPieceFromFENName('P'));
	TEST_ASSERT_EQUAL_INT(WhiteKnight, getPieceFromFENName('N'));
	TEST_ASSERT_EQUAL_INT(WhiteBishop, getPieceFromFENName('B'));
	TEST_ASSERT_EQUAL_INT(WhiteRook, getPieceFromFENName('R'));
	TEST_ASSERT_EQUAL_INT(WhiteQueen, getPieceFromFENName('Q'));
	TEST_ASSERT_EQUAL_INT(WhiteKing, getPieceFromFENName('K'));
	
	TEST_ASSERT_EQUAL_INT(BlackPawn, getPieceFromFENName('p'));
	TEST_ASSERT_EQUAL_INT(BlackKnight, getPieceFromFENName('n'));
	TEST_ASSERT_EQUAL_INT(BlackBishop, getPieceFromFENName('b'));
	TEST_ASSERT_EQUAL_INT(BlackRook, getPieceFromFENName('r'));
	TEST_ASSERT_EQUAL_INT(BlackQueen, getPieceFromFENName('q'));
	TEST_ASSERT_EQUAL_INT(BlackKing, getPieceFromFENName('k'));
	
	TEST_ASSERT_EQUAL_INT(Unknown, getPieceFromFENName('Z'));
	TEST_ASSERT_EQUAL_INT(Unknown, getPieceFromFENName('1'));
	TEST_ASSERT_EQUAL_INT(Unknown, getPieceFromFENName('2'));
	TEST_ASSERT_EQUAL_INT(Unknown, getPieceFromFENName(';'));
	TEST_ASSERT_EQUAL_INT(Unknown, getPieceFromFENName(','));
	TEST_ASSERT_EQUAL_INT(Unknown, getPieceFromFENName('/'));

}

void test_get_row_name(void)
{
	TEST_ASSERT_EQUAL_INT('1', getRowName(Row1));
	TEST_ASSERT_EQUAL_INT('2', getRowName(Row2));
	TEST_ASSERT_EQUAL_INT('3', getRowName(Row3));
	TEST_ASSERT_EQUAL_INT('4', getRowName(Row4));

	TEST_ASSERT_EQUAL_INT('5', getRowName(Row5));
	TEST_ASSERT_EQUAL_INT('6', getRowName(Row6));
	TEST_ASSERT_EQUAL_INT('7', getRowName(Row7));
	TEST_ASSERT_EQUAL_INT('8', getRowName(Row8));

	TEST_ASSERT_EQUAL_INT('X', getRowName(-274));
	TEST_ASSERT_EQUAL_INT('X', getRowName(2475));

}

void test_get_column_name(void)
{
	TEST_ASSERT_EQUAL_INT('a', getColumnName(ColA));
	TEST_ASSERT_EQUAL_INT('b', getColumnName(ColB));
	TEST_ASSERT_EQUAL_INT('c', getColumnName(ColC));
	TEST_ASSERT_EQUAL_INT('d', getColumnName(ColD));

	TEST_ASSERT_EQUAL_INT('e', getColumnName(ColE));
	TEST_ASSERT_EQUAL_INT('f', getColumnName(ColF));
	TEST_ASSERT_EQUAL_INT('g', getColumnName(ColG));
	TEST_ASSERT_EQUAL_INT('h', getColumnName(ColH));

	TEST_ASSERT_EQUAL_INT('X', getRowName(-274));
	TEST_ASSERT_EQUAL_INT('X', getRowName(2475));

}

void test_get_row_from_name(void)
{
	TEST_ASSERT_EQUAL_INT(Row1, getRowFromName('1'));
	TEST_ASSERT_EQUAL_INT(Row2, getRowFromName('2'));
	TEST_ASSERT_EQUAL_INT(Row3, getRowFromName('3'));
	TEST_ASSERT_EQUAL_INT(Row4, getRowFromName('4'));

	TEST_ASSERT_EQUAL_INT(Row5, getRowFromName('5'));
	TEST_ASSERT_EQUAL_INT(Row6, getRowFromName('6'));
	TEST_ASSERT_EQUAL_INT(Row7, getRowFromName('7'));
	TEST_ASSERT_EQUAL_INT(Row8, getRowFromName('8'));

}

void test_get_column_from_name(void)
{
	TEST_ASSERT_EQUAL_INT(ColA, getColumnFromName('a'));
	TEST_ASSERT_EQUAL_INT(ColB, getColumnFromName('b'));
	TEST_ASSERT_EQUAL_INT(ColC, getColumnFromName('c'));
	TEST_ASSERT_EQUAL_INT(ColD, getColumnFromName('d'));

	TEST_ASSERT_EQUAL_INT(ColE, getColumnFromName('e'));
	TEST_ASSERT_EQUAL_INT(ColF, getColumnFromName('f'));
	TEST_ASSERT_EQUAL_INT(ColG, getColumnFromName('g'));
	TEST_ASSERT_EQUAL_INT(ColH, getColumnFromName('h'));

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
			// getPieceForStartPos tested in another test
			TEST_ASSERT_EQUAL_INT(getPieceForStartPos(&testPos), 
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
	TEST_ASSERT_EQUAL_INT(WhiteRook, getPieceForStartPos(&forPos));
	forPos.column = ColB;
	TEST_ASSERT_EQUAL_INT(WhiteKnight, getPieceForStartPos(&forPos));
	forPos.column = ColC;
	TEST_ASSERT_EQUAL_INT(WhiteBishop, getPieceForStartPos(&forPos));
	forPos.column = ColD;
	TEST_ASSERT_EQUAL_INT(WhiteQueen, getPieceForStartPos(&forPos));
	forPos.column = ColE;
	TEST_ASSERT_EQUAL_INT(WhiteKing, getPieceForStartPos(&forPos));
	forPos.column = ColF;
	TEST_ASSERT_EQUAL_INT(WhiteBishop, getPieceForStartPos(&forPos));
	forPos.column = ColG;
	TEST_ASSERT_EQUAL_INT(WhiteKnight, getPieceForStartPos(&forPos));
	forPos.column = ColH;
	TEST_ASSERT_EQUAL_INT(WhiteRook, getPieceForStartPos(&forPos));

	forPos.row = Row2;
	
	for ( testCol = 0; testCol < COLUMNS; testCol++ ) {
		forPos.column = testCol;
		TEST_ASSERT_EQUAL_INT(WhitePawn, getPieceForStartPos(&forPos));
	}

	for ( testRow = Row3; testRow <= Row6; testRow++ ) {
		forPos.row = testRow;
		for ( testCol = 0; testCol < COLUMNS; testCol++ ) {
			forPos.column = testCol;
			TEST_ASSERT_EQUAL_INT(Empty, getPieceForStartPos(&forPos));
		}
	}

	forPos.row = Row7;
	
	for ( testCol = 0; testCol < COLUMNS; testCol++ ) {
		forPos.column = testCol;
		TEST_ASSERT_EQUAL_INT(BlackPawn, getPieceForStartPos(&forPos));
	}


	forPos.row = Row8;
	forPos.column = ColA;
	TEST_ASSERT_EQUAL_INT(BlackRook, getPieceForStartPos(&forPos));
	forPos.column = ColB;
	TEST_ASSERT_EQUAL_INT(BlackKnight, getPieceForStartPos(&forPos));
	forPos.column = ColC;
	TEST_ASSERT_EQUAL_INT(BlackBishop, getPieceForStartPos(&forPos));
	forPos.column = ColD;
	TEST_ASSERT_EQUAL_INT(BlackQueen, getPieceForStartPos(&forPos));
	forPos.column = ColE;
	TEST_ASSERT_EQUAL_INT(BlackKing, getPieceForStartPos(&forPos));
	forPos.column = ColF;
	TEST_ASSERT_EQUAL_INT(BlackBishop, getPieceForStartPos(&forPos));
	forPos.column = ColG;
	TEST_ASSERT_EQUAL_INT(BlackKnight, getPieceForStartPos(&forPos));
	forPos.column = ColH;
	TEST_ASSERT_EQUAL_INT(BlackRook, getPieceForStartPos(&forPos));

}

void assertBoardPiecesMatch(BoardState* boardA, BoardState* boardB)
{
	char debugStr[50]; 
	for ( int row = ROWS - 1; row >= 0; row-- ) {
		for ( int column = 0; column < COLUMNS; column++ ) {
			sprintf(debugStr,
				"Board place: row=%i, column=%i", row, column);
			TEST_ASSERT_EQUAL_INT_MESSAGE( 
				boardA->squareStates[row][column],
				boardB->squareStates[row][column],
				debugStr);

		}
	}

}

void test_import_fen_start_pos(void) 
{
	
	FEN toImport;
	BoardState importFromFen;

	strncpy(toImport.piecePlaces, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
		FEN_POS_MAX_CHARS);

	BoardState setupToStartPos;

	importFen(&toImport, &importFromFen);

	setupStartPos(&setupToStartPos);

	assertBoardPiecesMatch(&setupToStartPos, &importFromFen);

}

void test_export_fen_start_pos(void) 
{
	
	FEN exported;

	char *corrFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

	BoardState setupToStartPos;

	setupStartPos(&setupToStartPos);

	exportFen(&exported, &setupToStartPos);

	TEST_ASSERT_EQUAL_STRING(corrFen, exported.piecePlaces);
}

void test_fen_round_trip(void)
{
	FEN fen;
	BoardState boardState;

	char *startFen1 = "6r1/2p3P1/8/8/2p5/2P5/6R1/2k1K3";

	// test pos 1
	strncpy(fen.piecePlaces, startFen1,
		FEN_POS_MAX_CHARS);

	importFen(&fen, &boardState);

	TEST_ASSERT_EQUAL_INT(BlackRook, 
		getPieceRowCol(&boardState, Row8, ColG));
	TEST_ASSERT_EQUAL_INT(WhitePawn, 
		getPieceRowCol(&boardState, Row3, ColC));
	TEST_ASSERT_EQUAL_INT(Empty, 
		getPieceRowCol(&boardState, Row8, ColH));
	TEST_ASSERT_EQUAL_INT(Empty, 
		getPieceRowCol(&boardState, Row1, ColA));

	exportFen(&fen, &boardState);

	TEST_ASSERT_EQUAL_STRING(startFen1,
		&(fen.piecePlaces));

	// test pos 2
	char *startFen2 = "8/8/8/5Bp1/7k/8/4pPKP/8";

	strncpy(fen.piecePlaces, startFen2,
		FEN_POS_MAX_CHARS);

	importFen(&fen, &boardState);

	TEST_ASSERT_EQUAL_INT(Empty, 
		getPieceRowCol(&boardState, Row8, ColG));
	TEST_ASSERT_EQUAL_INT(WhiteBishop, 
		getPieceRowCol(&boardState, Row5, ColF));
	TEST_ASSERT_EQUAL_INT(BlackPawn, 
		getPieceRowCol(&boardState, Row5, ColG));
	TEST_ASSERT_EQUAL_INT(WhitePawn, 
		getPieceRowCol(&boardState, Row2, ColH));
	TEST_ASSERT_EQUAL_INT(Empty,
		getPieceRowCol(&boardState, Row6, ColC));

	exportFen(&fen, &boardState);

	TEST_ASSERT_EQUAL_STRING(startFen2,
		&(fen.piecePlaces));

	// test pos 3
	char *startFen3 = "7b/p3KBkp/7p/2p4p/3p3P/p2P4/P7/8";

	strncpy(fen.piecePlaces, startFen3,
		FEN_POS_MAX_CHARS);

	importFen(&fen, &boardState);

	TEST_ASSERT_EQUAL_INT(BlackBishop, 
		getPieceRowCol(&boardState, Row8, ColH));
	TEST_ASSERT_EQUAL_INT(WhiteKing, 
		getPieceRowCol(&boardState, Row7, ColE));
	TEST_ASSERT_EQUAL_INT(BlackKing, 
		getPieceRowCol(&boardState, Row7, ColG));
	TEST_ASSERT_EQUAL_INT(WhitePawn, 
		getPieceRowCol(&boardState, Row2, ColA));
	TEST_ASSERT_EQUAL_INT(Empty,
		getPieceRowCol(&boardState, Row3, ColC));

	exportFen(&fen, &boardState);

	TEST_ASSERT_EQUAL_STRING(startFen3,
		&(fen.piecePlaces));

	// test pos 4
	char *startFen4 = 
		"7B/1p3p1p/1P4k1/5p2/3p1p1K/3P1PN1/3PP2p/6b1";

	strncpy(fen.piecePlaces, startFen4,
		FEN_POS_MAX_CHARS);

	importFen(&fen, &boardState);

	TEST_ASSERT_EQUAL_INT(WhiteBishop, 
		getPieceRowCol(&boardState, Row8, ColH));
	TEST_ASSERT_EQUAL_INT(BlackPawn, 
		getPieceRowCol(&boardState, Row7, ColB));
	TEST_ASSERT_EQUAL_INT(Empty, 
		getPieceRowCol(&boardState, Row7, ColC));
	TEST_ASSERT_EQUAL_INT(Empty, 
		getPieceRowCol(&boardState, Row7, ColD));
	TEST_ASSERT_EQUAL_INT(Empty,
		getPieceRowCol(&boardState, Row7, ColE));
	TEST_ASSERT_EQUAL_INT(BlackPawn,
		getPieceRowCol(&boardState, Row7, ColF));
	TEST_ASSERT_EQUAL_INT(Empty,
		getPieceRowCol(&boardState, Row7, ColG));
	TEST_ASSERT_EQUAL_INT(BlackPawn,
		getPieceRowCol(&boardState, Row7, ColH));
	TEST_ASSERT_EQUAL_INT(WhiteKnight,
		getPieceRowCol(&boardState, Row3, ColG));

	exportFen(&fen, &boardState);

	TEST_ASSERT_EQUAL_STRING(startFen4,
		&(fen.piecePlaces));

}

void test_export_fen_to_str_start_pos(void) 
{
	
	FEN exported;
	char exportedFenStr[FEN_WHOLE_STR_MAX_CHARS];

	char *corrFen = 
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	BoardState setupToStartPos;

	setupStartPos(&setupToStartPos);

	exportFen(&exported, &setupToStartPos);

	exportFenToString(&exported, exportedFenStr);

	TEST_ASSERT_EQUAL_STRING(corrFen, exportedFenStr);
}

void test_import_start_pos_from_fen_str(void) 
{
	
	FEN imported;

	char *fenStr = 
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	importFenFromString(&imported, fenStr);

	TEST_ASSERT_EQUAL_STRING(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 
		imported.piecePlaces);

	TEST_ASSERT_EQUAL_INT(0, imported.halfMoveClock);
	TEST_ASSERT_EQUAL_INT(1, imported.fullMoveCount);

	TEST_ASSERT_EQUAL_INT(White, imported.activePlayer);

	TEST_ASSERT_EQUAL_INT(ColA, imported.enpassantAvailable.column);
	TEST_ASSERT_EQUAL_INT(Row1, imported.enpassantAvailable.row);
	
	TEST_ASSERT(isCastlingAvailable(imported.canCastleRooks, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(imported.canCastleRooks, WhiteQueenSide));
	TEST_ASSERT(isCastlingAvailable(imported.canCastleRooks, BlackKingSide));
	TEST_ASSERT(isCastlingAvailable(imported.canCastleRooks, BlackQueenSide));

}

// TODO more tests to FEN conversions

void test_write_move_lan_simple_moves(void)
{
	ChessMove moveToTest;
	char resBuffer[10];

	moveToTest.activePiece = WhitePawn;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row2;

	moveToTest.endSquare.column = ColE;
	moveToTest.endSquare.row = Row4;

	moveToTest.type = Move;

	int writtenChars = writeMoveLan(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e2-e4", resBuffer);

	moveToTest.activePiece = BlackBishop;

	moveToTest.startSquare.column = ColG;
	moveToTest.startSquare.row = Row2;

	moveToTest.endSquare.column = ColA;
	moveToTest.endSquare.row = Row8;

	moveToTest.type = Move;

	writtenChars = writeMoveLan(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("Bg2-a8", resBuffer);

}

void test_write_move_lan_simple_captures(void)
{
	ChessMove moveToTest;
	char resBuffer[10];

	moveToTest.activePiece = WhitePawn;
	moveToTest.passivePiece = BlackPawn;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row2;

	moveToTest.endSquare.column = ColE;
	moveToTest.endSquare.row = Row4;

	moveToTest.type = Capture;

	int writtenChars = writeMoveLan(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e2xe4", resBuffer);

	moveToTest.activePiece = BlackBishop;
	moveToTest.passivePiece = WhiteQueen;

	moveToTest.startSquare.column = ColG;
	moveToTest.startSquare.row = Row2;

	moveToTest.endSquare.column = ColA;
	moveToTest.endSquare.row = Row8;

	moveToTest.type = Capture;

	writtenChars = writeMoveLan(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("Bg2xa8", resBuffer);

}

void test_write_move_lan_castling(void)
{

	ChessMove moveToTest;
	char resBuffer[10];

	moveToTest.activePiece = WhiteKing;
	moveToTest.passivePiece = WhiteRook;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row1;

	moveToTest.endSquare.column = ColG;
	moveToTest.endSquare.row = Row1;

	moveToTest.type = Castling;

	int writtenChars = writeMoveLan(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("O-O", resBuffer);

	moveToTest.activePiece = BlackKing;
	moveToTest.passivePiece = BlackRook;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row8;

	moveToTest.endSquare.column = ColC;
	moveToTest.endSquare.row = Row8;

	moveToTest.type = Castling;

	writtenChars = writeMoveLan(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("O-O-O", resBuffer);

}

void test_write_move_lan_promotion(void)
{

	ChessMove moveToTest;
	char resBuffer[10];

	moveToTest.activePiece = WhitePawn;
	moveToTest.passivePiece = BlackRook;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row7;

	moveToTest.endSquare.column = ColF;
	moveToTest.endSquare.row = Row8;

	moveToTest.type = PromotionQueen;

	int writtenChars = writeMoveLan(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e7xf8=Q", resBuffer);

	moveToTest.activePiece = BlackPawn;
	moveToTest.passivePiece = Empty;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row2;

	moveToTest.endSquare.column = ColE;
	moveToTest.endSquare.row = Row1;

	moveToTest.type = PromotionRook;

	writtenChars = writeMoveLan(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e2-e1=R", resBuffer);

	moveToTest.passivePiece = WhiteRook;
	moveToTest.type = PromotionBishop;

	writtenChars = writeMoveLan(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e2xe1=B", resBuffer);

	moveToTest.passivePiece = Empty;
	moveToTest.type = PromotionKnight;

	writtenChars = writeMoveLan(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e2-e1=N", resBuffer);

}

void test_write_move_uci_simple_moves(void)
{
	ChessMove moveToTest;
	char resBuffer[10];

	moveToTest.activePiece = WhitePawn;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row2;

	moveToTest.endSquare.column = ColE;
	moveToTest.endSquare.row = Row4;

	moveToTest.type = Move;

	int writtenChars = writeMoveUci(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e2e4", resBuffer);

	moveToTest.activePiece = BlackBishop;

	moveToTest.startSquare.column = ColG;
	moveToTest.startSquare.row = Row2;

	moveToTest.endSquare.column = ColA;
	moveToTest.endSquare.row = Row8;

	moveToTest.type = Move;

	writtenChars = writeMoveUci(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("g2a8", resBuffer);

}

void test_write_move_uci_simple_captures(void)
{
	ChessMove moveToTest;
	char resBuffer[10];

	moveToTest.activePiece = WhitePawn;
	moveToTest.passivePiece = BlackPawn;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row2;

	moveToTest.endSquare.column = ColE;
	moveToTest.endSquare.row = Row4;

	moveToTest.type = Capture;

	int writtenChars = writeMoveUci(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e2e4", resBuffer);

	moveToTest.activePiece = BlackBishop;
	moveToTest.passivePiece = WhiteQueen;

	moveToTest.startSquare.column = ColG;
	moveToTest.startSquare.row = Row2;

	moveToTest.endSquare.column = ColA;
	moveToTest.endSquare.row = Row8;

	moveToTest.type = Capture;

	writtenChars = writeMoveUci(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("g2a8", resBuffer);

}

void test_write_move_uci_castling(void)
{

	ChessMove moveToTest;
	char resBuffer[10];

	moveToTest.activePiece = WhiteKing;
	moveToTest.passivePiece = WhiteRook;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row1;

	moveToTest.endSquare.column = ColG;
	moveToTest.endSquare.row = Row1;

	moveToTest.type = Castling;

	int writtenChars = writeMoveUci(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e1g1", resBuffer);

	moveToTest.activePiece = BlackKing;
	moveToTest.passivePiece = BlackRook;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row8;

	moveToTest.endSquare.column = ColC;
	moveToTest.endSquare.row = Row8;

	moveToTest.type = Castling;

	writtenChars = writeMoveUci(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e8c8", resBuffer);

}

void test_write_move_uci_promotion(void)
{

	ChessMove moveToTest;
	char resBuffer[10];

	moveToTest.activePiece = WhitePawn;
	moveToTest.passivePiece = BlackRook;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row7;

	moveToTest.endSquare.column = ColF;
	moveToTest.endSquare.row = Row8;

	moveToTest.type = PromotionQueen;

	int writtenChars = writeMoveUci(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e7f8q", resBuffer);

	moveToTest.activePiece = BlackPawn;
	moveToTest.passivePiece = Empty;

	moveToTest.startSquare.column = ColE;
	moveToTest.startSquare.row = Row2;

	moveToTest.endSquare.column = ColE;
	moveToTest.endSquare.row = Row1;

	moveToTest.type = PromotionRook;

	writtenChars = writeMoveUci(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e2e1r", resBuffer);

	moveToTest.passivePiece = WhiteRook;
	moveToTest.type = PromotionBishop;

	writtenChars = writeMoveUci(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e2e1b", resBuffer);

	moveToTest.passivePiece = Empty;
	moveToTest.type = PromotionKnight;

	writtenChars = writeMoveUci(resBuffer, &moveToTest);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("e2e1n", resBuffer);

}

void test_write_pgn_lan_simple_game_start(void)
{

	ChessGame gameToTest;
	ChessMove moves[1000];
	initChessGame(&gameToTest, moves, 1000);

	int halfMoveIndex = 0;

	char resBuffer[10000];

	gameToTest.moves[0].activePiece = WhitePawn;
	gameToTest.moves[0].passivePiece = Empty;
	gameToTest.moves[0].startSquare.column = ColE;
	gameToTest.moves[0].startSquare.row = Row2;
	gameToTest.moves[0].endSquare.column = ColE;
	gameToTest.moves[0].endSquare.row = Row4;
	gameToTest.moves[0].type = Move;

	gameToTest.finMovesCount = 1;

	int writtenChars = writePgnMoves(resBuffer, 10000, NULL, &gameToTest,
		writeMoveLan, NULL);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("1. e2-e4 ", resBuffer);

	gameToTest.moves[1].activePiece = BlackPawn;
	gameToTest.moves[1].passivePiece = Empty;
	gameToTest.moves[1].startSquare.column = ColD;
	gameToTest.moves[1].startSquare.row = Row7;
	gameToTest.moves[1].endSquare.column = ColD;
	gameToTest.moves[1].endSquare.row = Row5;
	gameToTest.moves[1].type = Move;

	gameToTest.finMovesCount = 2;

	writtenChars = writePgnMoves(resBuffer, 10000, NULL, &gameToTest,
		writeMoveLan, NULL);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("1. e2-e4 d7-d5 \n", resBuffer);

	gameToTest.moves[2].activePiece = WhitePawn;
	gameToTest.moves[2].passivePiece = BlackPawn;
	gameToTest.moves[2].startSquare.column = ColE;
	gameToTest.moves[2].startSquare.row = Row4;
	gameToTest.moves[2].endSquare.column = ColD;
	gameToTest.moves[2].endSquare.row = Row5;
	gameToTest.moves[2].type = Capture;

	gameToTest.finMovesCount = 3;

	writtenChars = writePgnMoves(resBuffer, 10000, NULL, &gameToTest,
		writeMoveLan, NULL);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("1. e2-e4 d7-d5 \n2. e4xd5 ", resBuffer);

	gameToTest.moves[3].activePiece = BlackKnight;
	gameToTest.moves[3].passivePiece = Empty;
	gameToTest.moves[3].startSquare.column = ColB;
	gameToTest.moves[3].startSquare.row = Row8;
	gameToTest.moves[3].endSquare.column = ColC;
	gameToTest.moves[3].endSquare.row = Row6;
	gameToTest.moves[3].type = Move;

	gameToTest.finMovesCount = 4;

	writtenChars = writePgnMoves(resBuffer, 10000, NULL, &gameToTest,
		writeMoveLan, NULL);

	resBuffer[writtenChars] = '\0';

	TEST_ASSERT_EQUAL_STRING("1. e2-e4 d7-d5 \n2. e4xd5 Nb8-c6 \n", resBuffer);

}

// more pgn tests in test_integrate_chessmoves

// TODO test the methods for updating fen metadata

// test the method for knowing if latest move was promotion