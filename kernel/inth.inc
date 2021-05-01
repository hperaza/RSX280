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

	include	Z280.INC

CR	equ	0Dh
LF	equ	0Ah

FALSE	equ	0
TRUE	equ	NOT FALSE

	public	TMRINT,$UGETB,$UPUTB,$UGETW,$UPUTW,$MCOPY
	public	$SBANK,$UBANK,IHSP,IHSTK,UPTIME,TICSEC,TICCNT
	public	TFLAG

	extrn	CHKWD,DECCT,DOCLKQ,$CTCB,LNKFQB

;--------------------------------------------------------------------

; Timer interrupt handler.

TMRINT:	push	af
	push	bc
	push	de
	push	hl
	ld	c,IOP
	ldctl	hl,(c)
	ex	de,hl		; save I/O page
	ld	l,CTP
	ldctl	(c),hl		; select timer I/O page
	ld	a,11100000b	; reset end-of-count and count-overrun bits
	out	(CS0),a		;  (acknowledge interrupt)
	ex	de,hl
	ldctl	(c),hl		; restore I/O page
	call	INCTKC		; increment absolute time counter
	call	INCUPT		; increment uptime counter
	ld	hl,TFLAG
	set	0,(hl)
	ld	a,(SYSLVL)	; check system level
	dec	a		; user state?
	jr	z,user		; branch if yes

	; interrupt from system or privileged task in exclusive access mode

	push	ix
	push	iy
	ld	hl,0
	inc	a
	jp	m,sys

	; interrupt from privileged task in exclusive access mode

	add	hl,sp
	ld	sp,SYSSTK	; reset system stack

sys:	; interrupt from system mode, kernel is mapped-in,
	; just handle device timeouts and exit

	push	hl
	call	CHKWD
	call	DECCT
	pop	hl

	; Because SYSLVL=-1 we cannot call DOCLKQ, as we may have interrupted
	; a fork process or system function that modifies the pool state
	; and/or the task list. Instead, we set a flag (see TFLAG above)
	; and execute the action in SYSXIT.

	ld	a,h
	or	l
	jr	z,sret

	ld	sp,hl

sret:	pop	iy
	pop	ix
	pop	hl
	pop	de
	pop	bc
	pop	af
	inc	sp
	inc	sp
	retil

user:	; interrupt from user mode, dispatch next task

	ex	af,af'		; AF, BC, DE and HL already saved
	push	af
	ex	af,af'
	exx
	push	bc
	push	de
	push	hl
	exx
	push	ix
	push	iy
	ld	sp,SYSSTK	; reset system stack
	ldctl	hl,usp		; get task's SP into HL
	ld	ix,($CTCB)	; get pointer to current TCB
	ldw	(ix+T.SP),hl	; save SP in task's TCB
	ld	a,-1
	ld	(SYSLVL),a	; we're in global system state
	call	CHKWD		; handle device timeouts
	call	DECCT		; decrement interval counters
	ei			; enable interrupts
	jp	SYSXIT		; return via SYSXIT to dispatch next task

;-----------------------------------------------------------------------

; Increment the uptime counter. Called from the timer interrupt routine
; with interrupts disabled.

INCUPT:	ld	hl,UPTIME
	inc	(hl)		; 1-byte ticks/sec
	ld	a,(TICSEC)
	cp	(hl)
	ret	nz
	ld	(hl),0

	inc	hl
	inc	(hl)		; 1-byte seconds 00..59
	ld	a,60
	cp	(hl)
	ret	nz
	ld	(hl),0

	inc	hl
	inc	(hl)		; 1-byte minutes 00..59
	cp	(hl)		; A still = 60
	ret	nz
	ld	(hl),0

	inc	hl
	inc	(hl)		; 1-byte hours 00..23
	ld	a,24
	cp	(hl)
	ret	nz
	ld	(hl),0

	inc	hl
	inc	(hl)		; 2-byte days 00000..65535
	ret	nz
	inc	hl
	inc	(hl)
	ret

;-----------------------------------------------------------------------

; Increment the absolute time tick counter. Called from the timer interrupt
; routine with interrupts disabled.

INCTKC:	ld	hl,TICCNT
	inc	(hl)
	ret	nz
	inc	hl
	inc	(hl)
	ret	nz
	inc	hl
	inc	(hl)
	ret	nz
	inc	hl
	inc	(hl)
	ret

;-----------------------------------------------------------------------

; Copy block of data from one bank to another
; HL = src address, HL' = src bank
; DE = dst address, DE' = dst bank
; BC = count
;
; Notes:
; - if using DMA, be sure to use a channel that is not in use used by
;   any device driver!

$MCOPY:	ld	a,b		; check count
	or	c		; anything to move?
	ret	z		; return if not

	push	hl		; push src address
	push	de		; push dst address
	push	bc		; push count

	push	hl		; push src address (2)
	push	bc		; push count (2)
	iopage	DMAP
	ld	c,TDR2
	ld	hl,0
	outw	(c),hl		; disable DMA while we mess around

	pop	hl		; pop count (2)
	ld	c,DCR2
	outw	(c),hl		; set DMA count

	pop	hl		; pop src address (2)
	ld	c,SAL2
	ld	a,h
	ld	b,a		; save high nibble
	or	11110000b	; set DMA bits
	ld	h,a
	outw	(c),hl		; set low half of src address
	exx
	push	hl		; get src bank address
	exx
	pop	hl
	add	hl,hl		; shift to bits 4..11
	add	hl,hl
	add	hl,hl
	add	hl,hl
	ld	a,b		; get high nibble of address
	or	00001111b	; set DMA bits
	add	l		; add to bank address
	ld	l,a		;  (can't use 'add hl,a' here because
	ld	a,h		;   it sign-extends A)
	adc	0
	ld	h,a
	inc	c		; SAH2
	outw	(c),hl		; set high half of src address

	ex	de,hl		; get dst address into HL
	ld	c,DAL2
	ld	a,h
	ld	b,a		; save high nibble
	or	11110000b	; set DMA bits
	ld	h,a
	outw	(c),hl		; set low half of dst address
	exx
	push	de		; get dst bank address
	exx
	pop	hl
	add	hl,hl		; shift to bits 4..11
	add	hl,hl
	add	hl,hl
	add	hl,hl
	ld	a,b		; get high nibble of address
	or	00001111b	; set DMA bits
	ld	e,a
	ld	d,0
	add	hl,de		; add to bank address
	inc	c		; DAH2
	outw	(c),hl		; set high half of dst address

	ld	c,DMCR
	ld	hl,0F0E0h
	outw	(c),hl

	ld	c,TDR2
	ld	hl,8100h	; continuous, byte size, inc src & dst
	outw	(c),hl

	pop	bc		; pop count
	pop	de		; pop dst address
	pop	hl		; pop src address
	add	hl,bc		; adjust src
	ex	de,hl		;  and dst addresses
	add	hl,bc		;   to point past moved block
	ex	de,hl
	ret

; Get byte from bank DE, address HL into A.

$UGETB:	push	hl
	push	de
	push	bc
	ld	b,16
	call	USRMAP
	pop	bc
	pop	de
	pop	hl
	ldup	a,(hl)
	ret

; Write byte in A to bank DE, address HL.

$UPUTB:	push	af
	push	hl
	push	de
	push	bc
	ld	b,16
	call	USRMAP
	pop	bc
	pop	de
	pop	hl
	pop	af
	ldup	(hl),a
	ret

; Read word from bank DE, address HL into BC. Returns with HL += 2

$UGETW:	push	hl
	push	de
	ld	b,16
	call	USRMAP
	pop	de
	pop	hl
	ldup	a,(hl)
	ld	c,a
	inc	hl
	ldup	a,(hl)
	ld	b,a
	inc	hl
	ret

; Write word in BC to bank DE, address HL. Returns with HL += 2

$UPUTW:	push	hl
	push	de
	push	bc
	ld	b,16
	call	USRMAP
	pop	bc
	pop	de
	pop	hl
	ld	a,c
	ldup	(hl),a
	inc	hl
	ld	a,b
	ldup	(hl),a
	inc	hl
	ret

; ----- data areas

;;	DSEG

TICSEC:	ds	1		; ticks per second
TICCNT:	ds	4		; absolute time count in ticks
UPTIME:	ds	6		; uptime in BCD

TFLAG:	ds	1		; bit 0 = clock queue tick elapsed
				; bit 1 = round-robin interval elapsed
				; bit 2 = swap interval elapsed

$SBANK:	ds	2		; base of system bank
$UBANK:	ds	2		; base of current task's bank

	.even
	ds	64		; space for interrupt stack
IHSTK	equ	$
IHSP:	ds	2

