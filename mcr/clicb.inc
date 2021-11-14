; CLI Control Block structure

CL.LNK	equ	0		; link to next CLICB (2 bytes)
CL.NAM	equ	CL.LNK + 2	; CLI task name (6 bytes)
CL.DPL	equ	CL.NAM + 6	; length of default prompt string (1 byte)
CL.CPL	equ	CL.DPL + 1	; length of ctrl-c prompt string (1 byte)
CL.DAT	equ	CL.CPL + 1	; start of data area (prompt strings)

; CLI Control Block size (without strings)

CLIBSZ	equ	CL.DAT

; RSX-11M style macro to get CLI information

GCII$	macro	buf,len,cli,dev,unit
	local	lbuf
	ifnb	<buf>
	dw	buf
	db	len
	else
	dw	lbuf
	db	32
	endif
	ifnb	<cli>
	db	cli
	else
	db	0,0,0,0,0,0
	endif
	ifnb	<dev>
	db	dev,unit
	else
	db	'TI',0
	endif
	ifb	<buf>
lbuf:	ds	32
	endif
	endm

