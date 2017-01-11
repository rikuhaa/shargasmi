A (rather unnecessarily complicated) sensor chess board project meant to be run on a Teensy (~3.2) connected to a sensory chess board

The project is mainly meant for teaching me how to code C and do small hobby electronics / embedded projects (so the code is certainly not of a reference quality)

The goal for the project is to provide relatively passive tool that records a chess game history and does not need active interaction with a chess program while playing (even though that would also be possible). For this reason a lot more functionality is done in the embedded code as would usually make sense.

The games played on the board are recorded and if something interesting happened, it is possible to print the PGN of the game for later analysis. There are also some other functionality, eg. setting up positions on the board and printing out the FEN descriptions of a position for analysis with a chess program.

The status of the project is that it is working on a proof-of-concept level but there are several improvements that should be done (to the code as well as to the sensor board I am running the code on).

The main design for reading the occupation of the squares of the chess board by using reed switches and scanning the rows is copied from SolusChess project shared by Gerardo Mateo (aka Berger)
  - [https://sites.google.com/site/bergersprojects/reedcb](https://sites.google.com/site/bergersprojects/reedcb)

## Running unit tests
1. Install Ceedling [http://www.throwtheswitch.org/ceedling](http://www.throwtheswitch.org/ceedling)

2. Install Ceedling requirements into the project by running 'ceedling new shargasmi' in the parent folder

  - this will not override important files

3. Change into shargasmi folder and run 'rake test:all' to see that everything works and tests pass

## Testing on command line
1. Build the project with the main method in 'test_driver/sysin\_chessdriver.c'

  - eg. gcc -o build/sysin\_chessdriver.out src/*.c test\_driver/sysin_chessdriver.c 

2. Run the executable

  - 'build/sysin_chessdriver.out'

3. Interact with the code using the text interface (commands can be found by looking at main()-method in 'test_driver/sysin\_chessdriver.c')

## Building for Teensy
1. Install Teensyduino (Arduino IDE and Teensy) and setup for Teensy (correct board etc, also USB-mode 'keyboard' is needed)

  - [https://www.pjrc.com/teensy/td\_download.html](https://www.pjrc.com/teensy/td_download.html)

2. Make sure that the shargasmi-library code (src-folder) is available as a library through the Arduino IDE

 - easiest way is to add a link to the project's src-folder into the 'Arduino/libraries' folder (and name the link 'shargasmi')

3. Open the sketch (.ino-file) in the Arduino IDE from the 'sketches'-folder