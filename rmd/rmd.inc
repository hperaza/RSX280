;----------------------------------------------------------------------;
;                                                                      ;
;       RMD - Macros, definitions, etc.                                ;
;                                                                      ;
;----------------------------------------------------------------------;

false	equ	0
true	equ	not false

CR	equ	0Dh
LF	equ	0Ah
ESC	equ	1Bh

hsize	equ	80
vsize	equ	24

VGTOXY	macro	x,y
	ld	hl,x*256+y
	call	vxy
	endm

GOTOXY	macro	x,y
	ld	hl,x*256+y
	call	putcur
	endm

KCALL	macro	addr
	call	$SWSTK
	dw	addr
	endm

