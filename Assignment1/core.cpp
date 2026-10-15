#include "core.h"
#include "Arduino.h"
#include "kernel.h"
#include "input.h"
#include "output.h"
#include "scoreboard.h"
#include "config.h"

#define MAX_TIME_IN_INTRO_STATE 10000
#define MAX_TIME_IN_STAGE2_STATE 10000
#define MAX_WAIT 5000

int randomWaitTime;
int count0, count1;

void initCore(){
  Serial.begin(9600);
  randomSeed(analogRead(0));
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
    count0=0;
    count1=0;
    //randomWaitTime = random(0, MAX_WAIT);
    //Serial.println(randomWaitTime);
  }

  /*If buttons get pressed bedfore the number gets displayed, the player will lose a point*/
  if(getCurrentTimeInState() < 3000){
    if(isButtonPressed(0) || isButtonPressed(1)){
      int i;
      if(isButtonPressed(0)){
        i = 0;
      }else{
        i = 1;
      }
      decreaseScore(i);
      resetInput(i);
      Serial.println("Indice i -> " + i);
      changeState(END_ROUND_STATE);
    }
  }else{
    /*Show random number*/
    createNumber();
    if(isButtonPressed(0) || isButtonPressed(1)){
      int i;
      if(isButtonPressed(0)){
        i = 0;
        count0++;
      }else{
        i = 1;
        count1++;
      }

      resetInput(i);
    }

    if(count0 == 2 || count1 == 2){
      if(count0 == 2){
        incrementScore(0);
      }else{
        incrementScore(1);
      }
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
  }
}