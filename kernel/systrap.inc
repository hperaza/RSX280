;**********************************************************************;
;                                                                      ;
;   This file is part of RSX280, a multi-tasking OS for the Z280.      ;
;   Copyright (C) 1985-2020, Hector Peraza.                            ;
;                                                                      ;
;   This program is free software; you can redistribute it and/or      ;
;   modify it under the terms of the GNU General Public License as     ;
;   published by the Free Software Foundation; either version 2 of     ;
;   the License, or (at your option) any later version.                ;
;                                                                      ;
;   This program is distributed in the hope that it will be useful,    ;
;   but WITHOUT ANY WARRANTY; without even the implied warranty of     ;
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      ;
;   GNU General Public License for more details.                       ;
;                                                                      ;
;   You should have received a copy of the GNU General Public License  ;
;   along with this program; if not, write to the Free Software        ;
;   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.          ;
;                                                                      ;
;**********************************************************************;

	.Z280

	public	$ITBL,SYSENT,SYSXIT,SYSLVL,TRPFLG,DOFORK

	extrn	SYSFN,TKILL,$FORKQ,$FREE,DOCLKQ,DSPTCH,DOAST,$CPCB
	extrn	USRMAP,PRVMAP
;;	extrn	$SBANK

;-----------------------------------------------------------------------

; Z280 Interrupt/Trap Vector Table

$ITBL:	dw	0,0		; reserved
	dw	0,nmint		; NMI
	dw	0,iret2		; INTA
	dw	0,iret2		; INTB
	dw	0,iret2		; INTC
	dw	0,TMRINT	; CT0 (system timer)
	dw	0,iret2		; CT1
	dw	0,iret2		; reserved
	dw	0,iret2		; CT2
	dw	0,iret2		; DMA0
	dw	0,iret2		; DMA1
	dw	0,iret2		; DMA2
	dw	0,iret2		; DMA3
	dw	0,iret2		; UART Rx
	dw	0,iret2		; UART Tx
	dw	0,sstrp		; single-step trap
	dw	0,bptrp		; breakpoint-on-halt trap
	dw	0,divtrp	; division trap
	dw	0,iret2		; system stack overflow
	dw	0,acvtrp	; access violation trap
	dw	0,SYSENT	; system call trap
	dw	0,prvins	; privileged instruction trap
	dw	0,exti2		; EPU trap
	dw	0,exti2		; EPU trap
	dw	0,exti1		; EPU trap
	dw	0,exti1		; EPU trap
	dw	0,iret2		; reserved
	dw	0,iret2		; reserved
	; vector tables for INTA...INTC not used

iret2:	inc	sp
	inc	sp
iret1:	retil

bptrp:
sstrp:	push	0		; fake trap reason code
	ld	(TRPFLG),TK.SSB	; set abort code
	jp	SYSENT		; abort task via SYSENT

exti2:	inc	sp
	inc	sp
exti1:	ld	(TRPFLG),TK.EXT	; set abort code
	jp	SYSENT		; abort task via SYSENT

divtrp:	push	0		; fake trap reason code
	ld	(TRPFLG),TK.DIV	; set abort code
	jp	SYSENT		; abort task via SYSENT

prvins:	push	0		; fake trap reason code
	ld	(TRPFLG),TK.PRV	; set abort code
	jp	SYSENT		; abort task via SYSENT

acvtrp:	push	0		; fake trap reason code
	ld	(TRPFLG),TK.PROT; set abort code
	jp	SYSENT		; abort task via SYSENT

nmint:	inc	sp		; TODO: call the debugger
	inc	sp
	retil

; System trap entry:
; - all registers are saved into the task's context block.
; - system stack pointer is reset.
; - system level variable gets decremented to prevent dispatch until
;   we return.
; - interrupts are reenabled.

SYSENT:	push	af		; system stack points to current task's
	push	bc		;  context register bank 
	push	de
	push	hl
	ex	af,af'
	push	af		; saved all registers
	ex	af,af'
	exx
	push	bc
	push	de
	push	hl
	exx
	push	ix
	push	iy
	ld	sp,SYSSTK	; reset system stack
	ldctl	hl,usp
	ld	ix,($CTCB)
	ldw	(ix+T.SP),hl	; save user SP in task's TCB
	ld	a,-1
	ld	(SYSLVL),a	; we are now in global system state
	ei			; enable interrupts

	ld	a,(TRPFLG)	; check trap flag
	or	a		; abnormal task exit?
	jr	z,normal	; proceed normally if not

	ld	e,a		; E = reason code
	xor	a
	ld	(TRPFLG),a
	ld	bc,($CTCB)	; else abort the current task
	call	TKILL
	jr	SYSXIT		; TKILL never returns, anyway

normal:	call	SYSFN		; handle system call

SYSXIT:	call	DOFORK		; process all pending fork blocks
	jr	nc,SYSXIT

	ld	hl,TFLAG
	bit	0,(hl)
	jr	z,go1
	res	0,(hl)
	call	DOCLKQ		; dispatch the clock queue

go1:	call	DSPTCH		; prepare next task to dispatch
	call	DOAST		; if there is a pending AST for this task,
				;  execute it now
	di
	ld	a,1
	ld	(SYSLVL),a	; leaving system, entering user state

	ld	ix,($CPCB)
	ld	e,(ix+P.BASE)	; get task bank base
	ld	d,0
	ld	b,(ix+P.SIZE)	; get number of pages
	ld	ix,($CTCB)
	bit	TS.SUP,(ix+T.ST); check if task is in supervisor mode
	ld	hl,USRMAP	; normal-mode memory map
	jr	z,go2
	ld	hl,PRVMAP	; supervisor-mode memory map
go2:	call	(hl)		; setup user mode map
	ldw	hl,(ix+T.SP)
	ldctl	usp,hl		; set user stack pointer
	ldw	ix,(ix+T.CTX)
	lda	hl,(ix+TX.REGS)
	ld	sp,hl		; prepare to restore registers
	pop	iy
	pop	ix
	exx
	pop	hl
	pop	de
	pop	bc
	exx
	ex	af,af'
	pop	af
	ex	af,af'
	pop	hl
	pop	de
	pop	bc
	pop	af
	inc	sp		; skip reason/sysfn code
	inc	sp
	retil

;-----------------------------------------------------------------------

; Fork block execution. Removes first queued fork block from the list
; and calls the corresponding routine. Returns CY if the list is empty.

DOFORK:	di
	ld	hl,$FORKQ
	ldw	de,(hl)		; get first fork block in list into DE
	ld	a,d
	or	e
	scf
	jr	z,frk1		; return if queue empty.
	ex	de,hl		; get fork block address into HL
	ld	a,(hl)
	ld	(de),a		; unlink the fork block
	inc	hl
	inc	de
	ld	a,(hl)
	ld	(de),a
	ei			; interrupts can happen from now on
	inc	hl
	ld	a,(hl)		; get bank
	inc	hl
	ldw	de,(hl)		; get routine address into DE
	inc	hl
	inc	hl
	ldw	bc,(hl)		; get context
	push	bc
	pop	iy		;  into IY
	ex	de,hl
	call	(hl)		; call the routine
	xor	a
frk1:	ei
	ret

;-----------------------------------------------------------------------

;	dseg

SYSLVL:	ds	1		; system level:
				;  1 = user task state
				;  0 = priv task accessing kernel data
				; -1 = system state
TRPFLG:	ds	1		; trap flag

