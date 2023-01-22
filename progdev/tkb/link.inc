; DASMed version of DR LINK.COM by W. Cirsovius
; (Heavily) Modified by H. Peraza

; %%%% Fix up LINK revision NOTE: Base 1.31

LINKver	macro
	db	'2.34'
	endm

FALSE	equ	0
TRUE	equ	-1		; all bits set!

; %%%% Set to TRUE to enable a mod, FALSE to disable (DEFAULT)

@@HASH	equ	TRUE		;; Allow # in symbols

TPA	equ	0100h		; default load address

RecLen	equ	128		; record length
ExtLen	equ	128		; extent size
BufLen	equ	128		; command line buffer length
PageLen	equ	256		; memory page length
HashLen	equ	128
HeadLen	equ	 12		; Header length on prefix

MaxCR	equ	128		; Max current record
MaxChIx	equ	  8
IDXend	equ	0FEh
	
.Rd	equ	0
.Wr	equ	1

BFlen1	equ	 256		; YYABS, YYCOM
				;               /XM: XXABS, XXCOM
BFlen2	equ	 512		; XXCOM
BFlen3	equ	1024		; input buffer size, YYPRG, YYDAT, XXABS
				;               /XM: XXPRG, XXDAT
BFlen4	equ	6144		; XXDAT
BFlen5	equ	8192		; XXPRG

@nam	equ	 8
@ext	equ	 3

.LD.BC	equ	01h
.JP	equ	0C3h
.CALL	equ	0CDh

@aseg	equ	00b
@cseg	equ	01b
@dseg	equ	10b
@comn	equ	11b

NumSegs	equ	4

; XX.. PB structure

X$.cur	equ	 0
X$.max	equ	 2
X$.len	equ	 4
X$.buf	equ	 6
X$.high	equ	 8
X$.low	equ	10
X$.RD	equ	12
X$.WR	equ	13
X$.FCB	equ	14

; YY.. PB structure

Y$.buf	equ	0
Y$.cur	equ	2
Y$.len	equ	4
Y$.WR	equ	6
Y$.FCB	equ	7

CtrlC	equ	03h
TAB	equ	09h
LF	equ	0Ah
FF	equ	0Ch
CR	equ	0Dh
EOF	equ	1Ah
SpcChar	equ	'#'		; Denotes special symbol prefix

MSB	equ	10000000b
NoMSB	equ	01111111b

.ENTRY	equ	0000b
.ModNam	equ	0010b
.ExtItm	equ	0100b
.COMMON	equ	0101b
.EntPnt	equ	0111b
.ModEnd	equ	1110b
.EndFil	equ	1111b

.OC	equ	0		; COM output
.OP	equ	1		; PRL output
.OR	equ	2		; RSP output
.OS	equ	3		; SPR output
.OY	equ	4		; OVL output*
.OX	equ	5		; RSX output
.OT	equ	6		; TSK output

; Symbol table structure

;    0     1     2     3     4     5     6 ...             N
; +-----+-----+-----+-----+-----+-----+-----...---+-----+-----+
; |   Chain   | Tlen|   Offset  | Slen| Symbol ...|   COsize  |
; +-----+-----+-----+-----+-----+-----+-----...---+-----+-----+
;
; Chain		Chain address
; Tlen		Total length of symbol entry (N)
;		Total length + code
;		code bits 76xx.xxxx
;		Bit 7 - LIB REQ
;		Bit 6 - Symbol fixed bit
; Offset	Offset within segment
; Slen		Symbol length + code
;		code bits MBBx.xxxx
;		BB address mode ------------------++
;		code	00	External	0.00
;			A0	Entry, CSEG	1.01
;			C0	Entry, DSEG	1.10
;			E0	COMMON		1.11
;		MSB set on COMMON and ENTRY ----^
; Symbol	The symbol name
; COsize	Size of COMMON block

.SymVal	equ	3
.SymCtr	equ	5
.SymNam	equ	6
.SymHdr	equ	6

$$ENT	equ	7

X$$ext	equ	7
X$$fix	equ	6
X$$ocv	equ	5

X@@ext	equ	1 SHL X$$ext
X@@fix	equ	1 SHL X$$fix
X@@ocv	equ	1 SHL X$$ocv

X@@mod	equ	00000011b

.SymLen	equ	00011111b
.ItmLen	equ	00011111b	;00111111b
.ItmBit	equ	10011111b

; Temporary (Y) table structure
;
;    0     1     2     3     4     5     6     7     8
; +-----+-----+-----+-----+-----+-----+-----+-----+-----+
; | Sta |    Adr    |    Val    |    Lnk    |    Off    |
; +-----+-----+-----+-----+-----+-----+-----+-----+-----+
;
; Sta		Status of entry
;		Bit 0,1	Address mode
;		Bit  2	Chain type flag (0=CHAIN ADDRESS, 1=CHAIN EXTERNAL)
;		Bit  3	Offset flag
;		Bit  4	Offset sign
; Adr		Current address
; Val		Current value
; Lnk		Link pointer, ends with zero
; Off		Offset, if defined

.Y$Sta	equ	0
.Y$Adr	equ	1
.Y$Val	equ	3
.Y$Lnk	equ	5
.Y$Off	equ	7

Y$$sign	equ	4
Y$$off	equ	3
Y$$type	equ	2

Y@@sign	equ	1 SHL Y$$sign
Y@@off	equ	1 SHL Y$$off
Y@@type	equ	1 SHL Y$$type

Y@@mod	equ	00000011b

Y.eof	equ	-1

MaxOVL	equ	5

_LabLen	equ	7

ColMask	equ	00000011b
LabCol	equ	9
LabDel	equ	3
UnkMask	equ	00000111b

; Segment table indexes

IASEG	equ	0
ICSEG	equ	2
IDSEG	equ	4
ICOMN	equ	6
