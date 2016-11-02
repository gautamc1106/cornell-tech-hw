
const int buttonPin = 10;
const int latchPin = 8;
const int dataPin = 11;
const int clockPin = 12;
const int ledPin = 13;
const int a = 5;
const int cols[] = { 2, 3, 4, 5, 6 };
bool buttonIntent = false;
bool lastButtonState = false;
int MorsePattern[5] = { -1, -1, -1, -1, -1 };
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long dotToDashTime = 300;
unsigned long intentTimeout = 3000;
unsigned long pressTime = 0;
unsigned long releaseTime = 0;
int ledEnable = LOW;
int morse[10][5] = { { 1, 1, 1, 1, 1 },
    { 0, 1, 1, 1, 1 },
    { 0, 0, 1, 1, 1 },
    { 0, 0, 0, 1, 1 },
    { 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0 },
    { 1, 1, 0, 0, 0 },
    { 1, 1, 1, 0, 0 },
    { 1, 1, 1, 1, 0 } };
const unsigned char ZERO[] = { 0x41, 0x41, 0x41, 0x7F, 0x7F };
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
        pinMode(cols[i], OUTPUT);
        digitalWrite(cols[i], HIGH);
    }
    Serial.begin(9600);
}

void LEDMatrix(int num)
{
    const unsigned char* led_disp;
    if (num == -1)
        led_disp = ERROR_INPUT;
    else
        led_disp = DIGITS[num];
    for (int c = 0; c < 1000; c++) {
        for (int i = 0; i < a; i++) {
            digitalWrite(cols[i], LOW);
            digitalWrite(latchPin, LOW);
            shiftOut(dataPin, clockPin, MSBFIRST, led_disp[i]);
            digitalWrite(cols[i], HIGH);
            digitalWrite(latchPin, HIGH);
        }
        delay(6);
    }
}
void loop()
{
    determineButtonIntent();
  
}
void determineButtonIntent()
{
    bool reading = digitalRead(buttonPin);

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
        lastButtonState = reading;
        return;
    }

    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTime <= debounceDelay) {
        return;
    }

    bool nwbt = lastButtonState;
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
    int duration = releaseTime - pressTime < dotToDashTime;
    int symbol = (duration) ? 0 : 1;
    for (int i = 0; i < 5; i++) {
        if (MorsePattern[i] == -1) {
            MorsePattern[i] = symbol;
            if (i == 4) {
                check();
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
        LEDMatrix(num);
    }
    for (int i = 0; i < 5; i++) {
        MorsePattern[i] = -1;
    }
}

