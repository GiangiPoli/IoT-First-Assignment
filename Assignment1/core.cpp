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
int T1 = 5000;
float F = 1000;
bool numberShown;

/*void setupDifficulty(int level){
  const float factors[4] = {0.95, 0.90, 0.85, 0.80}; 
  F  = factors[level - 1];
  T1 = 5000;   
}*/


void initCore(){
  Serial.begin(9600);
  randomSeed(analogRead(0));
  //setupDifficulty(4);
}

void intro(){
  if (isJustEnteredInState()){
    resetScreen();
    Serial.println("Intro...");
  }
  introOutput();
  int dt = getCurrentTimeInState();
  Serial.println(dt);
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
  sleepMode();
  changeState(INTRO_STATE);
}

void stage1(){
  if (isJustEnteredInState()){
    resetScreen();
    Serial.println("Stage1...");
  }
  showGo();
  /*Showing the ignition process*/
  if(getCurrentTimeInState() > 5000){
    changeState(STAGE2_STATE);
  }
}

void stage2(){
  if (isJustEnteredInState()){
    resetInput(0);
    resetInput(1);
    resetScreen();
    Serial.println("Stage2...");
    count[0] = count[1] = 0;
    reachedAt[0] = reachedAt[1] = 0;
    randomWaitTime = random(0, MAX_WAIT_BEFORE_NUMBER);
    numberShown = false;
  }

  int dt = getCurrentTimeInState();

  /*Checking if players press the button too early...in that case they will lose a point*/
  if (dt < randomWaitTime){
    for (int i = 0; i < 2; i++){
      if (isButtonPressed(i)){
        decreaseScore(i);
        resetInput(i);
        changeState(END_ROUND_STATE);
      }
    }
  }

  if (!numberShown){
    createNumber();
    numberShown = true;
  }


  /*Counting buttons pression and memorizing the time needed to press them the required amount of time*/
  for (int i = 0; i < 2; i++){
    if (isButtonPressed(i)){
      count[i]++;
      resetInput(i);
      if (count[i] >= 2){
        reachedAt[i] = dt;         
      }
    }
  }

  /*T1 time is over*/
  if (dt - randomWaitTime >= T1){
    
    /*Verifing if players did not reached. In this case the game is over*/
    if (reachedAt[0] == 0 && reachedAt[1] == 0){
      changeState(FINAL_STATE);
    }
    
    /*Verifing if players pressed the button the right amount of time, and who scored the point*/
    bool valid0 = (count[0] == 2);
    bool valid1 = (count[1] == 2);

    if (count[0] > 2) decreaseScore(0);
    if (count[1] > 2) decreaseScore(1);

    if (valid0 && valid1){
      incrementScore(reachedAt[0] <= reachedAt[1] ? 0 : 1);
    } else if (valid0){
      incrementScore(0);
    } else if (valid1){
      incrementScore(1);
    }

    T1 -= F;
    Serial.print("Tempo Rimanente -> ");
    Serial.println(T1);
        Serial.print("ReachedAt0 -> ");
    Serial.println(reachedAt[0]);
        Serial.print("ReachedAt1 -> ");
    Serial.println(reachedAt[1]);
    changeState(END_ROUND_STATE);
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
  }
}