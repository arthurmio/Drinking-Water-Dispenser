//////////////////////////////////////////////////////////////////////////
/// Name        : flow-meter.ino
/// Description : Code for testing the flow meter YF-S201
/// Author      : Arthur MARIANO
/// Github      : https://github.com/arthurmio/Drinking-Water-Dispenser
/// Date        : 24/12/2021
//////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <Time.h>

//Time
volatile float currentTime = 0.00;
volatile float lastTime = 0.00;

// Flow Meter
const int flowMeterPin = 2; 
const float factorK = 7.5;
volatile int pulse_freq = 0 ;
volatile float flow = 0.00;
volatile float waterVolume = 0.00;

void setup() {
  pinMode(flowMeterPin, INPUT); 
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(flowMeterPin), pulseFlow, RISING);
  currentTime = millis();
  lastTime = currentTime;
}

void loop() {
  currentTime = millis();
//  lastTime = currentTime;
  if(currentTime >= (lastTime + 1000)) // count always the flow every second 
  {   
      lastTime = currentTime; 
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      flow = (pulse_freq / factorK); // flow in L/min
      pulse_freq = 0; // Reset Counter
      Serial.print(flow, DEC); 
      Serial.println(" L/Min");
      //// Water Volume Counter (a changer en fonction du temps pour avoir uniquement le volume)
      //if(valve==1)
      //  waterVolume = volume(flow);
      //else
      //  waterVolume = 0;
  }
}

//// calcul of water volume
//float volume(float flow)
//{
//  waterVolume += flow;
//  return waterVolume;
//}

void pulseFlow() // interrupt fonction for flowMeter
{
  pulse_freq++;
}
