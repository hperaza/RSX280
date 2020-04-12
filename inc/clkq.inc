; Clock queue packets

C.LNK	equ	0		; address of next element in list (2 bytes)
C.TYPE	equ	C.LNK + 2	; packet type (1 byte)
C.TCB	equ	C.TYPE + 1	; TCB of associated task (2 bytes)
C.EFN	equ	C.TCB + 2	; event flag number (1 byte)
C.AST	equ	C.EFN + 1	; AST routine address (2 bytes)
C.TIME	equ	C.AST + 2	; absolute system time in ticks (4 bytes)
C.RSI	equ	C.TIME + 4	; reschedule interval in ticks (4 bytes)

; Packet size

CLQSZ	equ	C.RSI + 4

; Packet types

CT.MKT	equ	1		; mark time
CT.RUN	equ	2		; run at specified time

