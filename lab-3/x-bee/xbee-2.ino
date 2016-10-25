const int pulsePin = 3; // where sonar is
int rxPin = 3; // rxPin: the pin on which to receive serial data
int txPin = 1; // txPin: the pin on which to transmit serial data
int pulseValue = 0; // distance relayed by voltage
int buttonPin = 0; // where button is located

void setup() {
  //
  pinMode(pulsePin, INPUT);
  Serial.begin(9600);
}

void loop() {
  distance = pulseIn(pulsePin, HIGH);
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println(distance);
    delay(500);
  }
}