#include "unity.h"
#include "chessmoves.h"
#include "chesstypes.h"
#include "chessutils.h"
#include "chessrunner.h"

static BoardState board;
static MoveBuffer moveBuf;
static ChessGame game;

static ChessState state;

void setUp(void)
{
	setupStartPos(&board);

	moveBuf.firstLifted.piece = Empty;
	moveBuf.secondLifted.piece = Empty;

	game.finMovesCount = 0;

	state.board = board;
	state.moveBuf = moveBuf;

	state.game = game;

}

void tearDown(void)
{
}

