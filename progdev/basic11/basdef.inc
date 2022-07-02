; Define the following global to include full error message strings.
; Undefine it to generate short (three letters) error messages.

LMSG	equ	1

; Define the following global to include support for the CALL command.

;CALLS	equ	1		; not done, anyway

; External references to work data area

	extrn	SYMBOL,LINTAB,DEFTAB,LIMIT,PDL,STK,PDSIZE,ARRAYS
	extrn	HIFREE,LOFREE,HIFSAV,CODE,LINE,VARSAV,SS1SAV,SS2SAV
	extrn	CPSAVE,GSBCTR,COLUMN,CLMNTT,FAC1,FAC2,CLCMOD
	extrn	BCSAVE,DESAVE,HLSAVE,IXSAVE,IYSAVE,T1,T2,T3,T4,T5
	extrn	RND1,RND2,RNDCT,LOSTR,HISTR,COMN,HILINO,WIDTH
	extrn	IDEV,ODEV,FILSIZ,MODE,RECSIZ,SEQFIL,BUFCHN,EDITLN
	extrn	CHNFLG,SYSBUF,DATPTR,CLKPTR,CCFLG,COLTAB,FNBSZ
	extrn	SS1MAX,SS2MAX,PROGNAM,BASDAT

; File I/O buffer definitions

BUFSZ	equ	512		; file I/O buffer size

; Indexes into buffer descriptor

BUFAD	equ	0		; address of I/O buffer
BUF2	equ	BUFAD + 2	; address of second buffer for DOUBLEBUF files
BLKNO	equ	BUF2 + 2	; current block I/O number
BUFEND	equ	BLKNO + 2	; address of end byte in I/O buffer
BUFPTR	equ	BUFEND + 2	; current byte pointer
BFDB	equ	BUFPTR + 2	; address of OS-dependent file descriptor block

BDSZ	equ	BFDB + 2	; size of buffer descriptor

; LUN assignment

LUNTI	equ	1		; terminal
LUNSY	equ	2		; and above for file operations

