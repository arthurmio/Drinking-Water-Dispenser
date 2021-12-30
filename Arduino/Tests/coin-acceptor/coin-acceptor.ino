//////////////////////////////////////////////////////////////////////////
/// Name        : coin-acceptor.ino
/// Description : Code for testing the coin-acceptor CH-926
/// Author      : Arthur MARIANO
/// Github      : https://github.com/arthurmio/Drinking-Water-Dispenser
/// Date        : 30/12/2021
//////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <Time.h>

//Time
volatile float currentTime = 0.00;
volatile float lastTime = 0.00;
volatile float start = 0.00;
volatile float coinsValueSavedTime = 0.00;

const int coinAcceptorPin = 3;
volatile float coinsValue = 0.00;
volatile float coinsValueSaved = 0.00;


void setup() {
  pinMode(coinAcceptorPin, INPUT); 
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(coinAcceptorPin), pulseMachine, RISING);
  
}

void loop() {
  currentTime = millis();
  if(currentTime <= (start+1000)){
    coinsValueSaved = coinsValue;
    coinsValueSavedTime = millis();
  }
  else{
    coinsValue = 0;
  }
  if(currentTime >= (coinsValueSavedTime+1000)){
    if(coinsValueSaved>=5 && coinsValueSaved <= 11)
      Serial.println("0.50 soles");
    else if(coinsValueSaved>11)
      Serial.println("1 soles");
  }
}

void pulseMachine() // interrupt fonction for coin acceptor
{
  coinsValue  = coinsValue + 0.5;
  start = millis();
}
