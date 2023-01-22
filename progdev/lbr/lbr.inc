; Version macro

version	macro	type
	if1
.vers	macro	v,t
	ifidn	<&t>,<id>
	ident	'V&v'
	else
	db	'Version &v'
	endif
	endm
	endif
	.vers	<2.07>,&type	; change version here
	endm

; ASCII equates

TAB	equ	09h
FF	equ	0Ch
CR	equ	0Dh
LF	equ	0Ah
ESC	equ	1Bh
EOF	equ	1Ah

; Switch bit masks

; 1st byte - operation modes

S1.LI	equ	0		; full listing
S1.BR	equ	1		; brief listing (module names only)
S1.CR	equ	2		; cross-reference listing
S1.UN	equ	3		; undefined symbol listing
S1.AP	equ	4		; append mode
S1.EX	equ	5		; extract mode
S1.DE	equ	6		; delete mode
S1.IX	equ	7		; create index (IRL)

; 2nd byte - file flags

S2.II	equ	0		; IRL input file, else REL
S2.IO	equ	1		; IRL output file
S2.EQ	equ	7		; set if '=' seen in command line

SW.LST	equ	(1 SHL S1.LI) OR (1 SHL S1.BR) OR (1 SHL S1.CR) OR (1 SHL S1.UN)

; Symbol types

MODTYP	equ	0 SHL 4		; module name
GBLTYP	equ	1 SHL 4		; public symbol
EXTTYP	equ	2 SHL 4		; external reference
COMTYP	equ	3 SHL 4		; COMMON name
LIBTYP	equ	4 SHL 4		; library request
IDNTYP	equ	5 SHL 4		; version identifier

TYPMSK	equ	11110000b	; symbol type mask
LENMSK	equ	00001111b	; symbol length mask
SEGMSK	equ	11000000b	; symbol segment mask

