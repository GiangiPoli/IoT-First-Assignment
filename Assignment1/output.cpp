#include "output.h"
#include "Arduino.h"
#include "config.h"
#include <LiquidCrystal_I2C.h>
#include "scoreboard.h"
// #define __DEBUG__
int intervallo = 30;
int intervalloLCD = 500;
int aggiornamentoLCD = 0;
String intro = "Welcome to the React! Game. Press Buttons to Start";
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);
uint8_t outputPins[NUM_LED] = {L1, L2, L3, L4};
int led_value;
int f;
int pos;
int ultimoAggiornamento = 0;

void initOutput(){
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  led_value = 0;
  f = 5;
  lcd.init();
  lcd.backlight();
  pos = 0;  
}

void introOutput(){
  if (millis() - ultimoAggiornamento >= intervallo) {
    ultimoAggiornamento = millis();
    analogWrite(L2, led_value);
    analogWrite(L3, 255 - led_value);
    led_value = led_value + f;

    if(led_value <= 0 || led_value >= 255) {
      f = -f;
    }
  }

  if (millis() - aggiornamentoLCD >= intervalloLCD) {
    aggiornamentoLCD = millis();
    lcd.setCursor(0, 0);
    lcd.print(intro.substring(pos, pos + 20));
    pos++;
    if(pos == 31) {
      pos = 0;
    }
  }
}
void showGo() {
  lcd.clear();
  lcd.setCursor(10, 2);
  lcd.print("Go!");
}

void scoreboard() {
  lcd.clear();
  lcd.setCursor(3, 2);
  lcd.print(getScore0());
  lcd.setCursor(16, 2);
  lcd.print(getScore1());
}

void createNumber() {
  lcd.clear();
  lcd.setCursor(9, 2);
  lcd.print("2");
}
