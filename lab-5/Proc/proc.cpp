/*************************************************************************
 *
 *  Copyright (c) 2013 Rajit Manohar
 *  All Rights Reserved
 *
 **************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include "proc.h"
#include "Arduino.h"

#define assert(x) if (!(x)) { exit (1); }

#define q_ins(hd,tl,p,next)			\
  do {						\
    (p)->next = NULL;				\
    if ((hd) == NULL) {				\
      (hd) = (p);				\
    }						\
    else {					\
      (tl)->next = (p);				\
    }						\
    (tl) = (p);					\
  } while (0)


#define q_del(hd, tl, t, next)			\
  do {						\
    if (hd) {					\
      (t) = (hd);				\
      (hd) = (hd)->next;			\
    }						\
    else					\
      (t) = NULL;				\
  } while (0)

Process *Process::ready_hd = NULL;
Process *Process::ready_tl = NULL;
Process *Process::current_process = NULL;

void Process::Init ()
{
  Process::ready_hd = NULL;
  Process::ready_tl = NULL;
  Process::current_process = NULL;
}

void Process::Start ()
{
  if (Process::ready_hd) {
    process_begin ();
  }
}

/*
 *  Insert and delete from the ready queue
 */
void Process::_insert (Process *p)
{
  q_ins (Process::ready_hd, Process::ready_tl, p, _next);
}

Process *Process::_delete ()
{
  Process *p;
  q_del (Process::ready_hd, Process::ready_tl, p, _next);
  return p;
}


/*
 *  Select next ready process, saving away the current one
 */
unsigned int Process::Select (unsigned int cursp)
{
  if (cursp == 0) {
    if (Process::current_process) {
      /* terminated! */
      delete Process::current_process;
    }
    Process::current_process = NULL;
  }
  else {
    Process::current_process->_sp = cursp;
    /* put process into the ready queue */
	if (Process::current_process->_blocked == 0)
	{
		Process::_insert(Process::current_process);
	}
	else
	{
#ifdef DEBUG_THREADS
		Serial.println(F("process blocked"));
#endif
	}
  }
  Process::current_process = Process::_delete();
  if (Process::current_process) {
    return Process::current_process->_sp;
  }
  else {
    return 0;
  }
}

/* Hook from C to access this function */
extern "C" {
unsigned int process_select (unsigned int cursp)
{  return Process::Select (cursp); }
}

/* Stub function to invoke the process loop */
extern "C" void _launch_function (Process *p)
{
  while (1) {
    p->loop(); 
    cli();
    if (serialEventRun) serialEventRun();
    sei ();
  }
}

/*
 * Stack: save 32 regs, +2 for entry point +2 for ret address
 */
#define EXTRA_SPACE 37
#define EXTRA_PAD 4

Process::Process (int sz)
{
  unsigned long stk;
  int i;
  unsigned char *stkspace;

  //Serial.println(F("Creating proc!"));
  _blocked = 0;

  /* Create a new process */
  sz += EXTRA_SPACE + EXTRA_PAD;
  stkspace = (unsigned char *) malloc (sz);

  if (stkspace == NULL) {
    Serial.println ("FAILED!");
  }

  /* Create the "standard" stack, including entry point */
  for (i=0; i < sz; i++) {
      stkspace[i] = 0;
  }

  sz -= EXTRA_PAD;

  stkspace[sz-1] = ( (unsigned int) process_terminated ) & 0xff;
  stkspace[sz-2] = ( (unsigned int) process_terminated ) >> 8;
  stkspace[sz-3] = ( (unsigned int) _launch_function ) & 0xff;
  stkspace[sz-4] = ( (unsigned int) _launch_function ) >> 8;

  /* argument to the _launch_function is the process pointer */
  stkspace[sz-11] = ( (unsigned int) this) >> 8;
  stkspace[sz-12] = ( (unsigned int) this) & 0xff;

  /* SREG */
  stkspace[sz-EXTRA_SPACE] = SREG;

  stk = (unsigned int)stkspace + sz - EXTRA_SPACE - 1;
  _sp = stk;
  _stk = stkspace;
  
  /* Insert into ready queue */
  Process::_insert (this);
}

Process::~Process ()
{
  free (_stk);
}


/*------------------------------------------------------------------------
 *
 * Locks
 *
 *------------------------------------------------------------------------
 */
Lock::Lock()
{
  _hd = _tl = NULL;
  _lock = 0;
}

void Lock::Acquire()
{
  cli();

#ifdef DEBUG_THREADS
  Serial.println(F("ATTEMPT ACQUIRE"));
#endif

  while (_lock) {
#ifdef DEBUG_THREADS
	  Serial.println(F("FAILED ACQUIRE"));
#endif
    Process::current_process->_mkblocked ();
    _insert (Process::current_process);
    process_yield ();
    cli();
  }
#ifdef DEBUG_THREADS
  Serial.println(F("ACQUIRE"));
#endif

  _lock = 1;
  sei();
}

int Lock::_release ()
{
  Process *p;

  _lock = 0;

#ifdef DEBUG_THREADS
  Serial.println(F("RELEASE"));
#endif

  p = _delete ();
  if (p) {
    p->_clrblocked ();
    Process::_insert (p);
    return 1;
  }
  return 0;
}

void Lock::Release()
{
  cli();
  if (_release ()) {
    process_yield ();
  }
  else {
    sei ();
  }
}    

/*
 * insert and delete from the queue of blocked processes on the lock
 */
void Lock::_insert (Process *p)
{
  q_ins (_hd, _tl, p, _next);
}

Process *Lock::_delete ()
{
  Process *p;
  q_del (_hd, _tl, p, _next);
  return p;
}



/*------------------------------------------------------------------------
 *
 * Condition variables
 *
 *------------------------------------------------------------------------
 */
/* Constructor for a condition variable.
 * You should initialize any data structures you will use
 * for the condition variables here.  (Hint:  It would be wise to
 * have some sort of queue to keep track of blocked processes) */
Cond::Cond (Lock *l)
{
  _hd = _tl = NULL; /* Head and tail of waiting queue empty */
  _l = l;  /* Lock associated with this condition variable */
}

/* Cause a thread to block. */
void Cond::wait ()
{
  cli();  /* disable interrupts */
  assert (Process::current_process);
  Process::current_process->_mkblocked ();
  _insert (Process::current_process);
  _l->_release ();
  process_yield (); /*Run another thread.  Interrupts are automatically re-enabled here*/
}

int Cond::waiting ()
{
  return (_hd != NULL) ? 1 : 0;
}

void Cond::signal ()
{
  Process *p;
  
  cli();
  p = _delete ();
  if (p) {
    p->_clrblocked ();
    Process::_insert (p);
    sei ();
    process_yield ();
  }
  else {
    if (_l->_release ()) {
      process_yield ();
    }
    else {
      sei ();
    }
  }
}

/*
 * insert and delete from the queue of blocked processes on the lock
 */
void Cond::_insert (Process *p)
{
  q_ins (_hd, _tl, p, _next);
}

Process *Cond::_delete ()
{
  Process *p;
  q_del (_hd, _tl, p, _next);
  return p;
}
