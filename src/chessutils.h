#ifndef chessutils_H
#define chessutils_H

#include "chesstypes.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool updateEnpassantTarget(
	Piece piece, BoardPos *startPas, 
	BoardPos *endPos, BoardPos *enpassantSquare);

extern void updateCastlingAfterPieceMoved(
	BoardPos *movedFrom,  BoardPos *movedTo,
  	bool ignoreMovedTo,
  	CastlingAvailability *toUpdate);


extern bool resetsHalfMoveClock(ChessMove *move);

extern bool latestMoveIsPromotion(ChessGame *game);

extern Piece getPromotedToPiece(ChessMove *move);

extern ChessMove *getLatestFinMove(ChessGame *game);

extern int writeMoveLan(char* writeTo, ChessMove* move);

extern int writeMoveUci(char* writeTo, ChessMove* move);

extern int writePgnMoves(char* writeTo, ChessGame* game, 
  int (*singleMoveFormatter) (char*, ChessMove*),
  int (*moveCommentFormatter) (char*, ChessGame*, int));

extern int writeTimeStampInfo(char* writeTo,
	ChessGame *game, int halfMoveIndex);

extern char getPieceFENName(Piece piece);

extern Piece getPieceFromFENName(char name);

/*
* Takes a position on the board (row, colum) and returns
* which piece is on that square when the board is setup 
* correctly.
* Can return Empty
*/
extern Piece getPieceForStartPos(BoardPos *boardPos);

extern void setupEmptyBoard(BoardState* boardState);

extern void setupStartPos(BoardState* boardState);

extern char getPromotionChar(MoveType moveType);

extern char getRowName(Row row);

extern char getColumnName(Column col);

extern Row getRowFromName(char name);

extern Column getColumnFromName(char name);

/*
* If either queen side or king side castling has been done
* and the situation is still on the board for the active
* player, returns which one has been done.
* If there is no castled pattern on board for the 
* active player, returns 0
*/
extern CastlingAvailOption checkBoardActiveCastledness(
	BoardState* boardState);

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

extern void importFen(FEN* fen, BoardState* boardState);

extern void exportFen(FEN* fen, BoardState* boardState);

extern void exportFenToString(FEN* fen, char* fenString);

extern void importFenFromString(FEN* fen, char* fenString);

extern bool belongsToPlayer(Player player, Piece piece);

#ifdef __cplusplus
}
#endif

#endif // chessutils_H