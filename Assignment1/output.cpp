#include "output.h"
#include "Arduino.h"
#include "config.h"
#include <LiquidCrystal_I2C.h>
// #define __DEBUG__

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);
uint8_t outputPins[NUM_LED] = {L1, L2, L3, L4};
int led_value;
int f;
String intro = "Welcome to the React! Game. Press Buttons to Start";
int pos;

void initOutput(){
  led_value = 0;
  f = 1;
  lcd.init();
  lcd.backlight();
  pos = 0;  
}

void introOutput(){
  if (led_value == 255 || led_value == 0) {
    f = -f;
  }
  led_value = led_value + f;
  analogWrite(2, led_value);
  analogWrite(3, 255 - led_value);

  lcd.setCursor(0, 0);
  lcd.print(intro.substring(pos, pos + 20));
  pos++;

  if(pos == 31) {
    pos = 0;
  }

  delay(100);
}
