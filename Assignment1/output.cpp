#include "output.h"
#include "Arduino.h"
#include "config.h"
#include <LiquidCrystal_I2C.h>
#include "scoreboard.h"
// #define __DEBUG__
int intervallo = 30;
int intervalloLCD = 500;
unsigned long aggiornamentoLCD = 0;
String intro = "Welcome to the React! Game. Press Buttons to Start";
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);
uint8_t outputPins[NUM_LED] = {L1, L2, L3, L4};
int led_value;
int f;
int pos;
unsigned long ultimoAggiornamento = 0;
String endTie = "Game Over - Tie ";
String endMessage = "Game Over - The winner is ";
boolean tie;

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

void endOutput(){
  if(tie == false){
    if (millis() - aggiornamentoLCD >= intervalloLCD) {
      aggiornamentoLCD = millis();
      lcd.setCursor(0, 0);
      Serial.println(endMessage.substring(pos, pos + 20));
      Serial.println(pos);
      lcd.print(endMessage.substring(pos, pos + 20));
      pos++;
      if(pos == 10) {
        pos = 0;
      }
    }    
  }else{
    lcd.setCursor(0, 0);
    lcd.print(endTie);
  }
}

void endGame(){
  if (getScore0() == getScore1()){
    tie = true;
  }else if (getScore0() > getScore1()){
    endMessage = endMessage + "P1  ";
    tie = false;
  }else if (getScore0() < getScore1()){
    endMessage = endMessage + "P2  ";
    tie = false;
  }
}

void showGo() {
  lcd.setCursor(10, 2);
  lcd.print("Go!");
}

void scoreboard() {
  lcd.setCursor(2, 1);
  lcd.print("P1");
  lcd.setCursor(3, 2);
  lcd.print(getScore0());
  lcd.setCursor(16, 1);
  lcd.print("P2");
  lcd.setCursor(16, 2);
  lcd.print(getScore1());
}

void showNumber(int number) {
  lcd.setCursor(9, 2);
  lcd.print(number);
}

void resetScreen() {
  lcd.clear();
  aggiornamentoLCD = 0;
  pos=0;
  endMessage = "Game Over - The winner is ";
}

void greenL1() {
  digitalWrite(L1, HIGH);
}

void greenL4() {
  digitalWrite(L4, HIGH);
}

void redL2(){
  digitalWrite(L2, HIGH);
}
void redL3(){
  digitalWrite(L3, HIGH);
}

void allLedOff() {
  analogWrite(L1, LOW);
  analogWrite(L2, LOW);
  analogWrite(L3, LOW);
  analogWrite(L4, LOW);
}





