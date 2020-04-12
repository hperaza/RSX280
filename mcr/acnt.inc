; User account file structure offsets

AF.USR	equ	0		; user ID (1 byte)
AF.GRP	equ	AF.USR + 1	; group ID (1 byte)
AF.UNAM	equ	AF.GRP + 1	; user name = default user directory (9 bytes)
AF.NAME	equ	AF.UNAM + 9	; full name (32 bytes)
AF.PWD	equ	AF.NAME + 32	; encrypted password (16 bytes)
AF.SYD	equ	AF.PWD + 16	; login device and unit (3 bytes)
AF.DFP	equ	AF.SYD + 3	; default file protection (2 bytes)
AF.CLI	equ	AF.DFP + 2	; user CLI (3 bytes)
AF.CRE	equ	AF.CLI + 3	; created date and time (7 bytes, BCD)
AF.EXP	equ	AF.CRE + 7	; expiration date (4 bytes, BCD)
AF.LDT	equ	AF.EXP + 4	; last login date and time (7 bytes, BCD)
AF.LLA	equ	AF.LDT + 7	; last login attempt date and time (7 bytes, BCD)
AF.LGNS	equ	AF.LLA + 7	; total logins (2 bytes)
AF.FAIL	equ	AF.LGNS + 2	; total login failures (2 bytes)
AF.LLF	equ	AF.FAIL + 2	; login failures since last succesful login (1 byte)
AF.LF10	equ	AF.LLF + 1	; login failures in the last 10 min (1 byte)
AF.PWML	equ	AF.LF10 + 1	; password min length (1 byte)
AF.PWLT	equ	AF.PWML + 1	; password lifetime (1 byte)
AF.OPT	equ	AF.PWLT + 1	; option bits (2 bytes)
AF.RES	equ	AF.OPT + 2	; reserved (26 bytes)

AFSZ	equ	AF.RES + 26	; record length = 128 bytes

; Option bits

AO.SIL	equ	0		; silent login
AO.NPW	equ	1		; no password
AO.NPC	equ	2		; no password change
AO.LCK	equ	3		; locked (disabled) account
AO.RLD	equ	4		; remote login disabled
AO.SLV	equ	5		; slave terminal

