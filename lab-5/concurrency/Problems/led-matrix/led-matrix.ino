const int ledcol[5] = {             // PINs for cathode columns of LED matrix
  2, 3, 4, 5, 6
};
const int ledrow[7] = {             // PINs for anode rows of LED matrix
  7, 8, 9, 10, 11, 12, 13
};


void setup() {
  for (int col = 0; col < 5; col++) {
    pinMode(ledcol[col], OUTPUT);
    digitalWrite(ledcol[col], HIGH);
  };
  for (int row = 0; row < 7; row++) {
    pinMode(ledrow[row], OUTPUT);
    digitalWrite(ledrow[row], HIGH);
  };
}

void loop() {
  for (int row = 0; row < 7; row++){
    for (int col = 0; col < 5; col++){
      digitalWrite(ledcol[row], LOW);
      delay(3);              
      digitalWrite(ledcol[row], HIGH);
    };
    delay(10);
  };
}
