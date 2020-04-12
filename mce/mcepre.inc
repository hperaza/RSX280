	SUBTTL	MCE - Configuration prefixfile MCEPRE.INC
;
; MCE Macro Prefixfile
;
IDNPRE  equ 429   ; Ident code of MCEPRE.INC
COMPND  equ TRUE  ; Compound command (&) support
UPR     equ TRUE  ; User prompt
EXTNPR  equ TRUE  ; Extended prompt included
PRMDEF  equ 0     ; Extended prompt off by default
FILE    equ TRUE  ; MCE Init-file support included
INSDEF  equ 1     ; Return to insert mode by default
INTDEF  equ 1     ; Save internal commands by default
OLDDEF  equ 1     ; Save old commands by default
OVSDEF  equ 0     ; Don't return to overwrite mode by default
STATUS  equ TRUE  ; Status line support
STADEF  equ 0     ; Status line on by default
VT2XX   equ TRUE  ; VT2plus support included
VT4XX   equ TRUE  ; VT4plus support included
TIMOUT	equ FALSE ; Time-Out support included
TMOTIM  equ 10    ; Time-Out 10 minutes.
TMOSET  equ FALSE ; Time-Out setable
TMOON   equ FALSE ; Time-Out OFF by default
MAXFIF  equ 20    ; Maximum number of entries in FIFO
MAXHLP  equ 'L'   ; Highest available help page
MINCHR  equ 3     ; Minimum cmd-line length to be saved
RTMESS  equ TRUE
RTEXIT  equ FALSE
HTMESS  equ TRUE
HTEXIT  equ FALSE
CLISUP  equ FALSE
PWD	equ TRUE
SYLOGIN	equ FALSE
EDT	equ TRUE
SILENT	equ FALSE
