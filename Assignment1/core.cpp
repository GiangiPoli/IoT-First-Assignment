#include "HardwareSerial.h"
#include "core.h"
#include "Arduino.h"
#include "kernel.h"
#include "input.h"
#include "output.h"
#include "scoreboard.h"
#include "config.h"


int count[2];
int reachedAt[2]; 

int randomWaitTime;
int numberShown;
bool isNumberShown;

int T1 = GAME_TIME;
float F;

void setupDifficulty(int level){
  const float factors[4] = {DIFFICULTY_EASY, DIFFICULTY_MEDIUM, DIFFICULTY_HARD, DIFFICULTY_IMPOSSIBLE}; 
  F  = factors[level - 1];  
}


void initCore(){
  Serial.begin(9600);
  randomSeed(analogRead(0));
  setupDifficulty(getDifficultyLevel());
}

void intro(){
  if (isJustEnteredInState()){
    resetScreen();
    Serial.println("Intro...");
  }
  introOutput();
  int dt = getCurrentTimeInState();
  //Serial.println(dt);
  if (dt > MAX_TIME_IN_INTRO_STATE){
    changeState(SLEEP_STATE);
  }
  if (isButtonPressedTogheter()) {
    changeState(STAGE1_STATE);
    resetInput(0);
    resetInput(1);
  }

  resetScore();
}

void sleep() {
  allLedOff();
  sleepScreen();
  sleepMode();
  changeState(INTRO_STATE);
}

void stage1(){
  if (isJustEnteredInState()){
    allLedOff();
    resetScreen();
    Serial.println("Stage1...");
  }
  showGo();
  /*Showing the ignition process*/
  if(getCurrentTimeInState() > 5000){
    changeState(STAGE2_STATE);
  }
}

void stage2() {
  if (isJustEnteredInState()){
    allLedOff();
    resetInput(0);
    resetInput(1);
    resetScreen();
    Serial.println("Stage2...");

    count[0] = count[1] = 0;
    reachedAt[0] = reachedAt[1] = 0;

    randomWaitTime = random(0, MAX_WAIT_BEFORE_NUMBER);
    numberShown = random(1,4);
    isNumberShown = false;
  }

  int dt = getCurrentTimeInState();
  /*Checking if players press the button too early...in that case they will lose a point*/
  if (dt < randomWaitTime){
    for (int i = 0; i < 2; i++){
      if (isButtonPressed(i)){
        decreaseScore(i);
        resetInput(i);
        (i == 0) ? redL2() : redL3();
        changeState(END_ROUND_STATE);
      }
    }
  }else{

   if (!isNumberShown){
    showNumber(numberShown);
    isNumberShown = true;
  }


  /*Counting buttons pression and memorizing the time needed to press them*/
  for (int i = 0; i < 2; i++){
    if (isButtonPressed(i)){
      count[i]++;
      resetInput(i);
      if (count[i] == numberShown){
        reachedAt[i] = dt;         
      }
    }
  }

  /*
   * The reason i do not want to stop the cycle after the first player pressed the button the right amount of time  
   * is because i want to keep checking if he presses the button some more time. Supposing the first player press the 
   * button the right amount of time, the second one can still press it more time than asked: so first player 
   * increase his score by one, while the second player get his score decrease by one point (all in one round)
   */


  /*When T1 expires, the round ends*/
  if (dt - randomWaitTime >= T1){
    Serial.print("pulsante 0:  ");
    Serial.println(count[0]);
    Serial.print("pulsante 1:  ");
    Serial.println(count[1]);
    /*Verifing if the players did not press the button at least the amount of times required or if the time remaining is over: in this case the game is over*/
    if ((reachedAt[0] == 0 && reachedAt[1] == 0) || (T1 == 0)){
      Serial.println("Going in final state..");
      changeState(FINAL_STATE);
      return;
    }
    
    bool valid0 = (count[0] == numberShown);
    bool valid1 = (count[1] == numberShown);
    
    /**/
    if (valid0 || valid1){
      if (valid0 && valid1){
        (reachedAt[0] <= reachedAt[1]) ? (incrementScore(0), greenL1()) : (incrementScore(1), greenL4()); 
      }else{
        (valid0) ? (incrementScore(0), greenL1()) : (incrementScore(1), greenL4());    
      }
    }


    bool over0 = (count[0] > numberShown);
    bool over1 = (count[1] > numberShown);

    /**/
    if (over0 || over1){
      if (over0 && over1){
        decreaseScore(0);
        decreaseScore(1);
        redL2();
        redL3();
      }else{
        (over0) ? (decreaseScore(0), redL2()) : (decreaseScore(1), redL3());    
      }
    }

    

    T1 -= F;
    changeState(END_ROUND_STATE);
  }
  }
}

void endRound(){
  if (isJustEnteredInState()){
    resetScreen();
    Serial.println("End round stage...");
  }

  /*Showing scoreboard for 3 second, then going back to stage 2*/
  if(getCurrentTimeInState() < 3000){
    scoreboard();
  }else{
    changeState(STAGE2_STATE);
  }
}

void finalize(){
  if (isJustEnteredInState()){
    Serial.println("Finalize...");
    resetScreen();
    endGame();
  }
  endOutput();
  if(getCurrentTimeInState() > 10000){
    changeState(INTRO_STATE);
  }
}