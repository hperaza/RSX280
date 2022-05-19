; System configuration file

; Current system version

SVER.H	equ	6	; major
SVER.L	equ	31	; minor

; Filesystem version

FVER.H	equ	5	; major
FVER.L	equ	0	; minor

; Clock ticks per second

TCKSEC	equ	100

F$CHKP	equ	1	; set this to 1 to enable checkpoint file support

F$HALT	equ	1	; set this to 1 to halt CPU on idle condition

F$TTMX	equ	0	; set this to 1 to generate a terminal driver
			;  that supports 8 software-multiplexed units on
			;   the secondary serial port of the P112

; Resident device drivers
;
; - at least one terminal driver that defines TT0: should be present
; - at least one disk driver should be present for the system to boot
; - the null device driver is not mandatory, but recommended
; - any custom device drivers should be added to this table so they can
;   be installed automatically on startup

GENDEV	MACRO
;;	dw	DEVDY##	; floppy disk driver
	dw	DEVDU##	; GIDE hard disk driver
	dw	DEVTT##	; terminal driver (2 or 8 units, see TTMPX)
;;	dw	DEVLP##	; printer device driver (optional)
;;	dw	DEVCT## ; example driver to read a DS1820 on the parallel port
	dw	DEVCO##	; console driver (optional, for console logger)
	dw	DEVNL##	; null device driver
	ENDM

