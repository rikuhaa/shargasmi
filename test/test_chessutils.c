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

void testBoardPiecesMatch(BoardState* boardA, BoardState* boardB)
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

	importFEN(&toImport, &importFromFen);

	setupStartPos(&setupToStartPos);

	testBoardPiecesMatch(&setupToStartPos, &importFromFen);

}

void test_export_fen_start_pos(void) 
{
	
	FEN exported;

	char *corrFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

	BoardState setupToStartPos;

	setupStartPos(&setupToStartPos);

	exportFEN(&exported, &setupToStartPos);

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

	importFEN(&fen, &boardState);

	TEST_ASSERT_EQUAL_INT(BlackRook, 
		getPieceRowCol(&boardState, Row8, ColG));
	TEST_ASSERT_EQUAL_INT(WhitePawn, 
		getPieceRowCol(&boardState, Row3, ColC));
	TEST_ASSERT_EQUAL_INT(Empty, 
		getPieceRowCol(&boardState, Row8, ColH));
	TEST_ASSERT_EQUAL_INT(Empty, 
		getPieceRowCol(&boardState, Row1, ColA));

	exportFEN(&fen, &boardState);

	TEST_ASSERT_EQUAL_STRING(startFen1,
		&(fen.piecePlaces));

	// test pos 2
	char *startFen2 = "8/8/8/5Bp1/7k/8/4pPKP/8";

	strncpy(fen.piecePlaces, startFen2,
		FEN_POS_MAX_CHARS);

	importFEN(&fen, &boardState);

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

	exportFEN(&fen, &boardState);

	TEST_ASSERT_EQUAL_STRING(startFen2,
		&(fen.piecePlaces));

	// test pos 3
	char *startFen3 = "7b/p3KBkp/7p/2p4p/3p3P/p2P4/P7/8";

	strncpy(fen.piecePlaces, startFen3,
		FEN_POS_MAX_CHARS);

	importFEN(&fen, &boardState);

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

	exportFEN(&fen, &boardState);

	TEST_ASSERT_EQUAL_STRING(startFen3,
		&(fen.piecePlaces));

	// test pos 4
	char *startFen4 = 
		"7B/1p3p1p/1P4k1/5p2/3p1p1K/3P1PN1/3PP2p/6b1";

	strncpy(fen.piecePlaces, startFen4,
		FEN_POS_MAX_CHARS);

	importFEN(&fen, &boardState);

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

	exportFEN(&fen, &boardState);

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

	exportFEN(&exported, &setupToStartPos);

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