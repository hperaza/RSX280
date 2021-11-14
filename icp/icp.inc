DEBUG	equ	0	; enable .DEBUG command

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

V.STR	equ	00000000b ; string
V.LOG	equ	00100000b ; logical
V.NUM	equ	01000000b ; numeric
V.BLK	equ	10000000b ; block marker (note sign bit set)
V.FIL	equ	11000000b ; file frame marker (sign bit also set)
V.ERR	equ	10100000b ; .ONERR frame marker (sign bit also set)
V.LMSK	equ	00011111b ; mask for length field
V.TMSK	equ	11100000b ; mask for variable type bits

; .ENABLE/.DISABLE option bit numbers

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

; <ERRNUM> error codes

; Class 0 - informational, can't be trapped

E_IAT	equ	1	; Invalid answer or terminator
E_VNR	equ	2	; Value not in range
E_SLR	equ	3	; String length not in range
;E_INV	equ	4	; Invalid keyword

; Class 1 - handled with .ONERR

E_BRS	equ	5	; Bad range or default specification
E_FAO	equ	6	; File already open
E_FNO	equ	7	; File not open
E_BFM	equ	8	; Bad file mode
E_DAT	equ	9	; Data file error (see <FILERR>)
E_IFN	equ	10	; Invalid file number
E_LNF	equ	11	; Label not found
E_NCS	equ	12	; Null control string
E_RDF	equ	13	; Redefining symbol to different type
E_RWG	equ	14	; .RETURN without .GOSUB
E_TMG	equ	15	; .GOSUB nesting too deep
E_TYP	equ	16	; Type mismatch
E_SYM	equ	17	; Undefined symbol
E_TMB	equ	18	; Too many nested blocks [Out of memory]
E_EWB	equ	19	; .END without .BEGIN
E_EWE	equ	20	; .EXIT without .END
E_SPW	equ	21	; Spawn failure
;E_SUB	equ	22	; String substitution error
;E_XQT	equ	23	; Too many concurrent .XQTs

; Class 2 - can be handled with <ERRCTL>

E_FNF	equ	24	; File not found (command file)
E_LEN	equ	25	; Line too long (command file read error)
E_SEK	equ	26	; Seek error (command file)
E_STL	equ	27	; String too long (string expression)
E_MEM	equ	28	; Not enough memory
E_OVF	equ	29	; Numeric overflow
E_UDF	equ	30	; Numeric underflow
E_SNE	equ	31	; Syntax error
E_TMC	equ	32	; Too many nested command files
E_ROS	equ	33	; Redefining a read-only symbol

