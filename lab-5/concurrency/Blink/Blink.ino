/*
  Blink
 Turns on an LED on for one second, then off for one second, repeatedly.
 
 This example code is in the public domain.
 */
#include <proc.h>

// Pin 13 has an LED connected on most Arduino boards.

class Blink : Process {
  int led_id;
public:
  Blink(int led) { led_id = led; pinMode (led, OUTPUT); }
  void loop () {
    Serial.print (led_id);
    Serial.println (" -> Hello!");
    digitalWrite (led_id, HIGH);
    delay (200);
    digitalWrite (led_id, LOW);
    delay (200);
  }
};
    
    
// the setup routine runs once when you press reset:
void setup() {                
 // initialize the digital pin as an output.
  Serial.begin(9600);
  Process::Init();  
  Blink *bp;
  bp = new Blink(13);
}


// the loop routine runs over and over again forever:
void loop() {
 Process::Start();
}

