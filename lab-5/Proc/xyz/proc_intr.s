	.file "proc_intr.s"
__SP_L__ = 0x3d
__SP_H__ = 0x3e
__SREG__ = 0x3f
	.text
	.global _process_terminated
	.type _process_terminated, @function
	.global _process_begin
	.type _process_begin, @function
	.global _process_yield
	.type _process_yield, @function
	.global _process_timer_interrupt
	.type _process_timer_interrupt, @function

_process_terminated:
	;; clear interrupts; set arg to 0, and go to new process selection
	cli
	ldi r24, lo8(0)
	ldi r25, hi8(0)
	rjmp .dead_proc_entry
_process_begin:
	cli
	in r24,__SP_L__
	in r25,__SP_H__
	sts _orig_sp_hi, r25
	sts _orig_sp_lo, r24
	ldi r24, lo8(0)
	ldi r25, hi8(0)
	rjmp .dead_proc_entry
_process_yield:
	cli
_process_timer_interrupt:
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
	;;  call process select with the current stack pointer as an argument
	in r24, __SP_L__
	in r25, __SP_H__
.dead_proc_entry:
	rcall __process_select
	eor r18,r18
	or r18,r24
	or r18,r25
	brne .label_resume
	;; nothing left---restore original stack
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

	.section .bss
	.type _orig_sp_lo, @object
	.size _orig_sp_lo, 1
_orig_sp_lo:
	.skip 1,0

	.type _orig_sp_hi, @object
	.size _orig_sp_hi, 1
_orig_sp_hi:
	.skip 1,0
