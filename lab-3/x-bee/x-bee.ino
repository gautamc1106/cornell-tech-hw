// including software serial library
#include <SoftwareSerial.h>

// serialPin = where serial output is coming in from
// distance = distance relayed by range finder
// buttonPin = where button is located
int serialPin = 2, distance = 0, buttonPin = 3, buttonState = 0;
char newLine = "\n"; // single measurement is complete

// rxPin: the pin on which to receive serial data
// txPin: the pin on which to transmit serial data
int rxPin = 2, txPin = 3;

// set up a new serial object
SoftwareSerial rangeFinder (rxPin, txPin);

void setup() {
  pinMode(serialPin, INPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  rangeFinder.begin(19200);
}

void loop() {
  distance = pulseIn(serialPin, HIGH);
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    rangeFinder.print(distance);
    rangeFinder.print(newLine);
    delay(100);
  }
}
