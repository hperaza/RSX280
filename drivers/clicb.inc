; CLI Control Block structure

CL.LNK	equ	0		; link to next CLICB (2 bytes)
CL.NAM	equ	CL.LNK + 2	; CLI task name (6 bytes)
CL.DPL	equ	CL.NAM + 6	; length of default prompt string (1 byte)
CL.CPL	equ	CL.DPL + 1	; length of ctrl-c prompt string (1 byte)
CL.DAT	equ	CL.CPL + 1	; start of data area (prompt strings)

; CLI Control Block size (without strings)

CLIBSZ	equ	CL.DAT

