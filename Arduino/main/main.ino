//////////////////////////////////////////////////////////////////////////
/// Name        : main.ino
/// Description : Principal code for drinking water dispenser
/// Author      : Arthur MARIANO
/// Github      : https://github.com/arthurmio/Drinking-Water-Dispenser
/// Date        : 26/12/2021
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

// Change Machine //
const int changeMachinePin = 3; // interrupt port
volatile int coinsChange = 0;
volatile float coinsValue = 0.00;
volatile float coinsValueSaved = 0.00;
volatile float moneyValue = 0.00;
//volatile bool payed = false;
volatile int payed = 0;

// Solenoid //
const int solenoidPin = 4; 
volatile int valve = 0;


///////////
// setup //
///////////
void setup() {
  pinMode(flowMeterPin, INPUT);
  pinMode(changeMachinePin, INPUT); 
  pinMode(solenoidPin, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(flowMeterPin), pulseFlow, RISING);
  attachInterrupt(digitalPinToInterrupt(changeMachinePin), pulseMachine, RISING);
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
  while(payed==1){
    currentTime = millis();
    if(currentTime >= (lastTime + 1000)) // count always the flow every second 
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
    }
  }
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
    valve = 0;
    payed = 0;
    Serial.println("0.50L water served");
    }
  }
  else if(moneyValue == 2){
    if(flowCounter >= 1){
    valve = 0;
    payed = 0;
    Serial.println("1L water served");
    }
  }
  coinsValueSaved=0;
}

// Change Machine //
void pulseMachine() // interrupt fonction for change machine
{
  coinsValue  = coinsValue + 0.5;
  start = millis();
}

// Solenoid //
void solenoidOpen(int moneyValue){
  if(moneyValue==1){ // 0.50sole
    //Serial.println("0.50Sole");
    payed = 1;
    valve = 1;
  }
  else if(moneyValue==2){ // 1sole
    //Serial.println("1Sole");
    payed = 1;
    valve = 1;
  }
  else
    payed = 0;
    valve = 0;
}
