
const int buttonPin = 10; //button pin
const int latchPin = 8; //latch pin
const int dataPin = 11; // data pin
const int clockPin = 12; // clock pin
//const int ledPin = 13;  
const int a = 5; //for looping through columns of LED
const int cols[] = { 2, 3, 4, 5, 6 }; //pins of arduino connected to LED
bool buttonIntent = false; //it will be true if pressed
bool lastButtonState = false;
int MorsePattern[5] = { -1, -1, -1, -1, -1 }; // will change after input is taken through button
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long dotToDashTime = 300; // duration to differentiate 
unsigned long intentTimeout = 3000;
unsigned long pressTime = 0;
unsigned long releaseTime = 0;
int ledEnable = LOW;
int morse[10][5] = { { 1, 1, 1, 1, 1 }, //morse code for dots and dashes
    { 0, 1, 1, 1, 1 },
    { 0, 0, 1, 1, 1 },
    { 0, 0, 0, 1, 1 },
    { 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0 },
    { 1, 1, 0, 0, 0 },
    { 1, 1, 1, 0, 0 },
    { 1, 1, 1, 1, 0 } };
const unsigned char ZERO[] = { 0x41, 0x41, 0x41, 0x7F, 0x7F }; // to display on LED
const unsigned char ONE[] = { 0x0, 0x0, 0x0, 0x0, 0x7F }; 
const unsigned char TWO[] = { 0x49, 0x49, 0x49, 0x4F, 0x79 };
const unsigned char THREE[] = { 0x49, 0x49, 0x49, 0x49, 0x7F };
const unsigned char FOUR[] = { 0x8, 0x8, 0x8, 0x78, 0x7F };
const unsigned char FIVE[] = { 0x49, 0x49, 0x49, 0x79, 0x4F };
const unsigned char SIX[] = { 0x49, 0x49, 0x49, 0x7F, 0x4F };
const unsigned char SEVEN[] = { 0x40, 0x40, 0x40, 0x7F, 0x40 };
const unsigned char EIGHT[] = { 0x49, 0x49, 0x49, 0x7F, 0x7F };
const unsigned char NINE[] = { 0x49, 0x49, 0x49, 0x79, 0x7F };
const unsigned char ERROR_INPUT[] = { 0x14, 0x8, 0x14, 0x63, 0x63 };
const unsigned char* DIGITS[] = { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE };
const int num_press = 5;
void setup()
{
    pinMode(buttonPin, INPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    for (int i = 0; i < a; i++) {
        pinMode(cols[i], OUTPUT); // when high its off!
        digitalWrite(cols[i], HIGH);
    }
    Serial.begin(9600);
}

void LEDMatrix(int num) //function to display on LED
{
    const unsigned char* led_disp;
    if (num == -1)
        led_disp = ERROR_INPUT;
    else
        led_disp = DIGITS[num];
    for (int c = 0; c < 1000; c++) {
        for (int i = 0; i < a; i++) { //looping through column
            digitalWrite(cols[i], LOW);//set col to low
            digitalWrite(latchPin, LOW);
            shiftOut(dataPin, clockPin, MSBFIRST, led_disp[i]);// to check which row is high/low
            digitalWrite(cols[i], HIGH);
            digitalWrite(latchPin, HIGH);//reset latch
        }
        delay(6);
    }
}
void loop()
{
    determineButtonIntent();
  
}
void determineButtonIntent()
{ //debouncing
    bool reading = digitalRead(buttonPin); //check if reading has changed

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
        lastButtonState = reading;
        return;
    }

    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTime <= debounceDelay) {
        return;
    }

    bool nwbt = lastButtonState; //update button state
    if (nwbt != buttonIntent) {
        buttonIntent = nwbt;
        btdebounce(nwbt);
    }
}

void btdebounce(bool pressed)
{
    if (pressed) {
        pressTime = millis();
        return;
    }
    releaseTime = millis();
    int duration = releaseTime - pressTime < dotToDashTime; //check time of each input 
    int mc = (duration) ? 0 : 1; // convert it into 0's and 1's
    for (int i = 0; i < 5; i++) {
        if (MorsePattern[i] == -1) {
            MorsePattern[i] = mc; 
            if (i == 4) {
                check(); //check if its similar to stored array
            }
            break;
        }
    }
}

void check()
{
    int num = -1;
    for (int i = 0; i < 10; i++) {
        num = i;
        for (int j = 0; j < 5; j++) {
            if (morse[i][j] != MorsePattern[j]) {
                num = -1;
                break;
            }
        }
        if (num != -1) {
            break;
        }
    }
    if (num != -1) {
      Serial.print(num);
      Serial.print(" ");
        LEDMatrix(num);//if they match then its sent to led to display after getting decoded in this function 
    }
    for (int i = 0; i < 5; i++) {
        MorsePattern[i] = -1;
    }
}

