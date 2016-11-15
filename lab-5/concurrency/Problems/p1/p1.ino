/*
 Time Waster Wars: Reddit vs. 4chan

 a. Add synchronization primitives to ensure that
    (a) the club is exclusively redditors or 4channers, i.e. no redditors
        should enter as long as there are 4channers in the club,
        and vice versa,
    (b) the club should always be used as long as there are
        customers

    Note that starvation is not something you need to worry
    about. If the club becomes redditors and remains exclusively
    redditors for all time, the waiting 4channers will just have
    to get old at the door.

 Modify only the code of the class Club to make the program
 correct. (but add the LED manipulation to the other classes)

 Use the LED matrix for visual feedback on the state of the system.
 Look at the "TODO" comments. Pick different LEDs for the four 
 processes.

 Place your synchronization variables inside the Club instance.
 */

#include <proc.h>


void hang_out() {
  delay(random(300,1500)); //waste time
}

class Club {
  
  /* We have declared one lock and one condition
   * variable here.  You may (or may not) find it
   * useful to declare additional condition variables
   * to solve the problem.  We just left these here as
   * a reminder on the syntax. */
  Lock *_l;
  Cond *_c; 

  public:
    Club () {  //Constructor for club
      _l = new Lock();
      _c = new Cond(_l);
    }

    ~Club () { //Destructor for club
      delete _l;
      delete _c;
    }
    
    void redditor_enter() {
      /*TODO: make it so that this function only returns when it is safe for
       *      redditor to enter the club. */
    }

    void redditor_exit() {
      /*TODO: Exit the club. */
    }

    void fourchanner_enter() {
      /*TODO: make it so that this function only returns when it is safe for
       *      redditor to enter the club. */
    }

    void fourchanner_exit() {
      /*TODO: Exit the club. */
    }
};

Club daclub;

class Redditor: Process {
  int _id;

public:
  Redditor (int id) { 
    _id = id; 
  }

  void loop () {
  	daclub.redditor_enter();
    Serial.print("Redditor ");
    Serial.print(_id);
    Serial.println(": in the club");
    /* TODO: light up column #_id on the LED matrix */
    hang_out();
    daclub.redditor_exit();
  }
};

class Fourchanner: Process {
  int _id;

public:
  Fourchanner (int id) { 
    _id = id; 
  }

  void loop () {
  	daclub.fourchanner_enter();
    Serial.print("Fourchanner ");
    Serial.print(_id);
    Serial.println(": in the club");
    /* TODO: light up column #_id on the LED matrix */
    hang_out();
    daclub.fourchanner_exit();
  }
};


// the setup routine runs once when you press reset:
void setup() {                

  Redditor *r;
  Fourchanner *f;
  
  Serial.begin(9600); // open serial terminal
  Process::Init();  // start the threading library
  
  daclub = Club();
  r = new Redditor(1); //start first thread
  r = new Redditor(2); //start second thread
  f = new Fourchanner(4); //start third thread
  f = new Fourchanner(5); //start fourth thread
}

// the loop routine runs over and over again forever:
void loop() {
  Process::Start();
}

