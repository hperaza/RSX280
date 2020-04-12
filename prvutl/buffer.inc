; Buffer equates

BLKSZ	equ	512		; block size

; Buffer offsets

B.LNK	equ	0		; link to next buffer in list (2 bytes)
B.ST	equ	B.LNK + 2	; status (1 byte)
B.UCB	equ	B.ST + 1	; UCB addr of physical device name (2 bytes)
B.BLKNO	equ	B.UCB + 2	; block number (3 bytes)
B.ACCS	equ	B.BLKNO + 3	; access count (1 byte)
B.DATA	equ	B.ACCS + 1	; start of data field (1 block = 512 bytes)

BUFSZ	equ	B.DATA + BLKSZ	; buffer size

; Status bits

BS.VAL	equ	0		; valid
BS.MOD	equ	1		; modified

