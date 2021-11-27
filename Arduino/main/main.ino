#include <Arduino.h>
#include <Time.h>

//Time
volatile float currentTime = 0.00;
volatile float lastTime = 0.00;

//const int buttonPin = ?;

// Flow Meter
const int flowMeterPin = 2; 
const float factorK = 7.5;
volatile int pulse_freq = 0 ;
volatile float flow = 0.00;
volatile float waterVolume = 0.00;

// Solenoid Valve
//const int solenoidPin = ?; 
volatile int valve = 0;

// Change Machine
const int changeMachinePin = 3;
int coinsChange = 1;
volatile float coinsValue = 0.00;


void setup() {
  pinMode(flowMeterPin, INPUT); 
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(flowMeterPin), pulseFlow, RISING); // Setup Interrupt (interrupt pin in Nano is 2&3 / ex:https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)
  attachInterrupt(digitalPinToInterrupt(changeMachinePin), pulseMoney, RISING);
  currentTime = millis();
  lastTime = currentTime;
  //bool button;
}

void loop() {
  currentTime = millis();
  if(currentTime >= (lastTime + 1000)) // count always the flow every second 
  {   
      lastTime = currentTime; 
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      flow = (pulse_freq / factorK); // flow in L/min
      pulse_freq = 0; // Reset Counter
      Serial.print(flow, DEC); 
      Serial.println(" L/Min");

      // Change Machine
      

      // Water Volume Counter (a changer en fonction du temps pour avoir uniquement le volume)
      if(valve==1)
        waterVolume = volume(flow);
      else
        waterVolume = 0;
  }
}

// calcul of water volume
float volume(float flow)
{
  waterVolume += flow;
  return waterVolume;
}

void pulseFlow() // interrupt fonction for flowMeter
{
  pulse_freq++;
}

void pulseMoney() // interrupt fonction for Change Machine
{
  coinsValue += 0.5;
  coinsChange = 1;  
}
