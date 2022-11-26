; I/O packet structure.
; The first three fields must correspond to the first three QCB fields.

I.LNK	equ	0		; (QU.LNK) link to next packet (2 bytes)
I.PRI	equ	I.LNK + 2	; (QU.PRI) priority (1 byte)
I.SIZE	equ	I.PRI + 1	; (QU.SIZ) packet size (2 bytes)
I.TCB	equ	I.SIZE + 2	; TCB address of requesting task (2 bytes)
I.UCB	equ	I.TCB + 2	; UCB address of target device (2 bytes)
I.QDB	equ	I.UCB + 2	; QBD (QDBSZ bytes)

IOPKSZ	equ	I.QDB + QDBSZ	; total I/O packet size
IDAT	equ	IOPKSZ - I.TCB	; size of data field (TCB addr + UCB addr + QDB)

