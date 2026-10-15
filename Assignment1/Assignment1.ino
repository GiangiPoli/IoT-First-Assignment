#include "kernel.h"
#include "config.h"
#include "core.h"
#include "input.h" 
#include "output.h"
#include "scoreboard.h"

void setup() {
  initCore();
  initInput();
  changeState(INTRO_STATE);
  initOutput();
  resetScore();
}

void loop(){ 
  updateStateTime(); 
  switch (getCurrentState()) { 
  case INTRO_STATE:
    intro();
    break;
  case STAGE1_STATE:
    stage1();
    break;
  case STAGE2_STATE:
    stage2();
    break;
  case FINAL_STATE:
    finalize();
    break;
  case SLEEP_STATE:
    sleep();
  }
}