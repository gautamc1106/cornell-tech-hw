/*************************************************************************
 *
 *  Copyright (c) 2013 Rajit Manohar
 *  All Rights Reserved
 *
 **************************************************************************
 */
#ifndef __PROC_INTR_H__
#define __PROC_INTR_H__

extern "C" {
  void process_terminated ();
  void process_begin ();
  void process_yield ();
  void process_blocked ();
  void process_timer_interrupt ();
}

#endif /* __PROC_INTR_H__ */
