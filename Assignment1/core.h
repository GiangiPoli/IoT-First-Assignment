#ifndef __CORE__
#define __CORE__

#define INTRO_STATE   1
#define STAGE1_STATE  2
#define STAGE2_STATE  3
#define FINAL_STATE   4 
#define SLEEP_STATE   5
#define END_ROUND_STATE 6

#define MAX_TIME_IN_INTRO_STATE 10000
#define MAX_WAIT_BEFORE_NUMBER 5000

/* core business logic  */

void initCore();
void intro();
void stage1();
void stage2();
void endRound();
void finalize();
void sleep();

#endif