#include "scoreboard.h"

int score0;
int score1;

void resetScore(){
  score0 = 0;
  score1 = 0;
}

void incrementScore0(){
  score0 += 1;
}
void incrementScore1() {
  score1 += 1;
}
void decreaseScore0(){
  if(score0 > 0) {
    score0 -= 1;
  }
}
void decreaseScore1(){
  if(score1 > 0){
    score1 -= 1;
  }
}

int getScore0(){
  return score0;
}
int getScore1(){
  return score1;
}