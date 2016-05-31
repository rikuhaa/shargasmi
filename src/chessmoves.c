#include "./chesstypes.h"
#include "./chessutils.h"
#include "./chessmoves.h"

void handleMoveBoardChange(
  MoveBuffer *moveBuf, BoardState *boardState, ChessGame *currGame) 
{

  // first piece change for the move sequence
  if ( moveBuf->firstLifted.piece == Empty ) {
    if ( moveBuf->change.nowOccupied ) {
      // this is an error situation
      // first sub-move cannot add a new piece to the board

    }    

    Piece lifted = swapPiece(Empty, boardState, &(moveBuf->change.square));
    
    if ( lifted == Empty ) {
      // this is also an error situation, mainly telling
      // that the boardState was wrong...
    }

    moveBuf->firstLifted.piece = lifted;
    moveBuf->firstLifted.startPos.row = 
      moveBuf->change.square.row;
    moveBuf->firstLifted.startPos.column = 
      moveBuf->change.square.column;

    return;
  }

  // TODO en-passant, and castling moves can be done by first
  // moving capturing pawn or king doing castling
  // also should check for promotion after initial move

  // en passant and castling should be handled in a special
  // way by returning the pieces to board, but keeping 
  // the move open waiting for 'second' lifted piece
  // and for en passant keeping eyes open for the fact
  // that lifting the captured piece ends the turn

  // one piece moving, now returned to the board
  if ( moveBuf->change.nowOccupied && 
      moveBuf->secondLifted.piece == Empty ) {

    int currMoveInd = currGame->finMovesCount;

    // lowering the piece back to same square could be done
    // eg. to correct the piece positioning

    if ( moveBuf->change.square.row == moveBuf->firstLifted.startPos.row &&
      moveBuf->change.square.column == moveBuf->firstLifted.startPos.column ) {
      // empty buffer to start, but don't 
      // record as a move

      clearMoveBuffer(moveBuf);

      return;
    }

    // if max-move, error...

    ChessMove *doneMove = &currGame->moves[currMoveInd];

    doneMove->activePiece = moveBuf->firstLifted.piece;
    doneMove->passivePiece = Empty;

    doneMove->startSquare.column = moveBuf->firstLifted.startPos.column;
    doneMove->startSquare.row = moveBuf->firstLifted.startPos.row;

    doneMove->endSquare.column = moveBuf->change.square.column;
    doneMove->endSquare.row = moveBuf->change.square.row;

    // check if the move caused promotion

    if ( ( doneMove->activePiece == WhitePawn && 
      doneMove->endSquare.row == whitePiecePromotionRow ) ||
      ( doneMove->activePiece == BlackPawn &&
        doneMove->endSquare.row == blackPiecePromotionRow ) ) {
      doneMove->type = PromotionQueen;
      // other promotion types have to be switched later with some external 
      // logic...
    } else {
      doneMove->type = Move;
    }

    // 'commit' move
    currGame->finMovesCount++;
    
    Piece oldInSquare = swapPiece(
      doneMove->activePiece, boardState, &(moveBuf->change.square));
  
    if ( oldInSquare != Empty ) {
      // error, otherwise sub-move finished
    }

    if ( ! belongsToPlayer(boardState->active, doneMove->activePiece) ) {
      // error, normal move can only be done with own pieces
    }

    clearMoveBuffer(moveBuf);
    
    return;
  }

  if ( !moveBuf->change.nowOccupied &&
      moveBuf->secondLifted.piece == Empty ) {
  
    Piece lifted = swapPiece(Empty, boardState, &(moveBuf->change.square));
    
    if ( lifted == Empty ) {
      // this is also an error situation, mainly telling
      // that the boardState was wrong...
    }

    moveBuf->secondLifted.piece = lifted;
    moveBuf->secondLifted.startPos.row = 
      moveBuf->change.square.row;
    moveBuf->secondLifted.startPos.column = 
      moveBuf->change.square.column;

    return;

  } else {
    // error, only two pieces can be moving at the same time 
    // for normal moves
  }

  // two pieces on the move, now returning to board
  if ( moveBuf->change.nowOccupied && 
      moveBuf->secondLifted.piece != Empty ) {

    // handle en passant and castling by just checking 
    // the 'lifted pieces' and current board state, to see
    // which pieces already returned to the board

    // also here remember to check for promotion after capture    
    
    MoveType type = Capture;
    LiftedPiece *activeLifted;
    LiftedPiece *passiveLifted;
    if ( belongsToPlayer(boardState->active, moveBuf->firstLifted.piece) ) {
      activeLifted = &moveBuf->firstLifted;
      passiveLifted = &moveBuf->secondLifted;
      // here, if also second active belongs to the player,
      // only legal option is castling
      if ( belongsToPlayer(
          boardState->active, moveBuf->secondLifted.piece) ) {
        type = Castling;
      }
    } else if ( belongsToPlayer(
        boardState->active, moveBuf->secondLifted.piece) ) {
      activeLifted = &moveBuf->firstLifted;
      passiveLifted = &moveBuf->secondLifted;
    } else {
      // error neither piece was from the active player
    }

    // record capture move (TODO castling)    

    int currMoveInd = currGame->finMovesCount;

    // if max-move, error...

    ChessMove *doneMove = &currGame->moves[currMoveInd];

    if ( type != Castling ) {  

      doneMove->activePiece = activeLifted->piece;
      doneMove->passivePiece = passiveLifted->piece;

      doneMove->startSquare.column = activeLifted->startPos.column;
      doneMove->startSquare.row = activeLifted->startPos.row;

      doneMove->endSquare.column = moveBuf->change.square.column;
      doneMove->endSquare.row = moveBuf->change.square.row;

      // NOTE: en-passant has nothing special from this point of view
      // because we are not trying to test validity here;
      // the returning piece only returns to empty square ('en passant' square)
      // but it still is a capture and the endsquare is marked right

      // check if the move caused promotion

      if ( ( doneMove->activePiece == WhitePawn && 
        doneMove->endSquare.row == whitePiecePromotionRow ) ||
        ( doneMove->activePiece == BlackPawn &&
          doneMove->endSquare.row == blackPiecePromotionRow ) ) {
        doneMove->type = PromotionQueen;
        // other promotion types have to be switched later with some external 
        // logic...
      } else {
        doneMove->type = Capture;
      }
    }

    Piece pieceBackOnBoard = doneMove->activePiece;
    if ( type == Castling ) {
      if ( moveBuf->castlingFirstReturned == ColA ) {
        // any rook or king in a valid cannot return to col-a
        // if that was set as 'castlingFirstReturned' it means
        // that there was no first returned piece, and this 
        // actually is the first castling piece returning to the board
        pieceBackOnBoard = Unknown;
        moveBuf->castlingFirstReturned = moveBuf->change.square.column;
      } else {
        BoardPos posBuf;

        posBuf.column = moveBuf->castlingFirstReturned;
        if ( boardState->active == Black ) {
          posBuf.row = Row8;

          if ( posBuf.column == ColB || posBuf.column == ColG ) {
            swapPiece(BlackKing, boardState, &posBuf);
            pieceBackOnBoard = BlackRook;
          } else {
            swapPiece(BlackRook, boardState, &posBuf);
            pieceBackOnBoard = BlackKing;
          }

          doneMove->activePiece = BlackKing;
          doneMove->passivePiece = BlackRook;

          doneMove->startSquare.column = ColE;
          doneMove->startSquare.row = Row8;

          doneMove->endSquare.row = Row8;
          doneMove->type = Castling;

          if ( posBuf.column == ColB || posBuf.column == ColC ) {
            doneMove->endSquare.column = ColB;
          } else {
            doneMove->endSquare.column = ColG;
          }

        } else {
          posBuf.row = Row1;

          if ( posBuf.column == ColB || posBuf.column == ColG ) {
            swapPiece(WhiteKing, boardState, &posBuf);
            pieceBackOnBoard = WhiteRook;
          } else {
            swapPiece(WhiteRook, boardState, &posBuf);
            pieceBackOnBoard = WhiteKing;
          }

          doneMove->activePiece = WhiteKing;
          doneMove->passivePiece = WhiteRook;

          doneMove->startSquare.column = ColE;
          doneMove->startSquare.row = Row1;

          doneMove->endSquare.row = Row1;
          doneMove->type = Castling;

          if ( posBuf.column == ColB || posBuf.column == ColC ) {
            doneMove->endSquare.column = ColB;
          } else {
            doneMove->endSquare.column = ColG;
          }

        }
      }
    }

    // TODO
    // check that endsquare either matches the passive pieces
    // start square, or that the whole move matches the en passant pattern
    
    Piece oldInSquare = swapPiece(
      pieceBackOnBoard, boardState, &(moveBuf->change.square));

    // castling move becomes ready only after also the other 
    // piece returns to the board
    if ( type != Castling ) {
      // 'commit' move
      currGame->finMovesCount++;
      // clear buffer
      clearMoveBuffer(moveBuf);
    }

    return;
  }

  // error..?
}

void clearMoveBuffer(MoveBuffer *moveBuf) {
  moveBuf->firstLifted.piece = Empty;
  moveBuf->secondLifted.piece = Empty;
  moveBuf->castlingFirstReturned = ColA;
  // TODO is this enough clean up?
}
