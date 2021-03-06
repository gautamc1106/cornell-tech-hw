/*
 Life emulation problem

 Scientists have discovered alien life on planet Leviche 538X.

 Unlike earth-based lifeforms, Levicheans have three genders, he, she and it.

 On reaching adulthood, Levichean organisms go to a mating area in search of
 other Levicheans. When a Levichean of one gender comes together with two
 other Levicheans of the other two genders (for example, a she runs into a he
 and an it) in this area, they form a lifelong physical bond and attach
 themselves into a triad. Once in a triad, Levicheans wait sometime and then
 respawn, available to reproduce yet again. As an earth
 scientist, you have been tasked with simulating the mating habits of the
 Levicheans, using threads, locks and condition variables. Each Levichean
 is modeled by a thread. Fill in the missing code segments below such that

 a) the Levichean triad formation is modeled according to the specification
 above, paying special attention to make sure that the three-way join is
 simulated correctly,

 b) the code makes forward progress whenever possible to do so (i.e. your
 simulation should accommodate every mating opportunity that is present on
 Leviche 538X).
 */

#include <proc.h>

class MatingArea {

    /* We have declared one lock and one condition
     * variable here.  You may (or may not) find it
     * useful to declare additional condition variables
     * to solve the problem.  We just left these here as
     * a reminder on the syntax. */
    Lock* _l_he;
    Cond* _c_he;
    Lock* _l_she;
    Cond* _c_she;
    Lock* _l_it;
    Cond* _c_it;   
    Lock* _l_triad;
    Cond* _c_triad;    
    int nhe = 0; // Number of hes ready
    int nshe = 0; // Number of shes ready
    int nit = 0; // Number of its ready
    bool ready = false;

  public:
    MatingArea () {
      _l_he = new Lock();
      _c_he = new Cond(_l_he);
      _l_she = new Lock();
      _c_she = new Cond(_l_she);
      _l_it = new Lock();
      _c_it = new Cond(_l_it);
      _l_triad = new Lock();
      _c_triad = new Cond(_l_triad);
    }

    void he_ready() {
      /*TODO: Only return when there is a she and an it also ready*/
      _c_he->lock();
      nhe++;   
      if (nhe > 1) {
        _c_he->wait();
      }
      
      _c_triad->lock();
      if (nshe == 0 || nit == 0) {
        _c_triad->wait();
      } 
      nhe--;
      _c_triad->signal();
      _c_he->signal();     
      
    }

    void she_ready() {
      /*TODO: Only return when there is a he and an it also ready*/
      _c_she->lock();
      nshe++;   
      if (nshe > 1) {
        _c_she->wait();
      }
      
      _c_triad->lock();
      if (nhe == 0 || nit == 0) {
        _c_triad->wait();
      } 
      nshe--;
      _c_triad->signal();
      _c_she->signal();     
      
    }

    void it_ready() {
      /*TODO: Only return when there is a he and a she also ready*/
      _c_it->lock();
      nit++;   
      if (nit > 1) {
        _c_it->wait();
      }
      
      _c_triad->lock();
      if (nshe == 0 || nhe == 0) {
        _c_triad->wait();
      }
      nit--;
      _c_triad->signal();
      _c_it->signal();     
      
    }
};

MatingArea ma;

class He : Process {

  public:
    He () {}

    void loop () {
      delay(random(300, 1500)); //waste time
      Serial.println("He: I'm born!");
      Serial.println("He: Adult now, time to form a triad!");
      ma.he_ready(); //do not pass until there is a she and an it
      Serial.println("He: Yay, I'm part of a triad!");
    }
};

class She : Process {

  public:
    She () {}

    void loop () {
      delay(random(300, 1500)); //waste time
      Serial.println("She: I'm born!");
      Serial.println("She: Adult now, time to form a triad!");
      ma.she_ready(); //do not pass until there is a he and an it
      Serial.println("She: Yay, I'm part of a triad!");
    }
};

class It : Process {

  public:
    It () {}

    void loop () {
      delay(random(300, 1500)); //waste time
      Serial.println("It: I'm born!");
      Serial.println("It: Adult now, time to form a triad!");
      ma.it_ready(); //do not pass until there is an it and a she
      Serial.println("It: Yay, I'm part of a triad!");
    }
};


// the setup routine runs once when you press reset:
void setup() {

  He *h;
  She *s;
  It *i;

  Serial.begin(9600); // open serial terminal
  Process::Init();  // start the threading library

  ma = MatingArea();
  h = new He(); //start first thread
  h = new He(); //start second thread
  s = new She(); //start third thread
  i = new It(); //start fourth thread
}

// the loop routine runs over and over again forever:
void loop() {
  Process::Start();
}

