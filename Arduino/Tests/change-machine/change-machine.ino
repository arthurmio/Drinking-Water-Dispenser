//////////////////////////////////////////////////////////////////////////
/// Name        : change-machine.ino
/// Description : Code for testing the change machine CH-926
/// Author      : Arthur MARIANO
/// Github      : https://github.com/arthurmio/Drinking-Water-Dispenser
/// Date        : 26/12/2021
//////////////////////////////////////////////////////////////////////////
#include <Arduino.h>

const int changeMachinePin = 3;
//volatile int pulse_freq = 0 ;
volatile float coinsValue = 0.00;
int coinsChange = 0;

void setup() {
  pinMode(changeMachinePin, INPUT); 
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(changeMachinePin), pulseMachine, RISING);
}

void loop() {
  if(coinsChange==1){
    coinsChange = 0;
    Serial.print("COINS = ");
    Serial.println(coinsValue);
  }
}

void pulseMachine() // interrupt fonction for flowMeter
{
  coinsValue  = coinsValue + 0.5;
  coinsChange = 1;
}
