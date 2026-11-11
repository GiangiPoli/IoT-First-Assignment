#ifndef __OUTPUT__
#define __OUTPUT__

#define intervallo 30
#define intervalloLCD 500
#define intro "Welcome to the React! Game. Press Buttons to Start"
#define endTie "Game Over - Tie "
#define endMessage "Game Over - The winner is "

void introOutput();
void initOutput();
void endOutput();
void endGame();
void showGo();
void scoreboard();
void showNumber(int number);
void resetScreen();
void sleepScreen();
void stage1Output();
void allLedOff();
void greenL1();
void redL2();
void redL3();
void greenL4();

#endif