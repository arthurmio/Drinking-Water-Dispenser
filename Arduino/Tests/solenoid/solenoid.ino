//////////////////////////////////////////////////////////////////////////
/// Name        : solenoid.ino
/// Description : Code for testing a 12V solenoid valve
/// Author      : Arthur MARIANO
/// Github      : https://github.com/arthurmio/Drinking-Water-Dispenser
/// Date        : 24/12/2021
//////////////////////////////////////////////////////////////////////////


const int solenoidPin = 4; 

void setup() {
  pinMode(solenoidPin, OUTPUT);
}

void loop() {
  digitalWrite(solenoidPin, HIGH);   // turn the solenoid on (HIGH is the voltage level)
  delay(5000);                       // wait for a second
  digitalWrite(solenoidPin, LOW);    // turn the solenoid off by making the voltage LOW
  delay(5000);
}
