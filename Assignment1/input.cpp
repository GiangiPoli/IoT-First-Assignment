#include "input.h"
#include "Arduino.h"
#include "config.h"
#include "kernel.h"
#include "core.h"

#include <EnableInterrupt.h>

#define BOUNCING_TIME 100

// #define __DEBUG__

uint8_t inputPins[NUM_BUTTONS] = {BUT01_PIN, BUT02_PIN};
bool buttonPressed[NUM_BUTTONS] = {false, false};

/* used for debouncing */
long lastButtonPressedTimestamps[NUM_BUTTONS];
unsigned long buttonTimestamps[NUM_BUTTONS] = {0,0};

void buttonHandler(int i);
void buttonHandler0(){ buttonHandler(0); }
void buttonHandler1(){ buttonHandler(1); }

void (*buttonHandlers[NUM_BUTTONS])() = { buttonHandler0, buttonHandler1 };

void buttonHandler(int i){
  if (getCurrentState() == SLEEP_STATE){
    Serial.println("button handler");
  }else{
    long ts = millis();
    if (ts - lastButtonPressedTimestamps[i] > BOUNCING_TIME){
      lastButtonPressedTimestamps[i] = ts;
      int status = digitalRead(inputPins[i]);
      if (status == HIGH && !buttonPressed[i]) { 
          buttonPressed[i] = true;
          buttonTimestamps[i] = ts;
      }
    }
  }
}

bool isButtonPressedTogheter() {
  // if (!buttonPressed[0] || !buttonPressed[1]) {return false;}
  // unsigned long t0,t1;
  // noInterrupts();
  // t0 = buttonTimestamps[0];
  // t1 = buttonTimestamps[1];
  // interrupts();
  // unsigned long result = (t0 > t1) ? (t0 - t1) : (t1 - t0);
  // return result <= 200;
  return digitalRead(BUT01_PIN) == HIGH && digitalRead(BUT02_PIN) == HIGH;
}

void initInput(){
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(inputPins[i], INPUT);  
    enableInterrupt(inputPins[i], buttonHandlers[i], CHANGE);
  }
}

void resetInput(int buttonIndex){
  long ts = millis();

  /*Removed the for cycle so i could choose which button reset, without creatig new dedicated function*/
  buttonPressed[buttonIndex] = false;      
  lastButtonPressedTimestamps[buttonIndex] = ts;    
  
}
bool isButtonPressed(int buttonIndex){
  return buttonPressed[buttonIndex];
}

int getDifficultyLevel(){
  return map(analogRead(POT), 0, 1023, 1, 4);
}