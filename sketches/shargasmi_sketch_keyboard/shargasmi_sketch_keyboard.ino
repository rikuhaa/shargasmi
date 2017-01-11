#include <Bounce2.h>

#include <chessutils.h>
#include <chessmoves.h>
#include <chesstypes.h>
#include <chessrunner.h>

/*****************
 * DATA
 *****************/

/*
 * Chess state
 */
static ChessState chess;

static ChessMove moves[1000];
static int maxMovesPerGame = 1000;

static SquareChange change;
  
static int chessStrBufferSize = 300;
static char chessStrBuffer[300];

static ChessOutState latestOutputState;

elapsedMillis millisFromStart;

/*
 * pin setup
 */

//input
static const int doButtonPin = 17;
static const int whiteTimerPin = 18;
static const int playButtonPin = 19;
static const int blackTimerPin = 20;

Bounce doButtonBouncer = Bounce();
Bounce playButtonBouncer = Bounce();
Bounce whiteTimerBouncer = Bounce();
Bounce blackTimerBouncer = Bounce();

static ChessMode modeChoiceKnobVals[] = { Play, Setup, Meta, Config };
static int modeChoiceKnobValCount = 4;

static ChessAction actionChoiceKnobVals[] = { PrintPgn, PrintPgnLong, PrintFen, ResetAction };
static int actionChoiceKnobValCount = 4;

// TODO check these!!
static const int modeKnobPin = A10;
static const int actionChoiceKnobPin = A11;

// output 

typedef struct outled {
  bool pinAnalog;
  int ledPin;
  bool isOn;
  unsigned long long int nextChange;
  unsigned long int onPeriod;
  unsigned long int offPeriod;
  unsigned int blinkChangesLeft;
} OutputLed;

static const int unlimitedLedBlinkMagic = 99999;

static const int whiteTurnLedPin = 21;
static const int blackTurnLedPin = 22;
static const int workingLedPin = 13;
static const int actionConfirmPin = 23;

static OutputLed whiteTurnLed;
static OutputLed blackTurnLed;
static OutputLed workingLed;
static OutputLed actionConfirmLed;

// use the dac pin as one more led control output...
static const int errorLedAnalogPin = A14;
static OutputLed errorLed;

ChessMode lastReadMode;
elapsedMillis modeChangeCoolDownTimer;

static const int modeChangeCoolDownMillis = 500;

elapsedMillis whiteClockLastPressedTimer;
elapsedMillis blackClockLastPressedTimer;

static const int maxPromotionRollingIntervalMillis = 5000;

/*
 * chess square matrix handling data
 */

// matrix pins
static const int rowPins[] = {16, 15, 14, 12, 11, 10, 9, 8};
static const int columnPins[] = {0, 1, 2, 3, 4, 5, 6, 7};

Bounce squareStates[ROWS][COLUMNS];

const int minMoveMillis = 50;

elapsedMillis matrixDebounceMainTimer;

/*
 * HELPER FUNCTIONS
 */

ChessTimeStamp timeStamper()
{
  return millisFromStart;
}

bool isOccupied(Row row, Column column)
{
  return squareStates[row][column].read() == LOW;
}

void outputPrinter(char *toPrint)
{
  Keyboard.println(toPrint);
}

// defined in output section

void outputStateHandler(ChessOutState, ChessOutMessage);

void errorHandler(ChessErrorType, char*);

void setupChessData() {
  initEmptyChessState(&chess, chessStrBuffer, chessStrBufferSize,
    moves, maxMovesPerGame, &timeStamper);

  chess.outputPrinter = &outputPrinter;
  chess.isOccupied = &isOccupied;
  chess.outputStateHandler = &outputStateHandler;
  chess.errorHandler = &errorHandler;

  // TODO: Test init

  setRunnerMode(&chess, Play);
}

/*
 * General input and output handling functions
 */

// led handling helper functions

void setupLed(OutputLed *led, int ledPin, bool analogPin)
{
  if ( ! analogPin ) {
    pinMode(ledPin, OUTPUT);
  }
  led->pinAnalog = analogPin;
  led->ledPin = ledPin;
  turnOffLed(led, true);
}

void clearLedPeriod(OutputLed *led)
{
  led->blinkChangesLeft = 0;
  led->offPeriod = 0;
  led->onPeriod = 0;
}

void turnOffLed(OutputLed *led, bool clearPeriod)
{
  if ( led->pinAnalog ) {
    analogWrite(led->ledPin, 0);
  } else {
    digitalWrite(led->ledPin, LOW);
  }
  led->isOn = false;
  led->nextChange = 0;
  if ( clearPeriod ) {
    clearLedPeriod(led);
  }
}

void lightLed(OutputLed *led, bool clearPeriod) 
{
  if ( led->pinAnalog ) {
    analogWrite(led->ledPin, 4000);
  } else {
    digitalWrite(led->ledPin, HIGH);
  }
  led->isOn = true;
  led->nextChange = 0;
  if ( clearPeriod ) {
    clearLedPeriod(led);
  }
}

void switchLedState(OutputLed *led, bool clearPeriod) 
{
  if ( led->isOn ) {
    turnOffLed(led, clearPeriod);
  } else {
    lightLed(led, clearPeriod);
  }
}

void lightLedFor(OutputLed *led, unsigned long int periodMillis,
  bool isPeriodic)
{
  lightLed(led, true);
  led->nextChange = periodMillis + millisFromStart;
  if ( isPeriodic ) {
    led->onPeriod = periodMillis;
    led->offPeriod = periodMillis;
    led->blinkChangesLeft = unlimitedLedBlinkMagic;
  } else { 
    led->blinkChangesLeft = 1;
  }
}

void lightLedForBlinkCount(OutputLed *led, bool startOn, 
  unsigned long int onPeriodMillis, 
  unsigned long int offPeriodMillis, int changeBlinkCount)
{
  led->isOn = ! startOn;
  led->onPeriod = onPeriodMillis;
  led->offPeriod = offPeriodMillis;
  led->blinkChangesLeft = changeBlinkCount + 1;
  
  led->nextChange = millisFromStart - 1;
  updateLed(led);
}

void updateLed(OutputLed *led)
{
  if ( led->nextChange != 0 && 
    millisFromStart > led->nextChange ) {  
    
    switchLedState(led, false);
    if ( led->blinkChangesLeft != 0 && 
      led->blinkChangesLeft != unlimitedLedBlinkMagic ) {
      led->blinkChangesLeft--;
    }
    
    if ( led->blinkChangesLeft > 0 ) { 
      led->nextChange = millisFromStart + 
        ( led->isOn ? led->onPeriod : led->offPeriod );
    } else {
      clearLedPeriod(led);
    }
  }
}

// output handling functionsn

/* 
 * Quick feedback always when some input will be handled, eg.
 * when square change or state change has ben sent to the ChessRunner
 */
void showInputHandled() 
{
  lightLedFor(&workingLed, 300, false);
}

void handleOutputStateChange(ChessOutState newOutputState)
{
  if ( newOutputState == OutStateAfterReset ) {
    lightLed(&whiteTurnLed, true);
    lightLed(&blackTurnLed, true);
  } else if ( newOutputState == OutStatePlayPaused ) {
    lightLedFor(&whiteTurnLed, 1000, true);
    lightLedFor(&blackTurnLed, 1000, true);
  } else if ( newOutputState == OutStatePlayWhite ) {
    lightLed(&whiteTurnLed, true);
    turnOffLed(&blackTurnLed, true);
  } else if ( newOutputState == OutStatePlayBlack ) {
    turnOffLed(&whiteTurnLed, true);
    lightLed(&blackTurnLed, true);
  } else if ( newOutputState == OutStateMeta ) {
    lightLed(&whiteTurnLed, true);
    lightLedFor(&blackTurnLed, 1000, true);
  } else if ( newOutputState == OutStateConfig ) {
    lightLed(&blackTurnLed, true);
    lightLedFor(&whiteTurnLed, 1000, true);
  } else if ( newOutputState == OutStateSetup ) {
    lightLed(&whiteTurnLed, false);
    lightLedFor(&blackTurnLed, 1000, true);
  } else {
    Keyboard.println("--&&&&{{{{{}}}$$$$$&&&----");
    Keyboard.print("Unknown output state: ");
    Keyboard.println(newOutputState);
    Keyboard.println("--&&&&{{{{{}}}$$$$$&&&----");
  }
}

void handleBasicChessActionDone()
{
  lightLedFor(&actionConfirmLed, 1200, false);
  lightLedFor(&workingLed, 1200, false);
}

void handleOutputMessage(ChessOutMessage outputMsg)
{
//  Keyboard.println("--//--//--//--//--//--//--");
//  Keyboard.print("Got output message: ");
//  if ( outputMsg == ChessMoveDone ) {
//    Keyboard.println("ChessMoveDone");
//  } else if ( outputMsg == ChessClockChanged ) {
//    Keyboard.println("ChessClockChanged");
//  } else if ( outputMsg == PgnPrinted ) {
//    Keyboard.println("PgnPrinted");
//  } else if ( outputMsg == FenPrinted ) {
//    Keyboard.println("FenPrinted"); 
//  } else if ( outputMsg == PromotionRolledToKnight ) {
//    Keyboard.println("PromotionRolledToKnight");
//  } else if ( outputMsg == PromotionRolledToRook ) {
//    Keyboard.println("PromotionRolledToRook");
//  } else if ( outputMsg == PromotionRolledToBishop ) {
//    Keyboard.println("PromotionRolledToBishop");
//  } else if ( outputMsg == PromotionRolledToQueen ) {
//    Keyboard.println("PromotionRolledToQueen");
//  }
  if ( outputMsg == ChessMoveDone ) {
    lightLedFor(&actionConfirmLed, 600, false); 
  } else if ( outputMsg == ChessClockChanged ) {
    lightLedFor(&workingLed, 600, false);
  } else if ( outputMsg == PgnPrinted || outputMsg == FenPrinted ) {
    handleBasicChessActionDone();
  }
  // send timer messages to clock software
  else if ( outputMsg == ResetAction ) {
    handleBasicChessActionDone();
    // shift + r
    Keyboard.set_modifier(MODIFIERKEY_SHIFT);
    Keyboard.set_key1(KEY_R);
    Keyboard.send_now();

    Keyboard.set_modifier(0);
    Keyboard.set_key1(0);
    Keyboard.send_now();

  } else if ( outputMsg == ChessPausePressed ) {
    handleBasicChessActionDone();
    // shift + p
    Keyboard.set_modifier(MODIFIERKEY_SHIFT);
    Keyboard.set_key1(KEY_P);
    Keyboard.send_now();

    Keyboard.set_modifier(0);
    Keyboard.set_key1(0);
    Keyboard.send_now();
    
  } else if ( outputMsg == ChessClockWhitePressed ) {
    // shift + w
    Keyboard.set_modifier(MODIFIERKEY_SHIFT);
    Keyboard.set_key1(KEY_W);
    Keyboard.send_now();

    Keyboard.set_modifier(0);
    Keyboard.set_key1(0);
    Keyboard.send_now();
    
  } else if ( outputMsg == ChessClockBlackPressed ) {
    // shift + b
    Keyboard.set_modifier(MODIFIERKEY_SHIFT);
    Keyboard.set_key1(KEY_B);
    Keyboard.send_now();

    Keyboard.set_modifier(0);
    Keyboard.set_key1(0);
    Keyboard.send_now();
    
  }
  // TODO there should be some timer before these are shown,
  // because otherwise they will blend into each other
  else if ( outputMsg == PromotionRolledToKnight ) {
    lightLedForBlinkCount(&actionConfirmLed, true, 300, 200, 3);
  } else if ( outputMsg == PromotionRolledToRook ) {
    lightLedForBlinkCount(&actionConfirmLed, true, 300, 200, 5);
  } else if ( outputMsg == PromotionRolledToBishop ) {
    lightLedForBlinkCount(&actionConfirmLed, true, 300, 200, 7);
  } else if ( outputMsg == PromotionRolledToQueen ) {
    lightLedForBlinkCount(&actionConfirmLed, true, 300, 200, 9);
  } else {
//    TODO: warning?
//    Keyboard.println("--&&&&{{{{{}}}$$$$$&&&----");
//    Keyboard.print("Unknown output message: ");
//    Keyboard.println(outputMsg);
//    Keyboard.println("--&&&&{{{{{}}}$$$$$&&&----");
  }
//  Keyboard.println("--//--//--//--//--//--//--");
}

void outputStateHandler(ChessOutState outState, ChessOutMessage outMsg)
{
  if ( outState != NullStateMsg && outState != latestOutputState ) {
    handleOutputStateChange(outState);
  }
  if ( outMsg != NullOutputMsg ) {
    handleOutputMessage(outMsg);
  }
}

void errorHandler(ChessErrorType errorType, char* errorMsg)
{
  if ( errorType == NoError ) {
    turnOffLed(&errorLed, true);
  } else {
    lightLed(&errorLed, true);
  }

  // TODO whether errors are printed should be configurable somehow
  Keyboard.println(errorMsg);
}

void setupGeneralOutput() 
{

  setupLed(&whiteTurnLed, whiteTurnLedPin, false);
  setupLed(&blackTurnLed, blackTurnLedPin, false);
  setupLed(&workingLed, workingLedPin, false);
  setupLed(&actionConfirmLed, actionConfirmPin, false);

  setupLed(&errorLed, errorLedAnalogPin, true);
  
}

void updateGeneralOutput() 
{

  updateLed(&whiteTurnLed);
  updateLed(&blackTurnLed);
  updateLed(&workingLed);
  updateLed(&actionConfirmLed);
  updateLed(&errorLed);  
  
}

// input handling functions

void setupGeneralInput() {
  
  pinMode(doButtonPin, INPUT_PULLUP);
  pinMode(playButtonPin, INPUT_PULLUP);
  pinMode(whiteTimerPin, INPUT_PULLUP);
  pinMode(blackTimerPin, INPUT_PULLUP);

  doButtonBouncer.attach(doButtonPin);
  doButtonBouncer.interval(30);

  playButtonBouncer.attach(playButtonPin);
  playButtonBouncer.interval(30);

  whiteTimerBouncer.attach(whiteTimerPin);
  whiteTimerBouncer.interval(10);

  blackTimerBouncer.attach(blackTimerPin);
  blackTimerBouncer.interval(10);
  
}

ChessMode readModeKnob() {
  int modeKnobVal = analogRead(modeKnobPin);
  int modeIndex = (int) (modeChoiceKnobValCount * (modeKnobVal / 1024.0f));
  
  return modeChoiceKnobVals[modeIndex];
  
}

ChessAction readActionKnob() {

  int actionKnobVal = analogRead(actionChoiceKnobPin);
  int actionIndex = (int) (actionChoiceKnobValCount * (actionKnobVal / 1024.0f));
  
  return actionChoiceKnobVals[actionIndex];
}

void stabilizeGeneralInput() {

  // everything else than the mode don't store state but are only 
  // fire when done actions -> no need to stabilize state with input

  lastReadMode = readModeKnob();
  modeChangeCoolDownTimer = 0;
  
  bool done = false;
  while ( ! done ) {

    ChessMode modeInKnob = readModeKnob();
    if ( modeInKnob != lastReadMode ) {
      lastReadMode = modeInKnob;
      modeChangeCoolDownTimer = 0;
    } else if ( modeChangeCoolDownTimer > modeChangeCoolDownMillis ) {
      done = true;
      modeChangeCoolDownTimer = 0;
    }
    
  }

  setRunnerMode(&chess, lastReadMode);
  
}

void handleClockButtonPushed(Player forPlayer) {

  if ( forPlayer == White ) {
    doAction(&chess, WhiteClockPressed);
    if ( whiteClockLastPressedTimer < blackClockLastPressedTimer &&
      blackClockLastPressedTimer < maxPromotionRollingIntervalMillis && 
      chess.board.active == Black ) {
        doAction(&chess, RollPromotionAction);
    }
    whiteClockLastPressedTimer = 0;
  } else {
    doAction(&chess, BlackClockPressed);
    if ( blackClockLastPressedTimer < whiteClockLastPressedTimer &&
      blackClockLastPressedTimer < maxPromotionRollingIntervalMillis &&
      chess.board.active == White ) {
        doAction(&chess, RollPromotionAction);
    }
    blackClockLastPressedTimer = 0;
  }
  
  showInputHandled();
  
}

void debugStartPosition(bool (*isOccupied)(Row, Column));

void checkAndHandleGeneralInput() {

  // update all Bounce states
  doButtonBouncer.update();
  playButtonBouncer.update();
  whiteTimerBouncer.update();
  blackTimerBouncer.update();

  // test if mode has changed
  ChessMode modeInKnob = readModeKnob();
  if ( modeInKnob != lastReadMode ) {
    lastReadMode = modeInKnob;
    modeChangeCoolDownTimer = 0;
  } else if ( modeInKnob != chess.currMode && 
      modeChangeCoolDownTimer > modeChangeCoolDownMillis ) {
    showInputHandled();
    setRunnerMode(&chess, modeInKnob);
    modeChangeCoolDownTimer = 0;
  }

  // test if some button has been pushed and handle the action
  // if needed

  // the timer buttons send special signals (to debug) in config mode
  if ( chess.currMode != Config ) {
  
    if ( whiteTimerBouncer.fallingEdge() ) {
      handleClockButtonPushed(White);
    }
  
    if ( blackTimerBouncer.fallingEdge() ) {
      handleClockButtonPushed(Black);
    }
    
  } else {

    if ( whiteTimerBouncer.fallingEdge() ) {
      printDebug();
    }
    // maybe one of the buttons could eg. start / stop 
    // periodic debug message printing
    if ( blackTimerBouncer.fallingEdge() ) {
      printDebug();
    }

    if ( playButtonBouncer.fallingEdge() ) {
      debugStartPosition(&isOccupied);
    }
    
  }

  if ( playButtonBouncer.fallingEdge() ) {
    showInputHandled();
    if ( chess.currState == Running ) {
      doAction(&chess, PauseAction);
    } else {
      doAction(&chess, PlayAction);
    }
  }

  if ( doButtonBouncer.fallingEdge() ) {
    showInputHandled();
    ChessAction actionToDo = readActionKnob();
    doAction(&chess, actionToDo); 
  }
  
}

/*
 * chess square matrix handling functions
 */

void setupMatrixPins() {

  for (int row = 0; row < 8; row++ ) {
    pinMode(rowPins[row], OUTPUT);
    digitalWrite(rowPins[row], HIGH);
  }

  for (int column = 0; column < 8; column++ ) {
    pinMode(columnPins[column], INPUT_PULLUP);
  }

  for ( int row = 0; row < ROWS; row++ ) {
    for ( int column = 0; column < COLUMNS; column++ ) {
      squareStates[row][column] = Bounce();
      squareStates[row][column].attach(columnPins[column]);
      squareStates[row][column].interval(5);
    }
  }
  
}

void stabilizeOccupied() {

  bool changeInPeriod = true;

  while( changeInPeriod ) {
    matrixDebounceMainTimer = 0;
    changeInPeriod = false;
    
    while ( matrixDebounceMainTimer < minMoveMillis ) {
      if (readOccupied()) {
         changeInPeriod = true;
      }
    }
  }

  matrixDebounceMainTimer = 0;

}

bool readOccupied() {

    bool foundChange = false;
    for ( int row = 0; row < 8; row++ ) {
      digitalWrite(rowPins[row], LOW);

      for ( int column = 0; column < 8; column++ ) {

        squareStates[row][column].update(); 

        if ( squareStates[row][column].fell() ) {
          change.square.row = (Row) row;
          change.square.column = (Column) column;
          change.nowOccupied = true;
          if ( foundChange ) {
//            TODO: warning message?
//            Serial.println("MULTIPLE changes in scan");
          } else {
            foundChange = true;
          }
        } else if ( squareStates[row][column].rose() ) {
          change.square.row = (Row) row;
          change.square.column = (Column) column;
          change.nowOccupied = false;
          if ( foundChange ) {
//            TODO: warning message?
//            Serial.println("MULTIPLE changes in scan");
          } else {
            foundChange = true;
          }
        }
        
      }

      digitalWrite(rowPins[row], HIGH);
    }
    return foundChange;

}

void checkAndHandleSquareChanges() {
  
  if ( readOccupied() ) {

      showInputHandled();

      handleSquareChange(&chess, &change);

//      TODO: debug flag?
//      Serial.println("handled change to: ");
//      Serial.print(change.square.column);
//      Serial.print(change.square.row);
//      Serial.print(" changed to ");
//      Serial.println(change.nowOccupied);

  }
}

// main functions

void setup() {

  // TOD change to use keyboard simulation
  Serial.begin(9600);
  delay(1000);

//  Serial.println("STARTED LOADING");

  // analogWrites actullay
  // used only to get one more led controlled by DAC pin
  analogWriteResolution(12);

  // setup data structure

  setupChessData();

  // setup hardware

  setupMatrixPins();

  setupGeneralInput();

  setupGeneralOutput();

  // stabilize input and synchronize chess runner state to 
  // the input state after initial stabilization

  stabilizeOccupied();

  stabilizeGeneralInput();

//  Serial.println("LOADED");

}

static elapsedMillis debug = 0;

void loop() {

  checkAndHandleGeneralInput();

  checkAndHandleSquareChanges();

  updateGeneralOutput();

//  if ( debug > 2000 ) {
//    printDebug();
//    debug = 0;
//  }
  
}

// DEBUG STUFF

void printOccupied(bool (*isOccupied)(Row, Column));

void printDebug() {
  
  Keyboard.println("*********************************************");

  Keyboard.print("Current mode: ");
  if ( chess.currMode == Play ) {
    Keyboard.println("Play");
  } else if ( chess.currMode == Setup ) {
    Keyboard.println("Setup");
  } else if ( chess.currMode == Meta ) {
    Keyboard.println("Meta");
  } else if ( chess.currMode == Config ) {
    Keyboard.println("Config");
  } else {
    Keyboard.println("Unknown mode");
  }
  
  Keyboard.print("Current state: ");
  if ( chess.currState == Running ) {
    Keyboard.println("Running");
  } else if ( chess.currState == Paused ) {
    Keyboard.println("Paused");
  } else if ( chess.currState == AfterReset ) {
    Keyboard.println("After reset");
  } else {
    Keyboard.println("Unknown state");
  }

  Keyboard.println("****************");
  Keyboard.print("Current mode knob value: ");
  ChessMode currModeKnob = readModeKnob();
  if ( currModeKnob == Play ) {
    Keyboard.println("Play");
  } else if ( currModeKnob == Setup ) {
    Keyboard.println("Setup");
  } else if ( currModeKnob == Meta ) {
    Keyboard.println("Meta");
  } else if ( currModeKnob == Config ) {
    Keyboard.println("Config");
  } else {
    Keyboard.println("Unknown mode");
  }
  Keyboard.print("Analog mode reading: ");
  Keyboard.println(analogRead(modeKnobPin));

  Keyboard.print("Current action knob value: ");
  ChessAction actionReading = readActionKnob();
  if ( actionReading == PrintPgn ) {
    Keyboard.println("PrintPgn");
  } else if ( actionReading == PrintPgnLong ) {
    Keyboard.println("PrintPgnLong");
  } else if ( actionReading == PrintFen ) {
    Keyboard.println("PrintFen");
  } else if ( actionReading == ResetAction ) {
    Keyboard.println("ResetAction");
  } else {
    Keyboard.println("Unknown mode");
  }

  Keyboard.print("Analog action reading: ");
  Keyboard.println(analogRead(actionChoiceKnobPin));  

  Keyboard.println("**************");

  Keyboard.println("FEN: ");
  doAction(&chess, PrintFen);

  Keyboard.println("PGN:");
  doAction(&chess, PrintPgnLong);

  Keyboard.println("Curr board");
  
  printBoard(chess.board.squareStates);

  Keyboard.println("Curr occupied matrix");

  printOccupied(&isOccupied);

  Keyboard.println("*********************************************");
  
}

void printBoard(Piece positions[8][8]) {
  Keyboard.println("---------------------");

  for (int row = 7; row >= 0; row-- ) {
    Keyboard.print(getRowName((Row) row));
    Keyboard.print("\t");
    for (int column = 0; column < 8; column++) {
      char squareStr = getPieceFENName(positions[row][column]);
      Keyboard.print(squareStr);
      Keyboard.print("\t");
      
    }
    Keyboard.println("");
  }

  Keyboard.print("\t");
  for (int column = 0; column < 8; column++) {
    Keyboard.print(getColumnName((Column) column));
    Keyboard.print("\t");
  }
  Keyboard.println("");

  Keyboard.println("---------------------");  
}

void printOccupied(bool (*isOccupied)(Row, Column)) {
  Keyboard.println("---------------------");

  for (int row = 7; row >= 0; row-- ) {
    Keyboard.print(getRowName((Row) row));
    Keyboard.print("\t");
    for (int column = 0; column < 8; column++) {
      char squareStr;
      if ( isOccupied((Row) row, (Column) column) ) {
        squareStr = 'x';  
      } else {
        squareStr = '-';
      }
      Keyboard.print(squareStr);
      Keyboard.print("\t");
      
    }
    Keyboard.println("");
  }

  Keyboard.print("\t");
  for (int column = 0; column < 8; column++) {
    Keyboard.print(getColumnName((Column) column));
    Keyboard.print("\t");
  }
  Keyboard.println("");

  Keyboard.println("---------------------");  
}

void debugStartPosition(bool (*isOccupied)(Row, Column)) {

  int errorRow = -1;
  int errorCol = -1;

  for (int row = 7; row >= 0; row-- ) {
    for (int column = 0; column < 8; column++) {
      if ( ! isOccupied((Row) row, (Column) column) && 
        ( row == 7 || row == 6 || row == 1 || row == 0 ) ) {
          errorRow = row;
          errorCol = column;
      } else if ( isOccupied((Row) row, (Column) column) && 
        ( row == 2 || row == 3 || row == 4 || row == 5 ) ) {
          errorRow = row;
          errorCol = column;
      }
    }
  }

  if ( ! ( errorRow == -1 || errorCol == -1 ) ) {
    Keyboard.println("found error");
    Keyboard.println(errorRow);
    Keyboard.println(errorCol);

    // startOn, onPeriodMilfound errorlis, offPeriodMillis, changeBlinkCount
    
    lightLedForBlinkCount(&actionConfirmLed, false, 500, 400, 2 * (errorCol + 1));
  }

}

