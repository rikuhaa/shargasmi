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

    // if max-move, error...

    ChessMove *doneMove = &currGame->moves[currMoveInd];

    doneMove->type = Move;

    doneMove->activePiece = moveBuf->firstLifted.piece;
    doneMove->passivePiece = Empty;

    doneMove->startSquare.column = moveBuf->firstLifted.startPos.column;
    doneMove->startSquare.row = moveBuf->firstLifted.startPos.row;

    doneMove->endSquare.column = moveBuf->change.square.column;
    doneMove->endSquare.row = moveBuf->change.square.row;

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

    // record capture move (TODO castling, promotion)    

    int currMoveInd = currGame->finMovesCount;

    // if max-move, error...

    ChessMove *doneMove = &currGame->moves[currMoveInd];

    doneMove->type = Capture;

    doneMove->activePiece = activeLifted->piece;
    doneMove->passivePiece = passiveLifted->piece;

    doneMove->startSquare.column = activeLifted->startPos.column;
    doneMove->startSquare.row = activeLifted->startPos.row;

    doneMove->endSquare.column = moveBuf->change.square.column;
    doneMove->endSquare.row = moveBuf->change.square.row;

    // TODO
    // check that endsquare either matches the passive pieces
    // start square, or that the whole move matches the en passant pattern

    // 'commit' move
    currGame->finMovesCount++;
    
    Piece oldInSquare = swapPiece(
      doneMove->activePiece, boardState, &(moveBuf->change.square));

    // 
    clearMoveBuffer(moveBuf);

    return;
  }

  // error..?
}

void clearMoveBuffer(MoveBuffer *moveBuf) {
  moveBuf->firstLifted.piece = Empty;
  moveBuf->secondLifted.piece = Empty;
  // TODO is this enough clean up?
}
