; KDEF.INC
;       KERMIT - (Celtic for "FREE")
;
;       This is the RSX180/280 implementation of the Columbia University
;       KERMIT file transfer protocol. (C) 2021, Hector Peraza.
;
;       Version 4.0
;
; Derived from Kermit-80, originally written by Bill Catchings of the
; Columbia University Center for Computing Activities, 612 W. 115th St.,
; New York, NY 10025. with contributions by Frank da Cruz, Daphne Tzoar,
; Bernie Eiben, Bruce Tanner, Nick Bush, Greg Small, Kimmo Laaksonen,
; Jeff Damens, and many others. 
;
;       Copyright June 1981,1982,1983,1984,1985 Columbia University
;
;       This file contains definitions used by all the other modules of
;       Kermit.
;
; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
; Revision history (latest first):
;
; edit 10, 27-Dec-2020 by H. Peraza: converted to Z80, targeting RSX180/280.
;
; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
; Old Kermit-80 4.11 revision history:
;
; edit 9, 30-Nov-1990 by MF. Make "fairness" count "prfair" be 50 so
;	console gets checked a bit more often.
; edit 8, 11-Sep-1990 by MF.  Make default RECEIVE and SEND packet-size
;	80 (per Kermit standard) as packet size is adjustable in Version
;	4.10.
; edit 7 16-Jun-86 OBSchou.  Added cmnum in the command opcodes.  This gets a
;       number from the user inot variable number.  No checking on overflow.
;
; edit 6 13-May-86 OBSchou.  BDOS calls trapped to check for console use
;       as we want to substitute in commands from a TAKE file.  Trapping
;       means I dont have to go through an check ever BDOS call...
;
; edit 5: 22-Apr-86 by Bertil Schou, Loughborough University, UK
;       moved some definitions from the CP4SYS.ASM file to here for 
;       Kermit version 4.06
;
; edit 4: 6-Feb-85 by Charles Carvalho
;       modify pcc007: replace ffussy assembly switch with runtime test.
;       add "getvnm" - get CP/M version number.
;
; edit 3: 13-Jan-85 by Vanya J.Cooper Pima Commun. College Tel: 602-884-6809
;
; pcc007 2-Jan-85        vjc     modules:cp4def,cp4cmd
;       Cmifil is too fussy about what characters to accept in a
;       filespec.  My CP/M manual says any printable character is ok
;       except <>.,;:?*[], and lower case.  In practice, even those work
;       sometimes.  Kermit itself uses '&' if file warning is on,
;       and then won't let you reference the file.  Allow all
;       printable characters except those above.  Add conditional
;       ffussy, so that if not ffussy, all special characters will be
;       allowed, just convert lower to upper-case.
;
; pcc008 2-Jan-85        vjc     modules:cp4def,cp4tt,cp4utl
;       Keyboard input during CONNECT mode can get locked out if
;       there is enough input from the modem port to keep prtchr
;       busy.  This can happen for example, if the printer is running
;       at the same speed as the modem line, leaving you helpless to
;       turn it off or abort the host.  Add a fairness count, so that
;       at least every prfair characters we look at console input.
;
; edit 2: July 10, 1984 (CJC)
;       Remove defines for TRUE and FALSE, during reorganization for LASM
;       compatibility.  If we're using LASM, this file is linked by CP4KER
;       or CP4TYP, and links to CP4MIT or CP4LNK.  Also, push comments around
;       a little.
;
; edit 1: May, 1984 (CJC)
;       extracted from CPMBASE.M80 version 3.9; modifications are described
;       in the accompanying .UPD file.
;

; Symbolic Definitions for some ASCII characters

VERNO	EQU	22	; minor version number
REVNO	EQU	0	; revision level, 0-26 yields A-Z

SOH	equ	01o	; ASCII SOH (Control-A)
CTRLC	equ	03o	; ASCII ETX (Control-C)
BELL	equ	07o	; ASCII BEL (Control-G)
BS	equ	10o	; ASCII backspace (Control-H)
TAB	equ	11o	; ASCII Tab (Control-I)
LF	equ	12o	; ASCII Line Feed (CTRL-J)
FF	equ	14o	; ASCII Form Feed (CTRL-L)
CR	equ	15o	; ASCII Carriage Return (CTRL-M)
CTRLO	equ	17o	; ASCII SI (Control-O)
XON	equ	21o	; ASCII XON (Control-Q)
XOFF	equ	23o	; ASCII XOFF (Control-S)
CTRLU	equ	25o	; ASCII NAK (Control-U)
CTRLX	equ	30o	; ASCII CAN (Control-X)
ESC	equ	33o	; ASCII ESCape
SEMICO	equ	';'	; ASCII Semicolon
SUBT	equ	32o	; ASCII SUB (Control-Z)
CTRLZ	equ	SUBT	; ASCII SUB (Control-Z)
SPACE	equ	' '	; ASCII Space
DEL	equ	177o	; ASCII DELete (rubout)
	page
PAREVN	equ	00h	; even parity
PARMRK	equ	03h	; mark parity
PARNON	equ	06h	; no parity (eighth bit is data)
PARODD	equ	09h	; odd parity
PARSPC	equ	0Ch	; space parity

DEFPAR	equ	PARNON	; default parity
IBMPAR	equ	PARMRK	; IBM COMTEN's parity

BUFSIZ	equ	512	; file I/O block (sector) size
CMBUFL	equ	128-3	; command buffer length

MAXPKT	equ	'~'-' '+2 ; maximum size of a packet
MAXTRY	equ	5	; number of retries on a packet
IMXTRY	equ	16	; number of retries send initiate
PRFAIR	equ	50	; PRTCHR fairness count

; Opcodes for command parser

CMKEY	equ	01h	; parse a keyword
CMIFI	equ	02h	; parse an input file spec (can be wild)
CMOFI	equ	03h	; parse an output file spec
CMCFM	equ	04h	; parse a confirm
CMTXT	equ	05h	; parse text
CMNUM	equ	06h	; parse a number
CMIFIN	equ	10h	; parse an input file spec (but no error output)
	page

; Protocol parameters, some of these can be changed with commands

DRPSIZ	equ	80	; default receive packet size. (maximum is 94)
DSPSIZ	equ	80	; default send packet size. (maximum is 94)
DSTIME	equ	8	; default send timeout interval
DRTIME	equ	5	; default receive timeout interval

DSPAD	equ	00h	; default send padding
DRPAD	equ	00h	; default receive padding
DSPADC	equ	00h	; default send padding char
DRPADC	equ	00h	; default receive padding char
DSEOL	equ	CR	; default send EOL char
DREOL	equ	CR	; default receive EOL char
DSQUOT	equ	'#'	; default send quote char
DRQUOT	equ	'#'	; default receive quote char
DSCHKT	equ	'1'	; default checksum type

; Define VT or Terminal type values

VTDEFO	equ	0	; VT52 emulation by terminal itself
VTDEFV	equ	1	; VT52 emulation by TTAB tables in KSYS.MAC
VTDEFD	equ	2	; Dumb Terminal (just prints)
VTDEFE	equ	3	; Terminal emulation done outside (in overlay)

FALSE	equ	0
TRUE	equ	NOT	FALSE

debug	equ	FALSE	; set false for running system.  True => does some
	; unusual or unexpected things.

;DEFESC	equ	']'-100o ; the default escape character
DEFESC	equ	'\'-100o ; the default is Control-\ -- it's easier
TACVAL	equ	'@'	; typical TAC intercept character

; RSX180-specific stuff

LUNTI	equ	1	; LUN for terminal I/O
LUNMDM	equ	2	; LUN for communication line I/O
LUNTAK	equ	3	; LUN for TAKE file I/O
LUNFIL	equ	4	; LUN for send/receive file I/O
LUNFI2	equ	5	; LUN for file copy operations
LUNDIR	equ	6	; LUN for DIR, COPY and RENAME file ops
LUNSYS	equ	7	; LUN for accessing system files
LUNTMP	equ	8	; LUN for some miscellaneous operations

EFNTM	equ	10	; event flag for mark-time
EFNTI	equ	11	; event flag for terminal I/O
EFNGC	equ	12	; event flag for async terminal input
EFNMD	equ	13	; event flag for communication line I/O
EFNFIL	equ	14	; event flag for file I/O operations

