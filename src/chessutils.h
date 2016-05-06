#ifndef chessutils_H
#define chessutils_H

#include "./chesstypes.h"

/*
* Checks if the piece is in a position on the board that
* means that it will be promoted.
* True if WhitePawn on row 8 or BlackPawn on row 1,
* false otherwise.
*/
extern bool isInPromotePos(Piece* piece);

/*
* Returns true if the given piece is on its correct start
* position.
* If called for "meta" Piece (Unknown or Empty) returns
* false.
*/
extern bool isInStartPos(Piece* piece);

/*
* Takes a position on the board (row, colum) and returns
* which piece is on that square when the board is setup 
* correctly.
* Can return Empty
*/
extern Piece getForStartPos(BoardPos* pos);

/*
*
*/
extern char* getSAN(ChessMove* move);

/*
* Returns which piece currently occupies square 'boardPos'
* in board 'boardState'
* Can be Empty
*/
extern Piece getPiece(BoardState* boardState, BoardPos* boardPos);

/*
* Puts the 'newPiece' to the specified square on the board,
* and returns the Piece that was on the square earlier.
* Both the returned Piece and the Piece to put on the square
* can also be "meta" pieces, mainly Empty.
*/
extern Piece swapPiece(Piece newPiece, BoardState* boardState, BoardPos* boardPos);

extern bool belongsToPlayer(Player player, Piece piece);

extern void setupEmptyBoard(BoardState* boardState);

extern void setupStartPos(BoardState* boardState);

#endif // chessutils_H