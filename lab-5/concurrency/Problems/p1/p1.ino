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

 Place your synchronization variables inside the Club instance.
 */

#include <proc.h>


void hang_out() {
  //delay(random(300,1500)); //waste time
  delay(2000); //waste time
}

class Club {
  
  /* We have declared one lock and one condition
   * variable here.  You may (or may not) find it
   * useful to declare additional condition variables
   * to solve the problem.  We just left these here as
   * a reminder on the syntax. */
  Lock *_l;
  Cond *_c; 
  int nr = 0; // Number of redditors in the club
  int nf = 0; // Number of fourchanners in the club
  
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
      _c->lock();
      if (nf > 0) {
        Serial.println(F("Redditor waiting"));
        _c->wait();
      }
      nr++;
      _c->unlock();
    }

    void redditor_exit() {
      _c->lock();
      Serial.print(F("Number of redditors in the club "));
      Serial.println(nr);
      nr--;
      if (_c->waiting() && nr == 0) { 
        _c->signal();
      } else {
        _c->unlock();
      }
    }

    void fourchanner_enter() {
      _c->lock();
      if (nr > 0) {
        Serial.println(F("4chan waiting"));
        _c->wait();
      }
      nf++;
      _c->unlock(); 
    }

    void fourchanner_exit() {
      _c->lock();
      Serial.print(F("Number of fourchanners in the club "));
      Serial.println(nf);
      nf--;
      if (_c->waiting() && nf == 0) { 
        _c->signal();
      } else {
        _c->unlock();
      } 
    }
};

Club *daclub;

class Redditor: Process {
  int _id;

public:
  Redditor (int id) { 
    _id = id; 
  }

  void loop () {
    Serial.print(F("Redditor "));
    Serial.print(_id);
    Serial.println(F(": trying to enter"));
    daclub->redditor_enter();
    Serial.print(F("Redditor "));
    Serial.print(_id);
    Serial.println(F(": in the club"));
    /* TODO: light up column #_id on the LED matrix */
    hang_out();
    daclub->redditor_exit();
    Serial.print(F("Redditor "));
    Serial.print(_id);
    Serial.println(F(": out the club"));
    hang_out();
  }
};

class Fourchanner: Process {
  int _id;

public:
  Fourchanner (int id) { 
    _id = id; 
  }

  void loop () {
    Serial.print(F("Fourchanner "));
    Serial.print(_id);
    Serial.println(F(": trying to enter"));
    daclub->fourchanner_enter();
    Serial.print(F("Fourchanner "));
    Serial.print(_id);
    Serial.println(F(": in the club"));
    /* TODO: light up column #_id on the LED matrix */
    hang_out();
    daclub->fourchanner_exit();
    Serial.print(F("Fourchanner "));
    Serial.print(_id);
    Serial.println(F(": out the club"));
    hang_out();
  }
};


// the setup routine runs once when you press reset:
void setup() {                

  Redditor *r;
  Fourchanner *f;
  
  Serial.begin(9600); // open serial terminal
  Process::Init();  // start the threading library
  daclub = new Club();
  r = new Redditor(1); //start first thread
  r = new Redditor(2); //start second thread
  f = new Fourchanner(4); //start third thread
  f = new Fourchanner(5); //start fourth thread
}

// the loop routine runs over and over again forever:
void loop() {
  Process::Start();
}

