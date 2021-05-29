DEBUG	equ	1	; enable .DEBUG command

MAXIND	equ	3	; max number of nested command files
MAXGSB	equ	10	; max .GOSUB depth
MAXXQT	equ	10	; max number of concurrent .XQT (must be <= 16)
MAXFIL	equ	4	; max number of open files

SPWFLG	equ	11	; flag for MCR spawn
XQTFLG	equ	16	; base flag number for .XQT commands

LUNIND	equ	1	; base LUN for command file I/O
LUNDEV	equ	4	; misc LUN for <SYDISK>, <SYSDEV>, etc.
LUNFIL	equ	6	; base LUN for user file I/O

; Variable types

V.STR	equ	00h	; string
V.LOG	equ	20h	; logical
V.NUM	equ	40h	; numeric
V.BLK	equ	80h	; block marker (note sign bit set)
V.FIL	equ	0C0h	; file frame marker (sign bit also set)
V.LMSK	equ	1Fh	; mask for length field
V.TMSK	equ	0E0h	; mask for variable type bits

; Enable/disable option bit numbers

B0.ATT	equ	0	; attach
B0.CTZ	equ	1	; control-Z
B0.DAT	equ	2	; data
B0.DEC	equ	3	; decimal
B0.DEL	equ	4	; delete
B0.DET	equ	5	; detach
B0.DPY	equ	6	; display
B0.ESC	equ	7	; escape

B1.GBL	equ	0	; global
B1.LWC	equ	1	; lowercase
B1.MCR	equ	2	; mcr
B1.OVF	equ	3	; overflow
B1.QIE	equ	4	; quiet
B1.SUB	equ	5	; substitution
B1.TMO	equ	6	; timeout
B1.TRC	equ	7	; trace

B2.TRU	equ	0	; truncate
B2.LOC	equ	1	; shared locals

; File buffer structure fields

FB.BPT	equ	0		; back pointer into channel table (2 bytes)
FB.LUN	equ	FB.BPT + 2	; LUN (1 byte)
FB.MOD	equ	FB.LUN + 1	; file mode (1 byte)
FB.BLK	equ	FB.MOD + 1	; current block number (2 bytes)
FB.CNT	equ	FB.BLK + 2	; chars in buffer (2 bytes)
FB.CUR	equ	FB.CNT + 2	; index of current char in buffer (2 bytes)
FB.BUF	equ	FB.CUR + 2	; buffer (512 bytes)

FBFSZ	equ	FB.BUF + 512	; structure size

; Macro for building command tables

CMD	MACRO	name,addr1,addr2
	db	name,0
	dw	addr1
	IFNB	<addr2>
	dw	addr2
	ENDIF
	ENDM

