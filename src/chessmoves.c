#include "./chesstypes.h"
#include "./chessutils.h"
#include "./chessmoves.h"

void handleMoveBoardChange(
  MoveBuffer *moveBuf, BoardState *boardState, ChessGame *currGame) 
{

  Column startCol;
  Row startRow;
  Column endCol;
  Row endRow;
  MoveType doneType;
  Piece doneActive;
  Piece donePassive;

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

    // lowering the piece back to same square could be done
    // eg. to correct the piece positioning

    if ( moveBuf->change.square.row == moveBuf->firstLifted.startPos.row &&
      moveBuf->change.square.column == moveBuf->firstLifted.startPos.column ) {
      // empty buffer to start, but don't 
      // record as a move

      clearMoveBuffer(moveBuf);

      return;
    }

    doneActive = moveBuf->firstLifted.piece;
    donePassive = Empty;

    startCol = moveBuf->firstLifted.startPos.column;
    startRow = moveBuf->firstLifted.startPos.row;

    endCol = moveBuf->change.square.column;
    endRow = moveBuf->change.square.row;

    // check if the move caused promotion

    if ( ( doneActive == WhitePawn && 
      endRow == whitePiecePromotionRow ) ||
      ( doneActive == BlackPawn &&
        endRow == blackPiecePromotionRow ) ) {
      doneType = PromotionQueen;
      // other promotion types have to be switched later with some external 
      // logic...
    } else {
      doneType = Move;
    }

    // 'commit' move
    handleMoveFinished(currGame, boardState,
        doneType, startCol, startRow,
        endCol, endRow, doneActive, donePassive);
    
    Piece oldInSquare = swapPiece(
      doneActive, boardState, &(moveBuf->change.square));
  
    if ( oldInSquare != Empty ) {
      // error, otherwise sub-move finished
    }

    if ( ! belongsToPlayer(boardState->active, doneActive) ) {
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

      // TODo: error?
      activeLifted = &moveBuf->firstLifted;
      passiveLifted = &moveBuf->secondLifted;

    }

    // record capture move
    // or check if castling

    if ( type != Castling ) {  

      doneActive = activeLifted->piece;
      donePassive = passiveLifted->piece;

      startCol = activeLifted->startPos.column;
      startRow = activeLifted->startPos.row;

      endCol = moveBuf->change.square.column;
      endRow = moveBuf->change.square.row;

      // NOTE: en-passant has nothing special from this point of view
      // because we are not trying to test validity here;
      // the returning piece only returns to empty square ('en passant' square)
      // but it still is a capture and the endsquare is marked right

      // check if the move caused promotion

      if ( ( doneActive == WhitePawn && 
        endRow == whitePiecePromotionRow ) ||
        ( doneActive == BlackPawn &&
          endRow == blackPiecePromotionRow ) ) {
        doneType = PromotionQueen;
        // other promotion types have to be switched later with some external 
        // logic...
      } else {
        doneType = Capture;
      }
    }

    Piece pieceBackOnBoard = doneActive;
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

          if ( posBuf.column == ColC || posBuf.column == ColG ) {
            swapPiece(BlackKing, boardState, &posBuf);
            pieceBackOnBoard = BlackRook;
          } else {
            swapPiece(BlackRook, boardState, &posBuf);
            pieceBackOnBoard = BlackKing;
          }

          doneActive = BlackKing;
          donePassive = BlackRook;

          startCol = ColE;
          startRow = Row8;

          endRow = Row8;
          doneType = Castling;

          if ( posBuf.column == ColC || posBuf.column == ColD ) {
            endCol = ColC;
          } else {
            endCol = ColG;
          }

        } else {
          posBuf.row = Row1;

          if ( posBuf.column == ColC || posBuf.column == ColG ) {
            swapPiece(WhiteKing, boardState, &posBuf);
            pieceBackOnBoard = WhiteRook;
          } else {
            swapPiece(WhiteRook, boardState, &posBuf);
            pieceBackOnBoard = WhiteKing;
          }

          doneActive = WhiteKing;
          donePassive = WhiteRook;

          startCol = ColE;
          startRow = Row1;

          endRow = Row1;
          doneType = Castling;

          if ( posBuf.column == ColC || posBuf.column == ColD ) {
            endCol = ColC;
          } else {
            endCol = ColG;
          }

        }

        swapPiece(
          pieceBackOnBoard, boardState, &(moveBuf->change.square));

        // clean up after the second part of a castling move
        handleMoveFinished(currGame, boardState,
          doneType, startCol, startRow,
          endCol, endRow, doneActive, donePassive);

        clearMoveBuffer(moveBuf);

        return;

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

      handleMoveFinished(currGame, boardState,
        doneType, startCol, startRow,
        endCol, endRow, doneActive, donePassive);

      // clear buffer
      clearMoveBuffer(moveBuf);
    }

    return;
  }

  // error..?
}

void handleMoveFinished(
  ChessGame *currGame, BoardState *boardState,
  MoveType type, Column startCol, Row startRow, 
  Column endCol, Row endRow,
  Piece activePiece, Piece passivePiece)
{

  int currMoveInd = currGame->finMovesCount;

  // if max-move, error...

  ChessMove *doneMove = &(currGame->moves[currMoveInd]);

  doneMove->activePiece = activePiece;
  doneMove->passivePiece = passivePiece;

  doneMove->startSquare.column = startCol;
  doneMove->startSquare.row = startRow;

  doneMove->endSquare.column = endCol;
  doneMove->endSquare.row = endRow;

  doneMove->type = type;

  currGame->finMovesCount++;

  // update active player 
  if ( boardState->active == Black ) {
    boardState->active = White;
  } else {
    boardState->active = Black;
  }

  // would not be too hard to update here also the fen settings
  // to the board state, ie. was double pawn jump -> en passant
  // rook / king moved -> castling availability change etc...

}

void clearMoveBuffer(MoveBuffer *moveBuf) {
  moveBuf->firstLifted.piece = Empty;
  moveBuf->secondLifted.piece = Empty;
  moveBuf->castlingFirstReturned = ColA;
  // TODO is this enough clean up?
}

/*
* Mode for setting up a board mainly to send a FEN and then
* analyze it. Can also be used as a starting point to a normal
* game.
*
* The main logic of setup mode is as follows:
*   1. MoveBuffer is periodically cleaned (outside this function)
*      to allow easily removing pieces from the board
*   2. When MoveBuffer is empty, and a SquareChange happens that
*      marks a square occupied, the Piece is marked to be the one
*      with that square as the starting square
*   3. Square becomes occupied -> there is one Piece in move buffer
*      -> that piece is moved to the new square
*   4. Square becomes empty, already one moving, assume that 
*      the moving piece will be placed on the empty square
*         -> if next change does not make the 'captured'
*          square occupied, swap the 'captured' piece as moving
*          and assume the first moving piece was taken off the 
*          board
*      - this logic is chained for successive empty events
*      without any square becoming occupied
*  TODO: this rule is maybe confusing, and just removing 
*  piece and using the clear interval before reintroducing
*  would work almost as good
*   5. At any moment, a Piece can be "converted" by moving 
*      it to the start square of given piece type, and then
*      lifting and lowering it there again (before lift, lower
*      keeps old type also in that square)
*
*
*/
void handleSetupBoardChange(
  MoveBuffer *moveBuf, BoardState *boardState) 
{

  // first piece change for the move sequence
  if ( moveBuf->firstLifted.piece == Empty ) {
    
    // this is the case when a piece is introduced to the board
    if ( moveBuf->change.nowOccupied ) {
      
      Piece cameToBoard = getPieceForStartPos(&(moveBuf->change.square));

      // TODO error if cameToBoard == empty (non start pos)

      swapPiece(cameToBoard, boardState, &(moveBuf->change.square));

      // also could check that the square was empty, though if non-empty
      // square would have marked to become occupied, that would be a 
      // chessrunner user error

      // the move was handled completely, clear buffer
      clearMoveBuffer(moveBuf);

    } else {

      Piece lifted = swapPiece(Empty, boardState, &(moveBuf->change.square));
      
      if ( lifted == Empty ) {
        // this is also an error situation, mainly telling
        // that the boardState was wrong...
      }

      // for setup purposes, the starting positions does not really
      // matter that much actually, but let's record it anyway
      // might be important in 'capture' or 'off-board' decisions
      moveBuf->firstLifted.piece = lifted;
      moveBuf->firstLifted.startPos.row = 
        moveBuf->change.square.row;
      moveBuf->firstLifted.startPos.column = 
        moveBuf->change.square.column;
    }

    return;
  }

  if ( moveBuf->firstLifted.piece != Empty 
    && moveBuf->secondLifted.piece == Empty ) {
    
    if ( moveBuf->change.nowOccupied ) {
      
      // this is the case maybe bit dubious 'case 5' for quickly 
      // changing piece type
      if ( isSameSquare(&(moveBuf->firstLifted.startPos), 
        &(moveBuf->change.square)) ) {

        Piece startPosPiece = getPieceForStartPos(&(moveBuf->change.square));
        if ( startPosPiece != Empty ) {

          swapPiece(startPosPiece, boardState, &(moveBuf->change.square));

          clearMoveBuffer(moveBuf);

          return;

        } else {
          // just clean the buffer, otherwise ignore
          clearMoveBuffer(moveBuf);
          return;        
        }


      } 
      // this is the case for normal move with existing piece from square to square
      else {

        // TODO also here could check that the square really was empty
        // for debug purposes

        swapPiece(moveBuf->firstLifted.piece, 
          boardState, &(moveBuf->change.square));

        clearMoveBuffer(moveBuf);

        return;

      }

    } 
    // starting a move that might but a piece on a square of another piece 
    // or just move the first piece out off the board
    else {

      Piece lifted = swapPiece(Empty, boardState, &(moveBuf->change.square));
      
      if ( lifted == Empty ) {
        // this is also an error situation, mainly telling
        // that the boardState was wrong...
      }

      // for setup purposes, the starting positions does not really
      // matter that much actually, but let's record it anyway
      // might be important in 'capture' or 'off-board' decisions
      moveBuf->secondLifted.piece = lifted;
      moveBuf->secondLifted.startPos.row = 
        moveBuf->change.square.row;
      moveBuf->secondLifted.startPos.column = 
        moveBuf->change.square.column;
    }

    return;
  }

  // cases were either 'capturing' pieces (ie. moving a piece to an 
  // already occupied piece)
  // or when moving some of the pieces off the board
  if ( moveBuf->firstLifted.piece != Empty 
    && moveBuf->secondLifted.piece != Empty ) {

    // third (or more) piece lifted, move 
    // the old pieces backwards, and discard the first lifted
    if ( ! moveBuf->change.nowOccupied ) {

      // move 'secondLifted' to be 'firstLifted' and discard the
      // old 'firstLifted'
      moveBuf->firstLifted.piece = moveBuf->secondLifted.piece;
      moveBuf->firstLifted.startPos.row = 
        moveBuf->secondLifted.startPos.row;
      moveBuf->firstLifted.startPos.column =
        moveBuf->secondLifted.startPos.column;

      // mark the newly lifted piece as the new 'firstLifted'

      Piece lifted = swapPiece(Empty, boardState, &(moveBuf->change.square));
      
      if ( lifted == Empty ) {
        // this is also an error situation, mainly telling
        // that the boardState was wrong...
      }

      // for setup purposes, the starting positions does not really
      // matter that much actually, but let's record it anyway
      // might be important in 'capture' or 'off-board' decisions
      moveBuf->secondLifted.piece = lifted;
      moveBuf->secondLifted.startPos.row = 
        moveBuf->change.square.row;
      moveBuf->secondLifted.startPos.column = 
        moveBuf->change.square.column;

      return;

    }
    // either the second-lifted square become occupied -> "setup capture"
    // or some other square become occupied -> second lifted moved there,
    //    the first lifted was moved out of the board
    else {
      // "setup capture"
      if ( isSameSquare(&(moveBuf->secondLifted.startPos), 
        &(moveBuf->change.square)) ) {

        swapPiece(moveBuf->firstLifted.piece, 
          boardState, &(moveBuf->change.square));
      
        // TODO: could check that the square was empty

      }
      // "discard first, move second"
      else {

        swapPiece(moveBuf->secondLifted.piece, 
          boardState, &(moveBuf->change.square));

      }

      clearMoveBuffer(moveBuf);
      return;
    }

  }

}