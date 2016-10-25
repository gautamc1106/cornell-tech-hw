/** LAB 3 */

// set up serial port
#include <SoftwareSerial.h>

// to see input on LED
const int buttonPin = 12;
int buttonState = 0, ledPin = 3, ledValue = 0;

int ledPin = 3;
int ledValue = 0;

// for Arduino Fio
int rxPin = 0, txPin = 1;

// interface to xbee using serial port
SoftwareSerial xbee(rxPin, txPin);

// setting up pin modes
void setup() {
  pinMode(ledPin, INPUT);
  Serial.begin(9600);
}

// loop to read button pin state
void loop() {
  // check what distance is
  ledValue = pulseIn(ledPin, HIGH);
  Serial.print(ledValue);
  Serial.print("\n");
  buttonState = digitalRead(buttonPin);
  // if button is pressed, print out distance
  if (buttonState == HIGH) {
    xbee.print(ledValue);
    xbee.print("\n");
    delay(500);
  }
}
