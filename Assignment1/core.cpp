#include "core.h"
#include "Arduino.h"
#include "kernel.h"
#include "input.h"
#include "output.h"
#include "scoreboard.h"
#include "config.h"

#define MAX_TIME_IN_INTRO_STATE 10000
#define MAX_TIME_IN_STAGE2_STATE 10000

/* core business logic */

void initCore(){
  Serial.begin(9600);
}

void intro(){
  if (isJustEnteredInState()){
    Serial.println("Intro...");
  }
  introOutput();
  //introLCD();
  int dt = getCurrentTimeInState();
  Serial.println(dt);
  if (dt > MAX_TIME_IN_INTRO_STATE){
    sleepMode();
  }
  if (isButtonPressed(0) && isButtonPressed(1)) {
    changeState(STAGE1_STATE);
  }
  resetScore();
}

void stage1(){
  if (isJustEnteredInState()){
    Serial.println("Stage1...");
    resetInput();
  }

  /*Checking if the buttons get pressed*/
  for(int i=0; i < NUM_BUTTONS; i++){
    if(isButtonPressed(i)){
      decreaseScore(i);
    }
  }

  /*Showing the ignition process*/
  scoreboard();
  if(getCurrentTimeInState() > 5000){
    showGo();
    if(getCurrentTimeInState() > 7000){
      changeState(STAGE2_STATE);
    }
  }
}

void stage2(){
  if (isJustEnteredInState()){
    Serial.println("Stage2...");
  }

  /*Show random number*/
  createNumber();
  /* change the state if button 1 is pressed or max time elapsed*/
  if (isButtonPressed(1) || getCurrentTimeInState() > MAX_TIME_IN_STAGE2_STATE){
    changeState(FINAL_STATE);          
  }
}

void finalize(){
  if (isJustEnteredInState()){
    Serial.println("Finalize...");
  }
  changeState(INTRO_STATE);
}