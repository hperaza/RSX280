; Mounted Volume Control Block (VCB) fields

M.LNK	equ	0		; link to next item in list (2 bytes)
M.UCB	equ	M.LNK + 2	; pointer to UCB of physical device (2 bytes)
M.LABEL	equ	M.UCB + 2	; volume label (16 bytes)
M.BLKS	equ	M.LABEL + 16	; number of blocks (3 bytes)
M.CLF	equ	M.BLKS + 3	; cluster factor (1 byte)
M.DFPR	equ	M.CLF + 1	; default file protection bits (2 bytes)
M.FCBL	equ	M.DFPR + 2	; FCB list head (2 bytes)
M.IXLBN	equ	M.FCBL + 2	; LBN of index file (3 bytes)
M.BMLBN	equ	M.IXLBN + 3	; LBN of bitmap file (3 bytes)
M.IBLBN	equ	M.BMLBN + 3	; LBN of index bitmap file (3 bytes)
M.IXSZ	equ	M.IBLBN + 3	; size of index file in blocks (2 bytes)
M.FREE	equ	M.IXSZ + 2	; cached free cluster count (3 bytes)
M.MFCB	equ	M.FREE + 3	; master directory FCB address (2 bytes)
M.ACP	equ	M.MFCB + 2	; TCB addr of ACP task managing this unit (2 bytes)

; Mount table entry size

MNTSZ	equ	M.ACP + 2

