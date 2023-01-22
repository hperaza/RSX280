; System configuration file

; Current system version

SVER.H	equ	6	; major
SVER.L	equ	37	; minor

; Filesystem version

FVER.H	equ	5	; major
FVER.L	equ	0	; minor

; Clock ticks per second

TCKSEC	equ	100

F$CHKP	equ	1	; set this to 1 to enable checkpoint file support

F$HALT	equ	1	; set this to 1 to halt CPU on idle condition

F$TTMX	equ	0	; this option is only supported on the P112

; Resident device drivers
;
; - at least one terminal driver that defines TT0: should be present
; - at least one disk driver should be present for the system to boot
; - the null device driver is not mandatory, but recommended
; - any custom device drivers should be added to this table so they can
;   be installed automatically on startup

GENDEV	MACRO
	dw	DEVDU##	; CF disk driver
	dw	DEVTT##	; terminal driver
;;	dw	DEVLP##	; printer device driver (optional)
	dw	DEVCO##	; console driver (optional, for console logger)
	dw	DEVNL##	; null device driver
	ENDM

