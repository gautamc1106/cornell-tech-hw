/*************************************************************************
 *
 *  Copyright (c) 2013 Rajit Manohar
 *  All Rights Reserved
 *
 **************************************************************************
 */
#ifndef __PROC_H__
#define __PROC_H__
//#define DEBUG_THREADS
#include "proc_intr.h"
#include "Arduino.h"
class Process {
 public:
  Process (int stk_size = 100);
  ~Process ();
  
  virtual void loop () = 0;	/* every class has to override this
				   function. */

  /*
   * Initialize concurrency
   */
  static void Init ();

  /*
   * Start concurrency!
   */
  static void Start ();

  /*
   * Select next process
   */
  static unsigned int Select (unsigned int cursp);

  
 private:
  unsigned int _sp;		/* stack pointer: state for the process! */
  Process *_next;		/* Queue pointer for process */
  volatile int _blocked;	/* blocked in some other queue */
  void *_stk;			/* allocated space */

  void _mkblocked() { _blocked = 1; 
  #ifdef DEBUG_THREADS
  Serial.println(F("MKBLOCKED"));
  #endif
  }
  void _clrblocked() { _blocked = 0;
#ifdef DEBUG_THREADS
  Serial.println(F("CLRBLOCKED"));
#endif
  }

  static Process *ready_hd, *ready_tl; /* ready queue */
  static Process *current_process;

  static void _insert (Process *p);
  static Process *_delete ();

  friend class Lock;
  friend class Cond;
};


class Lock {
 public:
  Lock ();

  void lock() { Acquire (); }
  void unlock() { Release (); }

  void Acquire ();
  void Release ();

  int _release ();

 private:
  Process *_hd, *_tl;		/* blocked processes waiting for the
				   lock */

  void _insert (Process *p);
  Process *_delete ();
    
  volatile int _lock;
};

class Cond {
 public:
  Cond (Lock *l);

  void lock () { _l->lock(); }
  void unlock () { _l->unlock (); }

  void wait ();
  void signal ();
  int waiting ();
  
 private:
  Lock *_l;
  Process *_hd, *_tl;

  void _insert (Process *p);
  Process *_delete ();
};

#endif /* __PROC_H__ */
