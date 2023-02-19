FALSE	equ	0
TRUE	equ	NOT FALSE

; ASCII equates

CR	equ	0Dh
LF	equ	0Ah
TAB	equ	09h

; LUN assignments

LUNDBG	equ	1
LUNCML	equ	2
LUNFIL	equ	3
LUNTI	equ	5

; Event flag numbers

TTEFN	equ	11
IOEFN	equ	12
TMOEFN	equ	13
DBGEFN	equ	14

; Debugger settings

MAXBP	equ	8		; max number of breakpoints

KCALL	macro	addr
	call	$SWSTK
	dw	addr
	endm

