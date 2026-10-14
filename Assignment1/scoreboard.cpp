#include "scoreboard.h"
#include "config.h"

int teamScore[NUM_BUTTONS];

void resetScore(){
  for(int i=0; i < NUM_BUTTONS; i++){
    teamScore[i] = 0;
  }
}

void incrementScore(int buttonIndex){
  teamScore[buttonIndex] += 1;
}

void decreaseScore(int buttonIndex){
  if(teamScore[buttonIndex] > 0){
    teamScore[buttonIndex] -= 1;
  }
}

int getScore0(){
  return teamScore[0];
}
int getScore1(){
  return teamScore[1];
}