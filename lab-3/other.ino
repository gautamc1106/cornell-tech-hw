// including software serial library
#include <SoftwareSerial.h>

int pulsePin = 3; // where sonar is
int distance = 0; // distance relayed by voltage
int buttonState = 0;
int buttonPin = 12; // where button is located

const byte rxPin = 2; // rxPin: the pin on which to receive serial data
const byte txPin = 3; // txPin: the pin on which to transmit serial data

// set up a new serial object
SoftwareSerial rangeFinder (rxPin, txPin);

void setup() {
  pinMode(pulsePin, INPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  rangeFinder.begin(9600);
}

void loop() {
  distance = pulseIn(pulsePin, HIGH);
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    rangeFinder.println(distance);
    delay(500);
  }
}
