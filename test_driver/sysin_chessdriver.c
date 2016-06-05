#include "../src/chessrunner.h"
#include "../src/chesstypes.h"
#include "../src/chessutils.h"
#include <stdio.h>
#include <string.h>

static ChessState chess;

int tryParseSquareChange(char*, SquareChange*);
bool isOccupied(Row, Column);
void outputPrinter(char*);
ChessTimeStamp timeStamper();

int main(void)
{
  SquareChange change;
  char command[1000];
  
  int chessStrBufferSize = 10000;
  char chessStrBuffer[10000];

  initEmptyChessState(&chess, chessStrBuffer, chessStrBufferSize,
  	&timeStamper);

  chess.outputPrinter = &outputPrinter;
  chess.isOccupied = &isOccupied;

  printf("%s\n", "Next command: ");
  fgets(command, 1000, stdin);
  strtok(command, "\n");

  while ( strcmp("quit", command) != 0 ) {

  	// actions
  	if ( strcmp("pgn", command) == 0 ) {
  		doAction(&chess, PrintPgn);
  	} else if ( strcmp("lpgn", command) == 0 ) {
  		doAction(&chess, PrintPgnLong);
  	} else if ( strcmp("fen", command) == 0 ) {
  		doAction(&chess, PrintFen);
  	} else if ( strcmp("reset", command) == 0 ) {
  		doAction(&chess, ResetAction);
  	} else if ( strcmp("play", command) == 0 ) {
  		doAction(&chess, PlayAction);
  	} else if ( strcmp("pause", command) == 0 ) {
  		doAction(&chess, PauseAction);
  	} else if ( strcmp("prom", command) == 0 ) {
  		doAction(&chess, RollPromotionAction);
  	} 
  	// modes
  	else if ( strcmp("mode_play", command) == 0 ) {
  		setRunnerMode(&chess, Play);
  	} else if ( strcmp("mode_setup", command) == 0 ) {
  		setRunnerMode(&chess, Setup);
  	}
  	// square change or error
  	else {
  		if ( tryParseSquareChange(command, &change) == 0) {
  			handleSquareChange(&chess, &change);
  		} else {
  			printf("Unknown command: %s\n", command);
  		}
  	}

 	printf("%s\n", "Next command: ");
 	fgets(command, 1000, stdin);
 	strtok(command, "\n");

  }

  return 0;
}

ChessTimeStamp timeStamper()
{
	return 1234;
}

int tryParseSquareChange(char* cmd, SquareChange *change)
{

	Column col = getColumnFromName(*cmd);
	if ( col == ColA && *cmd != 'a' ) {
		return 1;
	}
	cmd++;
	Row row = getRowFromName(*cmd);
	if ( row == Row1 && *cmd != '1' ) {
		return 1;
	}
	cmd++;

	if ( *cmd != ' ' ) {
		return 1;
	}
	cmd++;

	bool occupied = false;
	if ( *cmd == 't' ) {
		occupied = true;
	} else if ( *cmd == 'f' ) {
		occupied = false;
	} else {
		return 1;
	}

	change->square.row = row;
	change->square.column = col;
	change->nowOccupied = occupied;

	return 0;
}

bool isOccupied(Row row, Column column)
{
	// better test simulation would maybe be to have
	// an internal occupation matrix here, and try 
	// to see that it stays synchronized with the state's board
	return getPieceRowCol(
		&(chess.board), row, column) != Empty;
}

void outputPrinter(char* toPrint)
{
	printf("CHESS: %s\n", toPrint);
}