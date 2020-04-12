; Login Control Block structure, one per logged-in terminal

L.ST	equ	0		; status (1 byte)
L.NAME	equ	L.ST + 1	; user name (9 characters)
L.USR	equ	L.NAME + 9	; user number (1 byte)
L.GRP	equ	L.USR + 1	; group number (1 byte)
L.CDIR	equ	L.GRP + 1	; current dir name (9 bytes)

; LCB size

LCBSZ	equ	L.CDIR + 9

; Status bit numbers

LS.PRV	equ	0		; privileged

