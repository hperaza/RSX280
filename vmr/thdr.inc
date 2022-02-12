; Task File Header offsets

TH.HDR	equ	00h		; task header magic string (6 bytes)
TH.VER	equ	08h		; task image file version (2 bytes)
TH.CPU	equ	0Ah		; CPU type (1 byte)
TH.NAME	equ	10h		; default task name (6 bytes)
TH.VID	equ	18h		; task version identification (6 bytes)
TH.PAR	equ	20h		; partition name (6 bytes)
TH.STRT	equ	28h		; start address (2 bytes)
TH.END	equ	2Ah		; end address (2 bytes)
TH.EPT	equ	2Ch		; entry point (2 bytes)
TH.INC	equ	2Eh		; extension/increment size (2 bytes)
TH.PRI	equ	30h		; task priority (1 byte)
TH.ATTR	equ	31h		; task attributes (1 byte)
TH.UIC	equ	38h		; task default UIC (2 bytes)
TH.LUNT	equ	40h		; LUN table (16*4 = 64 bytes)

; Task File Header size

THSZ	equ	256

; Attribute bit numbers, must match the ones defined in TCB.INC

TA.PRV	equ	0		; task is privileged
TA.CLI	equ	5		; task is a CLI
TA.ACP	equ	6		; task is an ACP
TA.CKD	equ	7		; checkpointing disabled

