#ifndef chessutils_H
#define chessutils_H

#include "chesstypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* Checks if the piece is in a position on the board that
* means that it will be promoted.
* True if WhitePawn on row 8 or BlackPawn on row 1,
* false otherwise.
*/
extern bool isInPromotePos(Piece piece);

/*
* Returns true if the given piece is on its correct start
* position.
* If called for "meta" Piece (Unknown or Empty) returns
* false.
*/
extern bool isInStartPos(Piece piece);

/*
*
*/
extern char* getSAN(ChessMove* move);

extern int writeMoveLan(char* writeTo, ChessMove* move);

extern int writeMoveUci(char* writeTo, ChessMove* move);

extern int writePgnLanMoves(char* writeTo, ChessGame* game);

extern char getPieceFENName(Piece piece);

extern Piece getPieceFromFENName(char name);

extern char getPromotionChar(MoveType moveType);

extern char getRowName(Row row);

extern char getColumnName(Column col);

extern Row getRowFromName(char name);

extern Column getColumnFromName(char name);

/*
* Returns which piece currently occupies square 'boardPos'
* in board 'boardState'
* Can be Empty
*/
extern Piece getPiece(BoardState* boardState, BoardPos* boardPos);

extern Piece getPieceRowCol(BoardState* boardState, Row row, Column column);

/*
* Puts the 'newPiece' to the specified square on the board,
* and returns the Piece that was on the square earlier.
* Both the returned Piece and the Piece to put on the square
* can also be "meta" pieces, mainly Empty.
*/
extern Piece swapPiece(Piece newPiece, BoardState* boardState, BoardPos* boardPos);

extern void importFEN(FEN* fen, BoardState* boardState);

extern void exportFEN(FEN* fen, BoardState* boardState);

extern void exportFenToString(FEN* fen, char* fenString);

extern void importFenFromString(FEN* fen, char* fenString);

extern bool belongsToPlayer(Player player, Piece piece);

#ifdef __cplusplus
}
#endif

#endif // chessutils_H