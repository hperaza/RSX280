; Partition Control Block structure

P.LNK	equ	0		; link to next PCB in list (2 bytes)
P.MAIN	equ	P.LNK + 2	; pointer to main partition (2 bytes)
P.SUB	equ	P.MAIN + 2	; pointer to subpartition (2 bytes)
P.SIZE	equ	P.SUB + 2	; size in pages (1=Z180 or 2=Z280 bytes)
P.BASE	equ	P.SIZE + 2	; start page address (1=Z180 or 2=Z280 bytes)
P.ATTR	equ	P.BASE + 2	; attributes (1 byte)
P.STAT	equ	P.ATTR + 1	; status flags (1 byte)
P.NAME	equ	P.STAT + 1	; partition name (6 chars)
P.WAIT	equ	P.NAME + 6	; pointer to wait queue list head (2 bytes)
P.TCB	equ	P.WAIT + 2	; ptr to TCB of owner (2 bytes)
P.PROT	equ	P.TCB + 2	; protection word for common partition (2 bytes)

PCBSZ	equ	P.PROT + 2

; Checkpoint PCB

P.LNK	equ	0		; link to next PCB in list (2 bytes)
P.MAIN	equ	P.LNK + 2	; pointer to main PCB (2 bytes)
P.SUB	equ	P.MAIN + 2	; pointer to next checkpoint PCB (2 bytes)
P.SIZE	equ	P.SUB + 2	; size of checkpointed task in disk blks (2 bytes)
P.UCB	equ	P.SIZE + 2	; UCB of checkpoint file device (2 bytes)
P.LBN	equ	P.UCB + 2	; starting LBN (4 bytes)
P.REL	equ	P.LBN + 4	; relative block# within checkpoint file (2 bytes)

CPCBSZ	equ	P.REL + 2

; Partition attributes

PA.SUB	equ	0		; subpartition
PA.SYS	equ	1		; system-controlled
PA.CHK	equ	2		; not checkpointable
PA.FXD	equ	3		; fixed
PA.NST	equ	4		; not shuffable
PA.COM	equ	5		; lib or common block
PA.DEL	equ	6		; delete when not attached

; Partition status bits

PS.OUT	equ	0		; partition is out of memory
PS.CKP	equ	1		; checkpoint in progress
PS.CKR	equ	2		; checkpoint requested
PS.BSY	equ	3		; partition busy
PS.DRV	equ	4		; driver loaded in partition
PS.CFD	equ	5		; checkpoint file discontinued
