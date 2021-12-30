//////////////////////////////////////////////////////////////////////////
/// Name        : main.ino
/// Description : Principal code for drinking water dispenser
/// Author      : Arthur MARIANO
/// Github      : https://github.com/arthurmio/Drinking-Water-Dispenser
/// Date        : 27/12/2021
//////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <Time.h>

///////////////////
// Instantiation //
///////////////////

// Time //
volatile float currentTime = 0.00;
volatile float lastTime = 0.00;
volatile float start = 0.00;
volatile float coinsValueSavedTime = 0.00;

// Flow Meter //
const int flowMeterPin = 2; // interrupt port
const float factorK = 7.5;
volatile int pulse_freq = 0 ;
volatile float flow = 0.00;
volatile float waterVolume = 0.00;
volatile float flowCounter = 0.00;

// Coin Acceptor //
const int coinAcceptorPin = 3; // interrupt port
volatile float coinsValue = 0.00;
volatile float coinsValueSaved = 0.00;
volatile float moneyValue = 0.00;
volatile bool payed = false;

// Solenoid //
const int solenoidPin = 4; 


///////////
// setup //
///////////
void setup() {
  pinMode(flowMeterPin, INPUT);
  pinMode(coinAcceptorPin, INPUT); 
  pinMode(solenoidPin, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(flowMeterPin), pulseFlow, RISING);
  attachInterrupt(digitalPinToInterrupt(coinAcceptorPin), pulseMachine, RISING);
  currentTime = millis();
  lastTime = currentTime;
}


//////////
// loop //
//////////
void loop(){
  // money calcul
  currentTime = millis();
  if(currentTime <= (start+1000)){
    coinsValueSaved = coinsValue;
    coinsValueSavedTime = millis();
  }
  else{
    coinsValue = 0;
  }
  if(currentTime >= (coinsValueSavedTime+1000)){
    if(coinsValueSaved>=5 && coinsValueSaved <= 11){
      moneyValue = 1; // 0.50 sole
      solenoidOpen(moneyValue); 
    }
    else if(coinsValueSaved>11){
      moneyValue = 2; // 1 sole
      solenoidOpen(moneyValue);
    }
    else{
      moneyValue = 0; // no money
      solenoidOpen(moneyValue);
    }   
  }
  flowCounter=0.00;
  flow=0.00;
  pulse_freq=0.00;
  while(payed==true){
    currentTime = millis();
    if(currentTime >= (lastTime + 100)) // count always the flow every second 
    {   
      lastTime = currentTime; 
      flow = (pulse_freq / factorK) / 60; // flow in L/min
      pulse_freq = 0; // Reset Counter
      Serial.print(flow, DEC); 
      Serial.println(" L/Min");
      flowCounter = flowCounter + flow;
      Serial.print(flowCounter, DEC); 
      Serial.println(" L");
      flowCount(moneyValue, flowCounter);
      digitalWrite(solenoidPin, LOW); // solenoid open
    }
  }
  digitalWrite(solenoidPin, HIGH); // solenoid closed
}

///////////////
// Functions //
///////////////

// Flow Meter //
void pulseFlow() // interrupt fonction for flow meter
{
  pulse_freq++;
}

void flowCount(int moneyValue, float flowCounter){ // calcul for volume
  if(moneyValue == 1){
    if(flowCounter >= 0.50){
      payed = false;
      Serial.println("0.50L water served");
    }
  }
  else if(moneyValue == 2){
    if(flowCounter >= 0.97){
      payed = false;
      Serial.println("1L water served");
    }
  }
  coinsValueSaved=0;
}

// Coin Acceptor //
void pulseMachine(){ // interrupt fonction for coin acceptor
  coinsValue  = coinsValue + 0.5;
  start = millis();
}

// Solenoid //
void solenoidOpen(int moneyValue){
  if(moneyValue==1){ // 0.50sole
    payed = true;
  }
  else if(moneyValue==2){ // 1sole
    payed = true;
  }
  else
    payed = false;
}
