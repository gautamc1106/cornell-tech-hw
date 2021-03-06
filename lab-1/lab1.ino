char num;                     //Variable to hold the input from serial
int index;                    //Holds the index for the morseCodes array, if the input was a number between 0 & 9
bool numAvail = false;        //To keep track of when there is a valid number available

//Holds morse codes for all the numbers. Index corresponds to the number.
String morseCodes[] = {"-----",           // 0
                        ".----",          // 1
                        "..---",          // 2
                        "...--",          // 3
                        "....-",          // 4
                        ".....",          // 5
                        "-....",          // 6
                        "--...",          // 7
                        "---..",          // 8
                        "----."           // 9
                       };          

//The setup code that runs once
void setup() {
  //Initialize serial:
  Serial.begin(9600);

  //Initialize digital pin 13 as an output, which is the LED-light
  pinMode(13, OUTPUT);
}

//Depending on the input number, display that mores code on the LED-light
void loop() {
  if(numAvail)  {
    //Get the morse code corresponding to input number
    String morseCode = morseCodes[index];

    //Loop though the code character by character. 
    //If it's a dot, then call the dotBlink() method, else call the dashBlink() method
    int i = 0;
    while(i < 5) {
      String subCode = morseCode.substring(i,i+1);
      if (subCode == ".") {
        dotBlink();
      } else {
        dashBlink();
      }
      i++;
    }

    //Since the number has been displayed, there is nothing else to display
    numAvail = false;
  }
}

/*
 * Is called between each loop
 * Read the input and convert to number if possible
 */
void serialEvent()  {
  while(Serial.available())  {
    num = char(Serial.read());

    if(num >= 48 && num <= 57)  {
      index = num - '0';
      numAvail = true;
    }
  }
}

//Called when the moorse code has a dot in it.
//Makes the lamp blink
void dotBlink() {
  digitalWrite(13, HIGH);   // turn the LED on 
  delay(1000);              //Keep on for 1 sec
  digitalWrite(13, LOW);    // turn the LED off 
  delay(1000);              //Keep it off for 1 sec
}

//Called when the moorse code has a dash in it.
//Makes the lamp blink
void dashBlink() {
  digitalWrite(13, HIGH);   // turn the LED on
  delay(3000);              //Keep it on for 3 sec
  digitalWrite(13, LOW);    // turn the LED off
  delay(1000);              //Keep it off for 1 sec
}

