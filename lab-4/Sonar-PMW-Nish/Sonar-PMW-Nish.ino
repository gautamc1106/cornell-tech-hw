
/* Sonar distance measurement using PWM
   Author: Nishith Kumar - nk595@cornell.edu
   Sensor Operation from 30cm to 500cm
   Output range is 300uS for 300-mm to 5000uS for 5000-mm.
 *  */
#include <Servo.h>
#define NUMREAD 5

// Digital input for recieving PMW signal from sensor
const int pwmPin = 7;
const int sonarPin = 3;             // pin number of the sonar

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position


//Returns the length of the pulse in microseconds
unsigned long highPulse = 0;
unsigned long lowPulse = 0;
unsigned long distance = 0;

void setup() {
  //set-up digital input
  pinMode(pwmPin, INPUT);
  pinMode(sonarPin, OUTPUT);
  digitalWrite(sonarPin, LOW);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  //set-up serial port
  Serial.begin(9600);
}


/*
   getDistance()
   Function returns distance measured by sensor
    in cms.
    MAX : 500 cms
    MIN : 30 cms
    Sensor accuracy not gauranteed below 30 cms.
*/

long getDistance() {
  int i = 0;
  int count = 0;
  unsigned long sum = 0;
  unsigned long pulseTime = 0;
  unsigned long cms = 0;


  for (i = 0; i < NUMREAD ; i++) {
    pulseTime =  pulseIn(pwmPin, HIGH);

    if (pulseTime > 0) {
      sum = sum + pulseTime;
      count++;
    }

  }

  if (pulseTime > 0)
    pulseTime =  sum / count;

  /* Convert to cms */
  cms = pulseTime;

  return cms;
}


void measurements(int pos) {
  myservo.write(pos);              // tell servo to go to position in variable 'pos'
  delay(15);                       // waits 15ms for the servo to reach the position

  digitalWrite(sonarPin, HIGH);
  char pos_buffer[10];
  itoa(pos, pos_buffer, 10);
  Serial.write(pos_buffer);
  Serial.write(",");
  char dist_buffer[10];
  distance = getDistance();
  itoa(distance, dist_buffer, 10);
  Serial.write(dist_buffer);
  Serial.write(10);    // send a line feed/new line, ascii 10
}


void loop() {
  int start_degree;
  int end_degree;
  int increment;

  while (Serial.available() > 0) {
    start_degree = Serial.parseInt();
    end_degree = Serial.parseInt();
    increment = Serial.parseInt();
    Serial.println(start_degree);
    Serial.println(end_degree);
    Serial.println(increment);
    for (pos = start_degree; pos <= end_degree; pos += increment) { // goes from 0 degrees to 180 degrees
      if (Serial.read() != NULL) {
        break;
      }
      measurements(pos);
    }

    for (pos = end_degree; pos >= start_degree; pos -= increment) { // goes from 180 degrees to 0 degrees
      if (Serial.read() != NULL) {
        break;
      }
      measurements(pos);
    }
  }

}


