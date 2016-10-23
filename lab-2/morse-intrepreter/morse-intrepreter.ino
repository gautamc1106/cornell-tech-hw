
const int buttonPin = 2; // pushbutton pin
const int latchPin = 8;  // latch pin - pin12
const int dataPin = 11;   // data pin - pin14 
const int clockPin = 12;  // clock pin - pin11 
const int ledPin = 13;   // LED pin 
const int cath_col = 5;  // 5 cathode columns
const int COLS[] = {7, 3, 4 ,9, 5};   // arduino pins -> resistor -> 5 cathode columns

     
const unsigned char ZERO[] = {0x41, 0x41, 0x41, 0x7F,0x7F};
const unsigned char ONE[] = {0x0, 0x0, 0x0, 0x0, 0x7F};
const unsigned char TWO[] = {0x49, 0x49, 0x49,0x4F,0x79};
const unsigned char THREE[] = {0x49, 0x49, 0x49, 0x49,0x7F};
const unsigned char FOUR[] = {0x8, 0x8, 0x8, 0x78,0x7F};
const unsigned char FIVE[] = {0x49, 0x49, 0x49,0x79,0x4F};
const unsigned char SIX[] = {0x49, 0x49, 0x49, 0x7F,0x4F};
const unsigned char SEVEN[] = {0x40, 0x40, 0x40, 0x7F, 0x40};
const unsigned char EIGHT[] = {0x49, 0x49, 0x49,0x7F,0x7F};
const unsigned char NINE[] = {0x49, 0x49, 0x49, 0x79,0x7F};
const unsigned char ERROR_INPUT[] = {0x14, 0x8, 0x14, 0x63,0x63};
const unsigned char* DIGITS[] = { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};\
const int num_press = 5; // maximum num of pushes to record input
const long DOT_TIME = 300; //time limit for dot

// the debouncing algorithm : https://www.arduino.cc/en/Tutorial/Debounce

// variables that will change:
int buttonState = 0;                     // the current reading from the input pin
int lastButtonState = LOW;               // for debouncing (taken from https://www.arduino.cc/en/Tutorial/Debounce)
long lastDebounceTime = 0;               // the last time the output pin was toggled
long debounceDelay = 50;                 // the debounce time; increase if the output flickers
long lastTime = 0;                       // Last recorded timestamp  (used for mesuring duration)
boolean buttonWasPressed = false;        // Indicator of whether button was pressed in the last cycle
byte inputSignals[num_press];          // Input signal buffer
int inputSignalIndex = 0;                // Index into the input signal buffer

void resetInputSignals() {               // Reset the input signal buffer and index
  memset(inputSignals, 0, num_press);  // reset each element in array to 0
  inputSignalIndex = 0;
}

int translateInput(byte input[]) {
  int num; bool flag = 0;                 // create num and flag variabls
  if (input[0] == 0)                     // if first byte is 0 - i.e. a dot:
    num = 1;                              // num must be between 1 and 5. start at 1
  else                                    // otherwise:
    num = 6;                              // num must be between 6 and 9, or 0. start at 6
  // loop through the rest of the morse signals to determine the num  
  for (int i = 1; i < num_press; i++) {
    if (input[i] == input[0] && flag)     // handles error morse input
        return -1;                        // returns error morse code input
    else if (input[i] == input[0])        // if the first byte is equal to the current signal:
      num++;                              // increment the num
    else                                  // otherwise:
      flag = 1; 
  }       
  return num % 10;                        // returns num modulus 10 to handle the case of '0'
}

void displayOnLEDMatrix(int num) {
  const unsigned char* LED_DISPLAY;        // current led display pattern
  if (num == -1)                          // error
    LED_DISPLAY = ERROR_INPUT;            // display 'X'
  else                                    // else
    LED_DISPLAY = DIGITS[num];     // display current num
  for (int num_loops = 0 ; num_loops < 1000; num_loops++){    // loop it 1000 times for persistance of vision
    for(int i = 0; i < cath_col; i++) {
         digitalWrite(COLS[i],LOW);       // turn cathode to GND (on)
         digitalWrite(latchPin, LOW);     // turn latch off
         shiftOut(dataPin, clockPin, MSBFIRST, LED_DISPLAY[i]);    // enter data
         digitalWrite(COLS[i],HIGH);      // turn cathode off
         digitalWrite(latchPin, HIGH);    // turn latch on to output the data
    }
    delay(6);                           // display for 6ms
   }
}

void setup() {
  pinMode(buttonPin, INPUT);              // set button as input pin
  pinMode(latchPin, OUTPUT);              // set latch as output pin
  pinMode(clockPin, OUTPUT);              // set clock as output pin
  pinMode(dataPin, OUTPUT);               // set data as output pin
  pinMode(ledPin, OUTPUT);                // set led as output pin
  for(int i = 0; i < cath_col; i++) {     // loop through each col
    pinMode(COLS[i], OUTPUT);             // set col pin as output
    digitalWrite(COLS[i], HIGH);          // turn cathodes off
  }
  Serial.begin(9600);                     // init the serial port
  resetInputSignals();                    // reset input signal buffer
}
//debounce

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);   // get the current button state
  long currTime = millis();               // get the current timestamp
  long duration = currTime - lastTime;    // get elapsed time
  
  if (reading != lastButtonState)         // if the switch changed, due to noise or pressing:
    lastDebounceTime = millis();          
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState)           
      buttonState = reading;            
  }
  digitalWrite(ledPin, buttonState);      
  lastButtonState = reading;  
  
  if (buttonState == HIGH) {               // if the button is pressed (DEBOUNCED)
    if (!buttonWasPressed) {               // and the button was not pressed before
      buttonWasPressed = true;             // remember the button press
      lastTime = currTime;                 // record the time of the button press
    }                                      // end of if (button was not pressed)
  } else {                                 // the button is not pressed
    if (buttonWasPressed) {                // the button was just released

      if (duration < DOT_TIME){         // Check if duration exceeds DOT cutoff
        inputSignals[inputSignalIndex] = 0;
      }
      else{                                 // otherwise
        inputSignals[inputSignalIndex] = 1;// remember the DASH

      }
    inputSignalIndex++;                    // advances to the next signal buffer
    buttonWasPressed = false;              // consume previous button press
    lastTime = currTime;                   // record the time the button was released
    } else {                               // the button was not just released
      if (inputSignalIndex == num_press) {        // if the buffer is full

        int currNum = translateInput(inputSignals); // get num from morse code
        displayOnLEDMatrix(currNum);       // call function to display digit on led matrix
        resetInputSignals();               // reset the input signal buffer
      }
    }
  }
}
