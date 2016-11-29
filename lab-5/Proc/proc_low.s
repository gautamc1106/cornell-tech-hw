	.file	"proc_low.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.comm	_orig_sp_hi,1,1
	.comm	_orig_sp_lo,1,1
	.text
.global	process_begin
	.type	process_begin, @function
process_begin:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
/* #APP */
 ;  5 "proc_low.c" 1
	cli 
	in r24,__SP_L__ 
	sts _orig_sp_lo, r24
	in r25,__SP_H__ 
	sts _orig_sp_hi, r25
	ldi r24, lo8(0)
	ldi r25, hi8(0)
	rjmp .dead_proc_entry
	
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	pop r29
	pop r28
	ret
	.size	process_begin, .-process_begin
.global	process_terminated
	.type	process_terminated, @function
process_terminated:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
/* #APP */
 ;  19 "proc_low.c" 1
	cli
	lds r25, _orig_sp_hi
	out __SP_H__, r25
	lds r24, _orig_sp_lo
	out __SP_L__, r24
	ldi r24, lo8(0)
	ldi r25, hi8(0)
	rjmp .dead_proc_entry
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	pop r29
	pop r28
	ret
	.size	process_terminated, .-process_terminated
.global	process_yield
	.type	process_yield, @function
process_yield:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
/* #APP */
 ;  35 "proc_low.c" 1
	cli
	
 ;  0 "" 2
 ;  36 "proc_low.c" 1
	rjmp process_timer_interrupt
	
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	pop r29
	pop r28
	ret
	.size	process_yield, .-process_yield
.global	process_timer_interrupt
	.type	process_timer_interrupt, @function
process_timer_interrupt:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
/* #APP */
 ;  41 "proc_low.c" 1
	push r31
	push r30
	push r29
	push r28
	push r27
	push r26
	push r25
	push r24
	push r23
	push r22
	push r21
	push r20
	push r19
	push r18
	push r17
	push r16
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push r7
	push r6
	push r5
	push r4
	push r3
	push r2
	push r1
	push r0
	in r24, __SREG__
	push r24
	in r24, __SP_L__
	in r25, __SP_H__
	.dead_proc_entry:
	rcall process_select
	eor r18,r18
	or r18,r24
	or r18,r25
	brne .label_resume
	lds r25, _orig_sp_hi
	out __SP_H__, r25
	lds r24, _orig_sp_lo
	out __SP_L__, r24
	ret
	.label_resume:
	out __SP_L__, r24
	out __SP_H__, r25
	pop r0
	out  __SREG__, r0
	pop r0
	pop r1
	pop r2
	pop r3
	pop r4
	pop r5
	pop r6
	pop r7
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
	pop r16
	pop r17
	pop r18
	pop r19
	pop r20
	pop r21
	pop r22
	pop r23
	pop r24
	pop r25
	pop r26
	pop r27
	pop r28
	pop r29
	pop r30
	pop r31
	reti
	
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	pop r29
	pop r28
	ret
	.size	process_timer_interrupt, .-process_timer_interrupt
.global	yield
	.type	yield, @function
yield:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	rcall process_yield
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	yield, .-yield
	.ident	"GCC: (GNU) 4.9.2"
.global __do_clear_bss
