#include "output.h"
#include "Arduino.h"
#include "config.h"

// #define __DEBUG__

uint8_t outputPins[NUM_LED] = {L1, L2, L3, L4};
int led_value;
int f;

void initOutput(){
  led_value = 0;
  f = 1;
}

void introOutput(){
  if (led_value == 255 || led_value == 0) {
    f = -f;
  }
  led_value = led_value + f;
  analogWrite(2, led_value);
  analogWrite(3, 255 - led_value);
}
