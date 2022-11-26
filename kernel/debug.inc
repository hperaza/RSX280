;**********************************************************************;
;                                                                      ;
;   This file is part of RSX280, a multi-tasking OS for the Z280.      ;
;   Copyright (C) 1985-2022, Hector Peraza.                            ;
;                                                                      ;
;   This program is free software; you can redistribute it and/or      ;
;   modify it under the terms of the GNU General Public License as     ;
;   published by the Free Software Foundation; either version 2 of     ;
;   the License, or (at your option) any later version.                ;
;                                                                      ;
;   This program is distributed in the hope that it will be useful,    ;
;   but WITHOUT ANY WARRANTY; without even the implied warranty of     ;
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      ;
;   GNU General Public License for more details.                       ;
;                                                                      ;
;   You should have received a copy of the GNU General Public License  ;
;   along with this program; if not, write to the Free Software        ;
;   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.          ;
;                                                                      ;
;**********************************************************************;

; Simple Z280 standalone debug monitor.

	.Z280

	include	Z280.INC

CR	equ	0Dh
LF	equ	0Ah
TAB	equ	09h

FALSE	equ	0
TRUE	equ	NOT FALSE

CMD_C	equ	FALSE		; compare command?
CMD_S	equ	FALSE		; search command?
CMD_T	equ	FALSE		; transfer command?
CMD_B	equ	FALSE		; breakpoint command?

;;	cseg

	public	$DBMON,$DBTRP,$DMTRP,$DBMSG

;-----------------------------------------------------------------------

; Main entry point

$DBMON:	di
	ld	sp,mon_stack	; setup local stack
	ld	hl,prompt
	call	print		; output prompt
	call	getln		; get command
	push	$DBMON		; push return address
	xor	a
	ld	(argset),a
	ld	hl,lnbuf
	call	skipbl		; skip blanks and get command char
	or	a
	ret	z		; ignore empty lines
	inc	hl
	ld	(lnptr),hl
	call	uc		; convert command char to uppercase
	cp	'X'		; show regs?
	jp	z,showregs
	cp	'M'		; show MMU regs?
	jp	z,showmmu
	cp	'I'		; input/output commands may have a word	modifier
	call	z,chkmod
	cp	'W'
	call	z,chkmod
	push	af
	call	get_args	; parse command and get arguments
	ld	bc,(arg3)	; load arguments into registers
	ld	de,(arg2)
	ld	hl,(arg1)
	pop	af
	cp	'D'		; dump?
	jp	z,dump
	IF	CMD_C
	cp	'C'		; compare?
	jp	z,comp
	ENDIF
	cp	'F'		; fill?
	jp	z,fill
	IF	CMD_S
	cp	'S'		; search?
	jp	z,search
	ENDIF
	IF	CMD_T
	cp	'T'		; move (transfer)?
	jp	z,move
	ENDIF
	cp	'E'		; examine?
	jp	z,exam
	IF	CMD_B
	cp	'B'		; set breakpoint?
	jp	z,setbrk
	ENDIF
	cp	'G'		; go?
	jp	z,run
	cp	'I'		; input?
	jp	z,inport
	cp	'O'		; output?
	jp	z,outport
	cp	'N'		; continue?
	jp	z,continue
	cp	'W'		; word?
	jp	z,word
	cp	'/'		; tasks?
	jp	z,tasks
error:	call	cr_lf
	ld	a,'?'		; unknown command
	call	mon_putch
	jr	$DBMON

chkmod:	ld	c,a
	ld	a,(hl)
	call	uc
	cp	'W'
	ld	a,c
	ret	nz
	inc	hl
	ld	(lnptr),hl
	ld	hl,argset
	set	7,(hl)
	ret

prompt:	db	CR,LF,"Z280>",0

getln:	ld	hl,lnbuf	; HL = start of command buffer
	ld	b,1
get:	call	mon_getch	; get char
	cp	CR
	jr	z,eol		; CR ends command
	cp	8
	jr	z,bkspc		; del and backspace erase prev char
	cp	7Fh
	jr	z,bkspc
	cp	3
	jr	z,ctrlc		; ^C restarts
	cp	' '
	jr	c,get		; ignore any other control chars
	ld	c,a
	ld	a,b
	cp	30-1		; max length reached?
	jr	nc,get		; ignore extra input if yes
	ld	a,c
	call	mon_putch	; echo char
	ld	(hl),a		; store in buffer
	inc	hl		; advance pointer
	inc	b		;  and counter
	jr	get		; loop
bkspc:	dec	b
	jr	z,getln
	ld	a,8
	call	mon_putch	; erase prev char from the screen
	call	space
	ld	a,8
	call	mon_putch
	dec	hl
	jr	get
eol:	ld	(hl),0		; end of input, append null char
	call	mon_putch
	ret
ctrlc:	ld	hl,ctlcm
	call	print
	jp	$DBMON

ctlcm:	db	"^C",CR,LF,0

; convert char to uppercase

uc:	cp	'a'
	ret	c
	cp	'z'+1
	ret	nc
	and	5Fh
	ret

; output string

print:	ld	a,(hl)
	or	a
	ret	z
	call	mon_putch
	inc	hl
	jr	print

; skip blanks

skip1:	inc	hl
skipbl:	ld	a,(hl)
	or	a
	ret	z
	cp	' '
	jr	z,skip1
	cp	TAB
	jr	z,skip1
	ret

; get command line arguments

get_args:
	ld	hl,0
	ld	(arg1),hl	; unspecified args default to zero
	ld	(arg2),hl
	ld	(arg3),hl
	ld	de,(lnptr)
	ld	c,01h
	call	gethex		; get first arg
	ld	(arg1),hl
	ld	(arg2),hl
	ret	c
	ld	c,02h
	call	gethex		; get second arg
	ld	(arg2),hl
	ret	c
	ld	c,04h
	call	gethex		; get third arg
	ld	(arg3),hl
	ret	c
	jp	error		; error if more args

; get hexadecimal value from command line

gethex:	ld	hl,0
gh1:	ld	a,(de)
	or	a		; end of command line?
	scf
	ret	z		; yes, return with CY flag set
	inc	de
	cp	' '
	jr	z,gh1
	cp	','
	ret	z
	call	tohex
	jp	c,error
	call	adddig
	jp	c,error
	ld	a,(argset)
	or	c		; set bit if argument present
	ld	(argset),a
	jr	gh1

adddig:	add	hl,hl
	ret	c
	add	hl,hl
	ret	c
	add	hl,hl
	ret	c
	add	hl,hl
	ret	c
	add	hl,a
	ret

tohex:	call	uc
	sub	'0'
	ret	c
	cp	10
	ccf
	ret	nc
	cp	11h
	ret	c
	cp	17h
	ccf
	ret	c
	sub	7
	ret

; compare addresses and return one level higher if they match

next:	call	?stop		; check for ^C abort
	cpw	hl,de		; end address reached?
	jr	z,cmp_eq	; return one level higher if yes
	inc	hl		; else increment HL and return
	ret
cmp_eq:	inc	sp
	inc	sp
	ret

; check for ^C abort or XON/XOFF

?stop:	call	mon_status
	or	a
	ret	z
	call	mon_getch
	cp	3		; ^C aborts
	jp	z,ctrlc
	cp	13h		; ^S suspends output
	ret	nz
	call	mon_getch
	cp	3
	jp	z,ctrlc
	ret

	IF	CMD_T

; T addr1,addr2,addr3
; Transfer (move) region addr1...addr2 to addr3,
; source and dest regions may overlap

move:	ld	a,b
	or	c
	ret	z
	cpw	hl,de
	ret	z
	jr	c,m1
	ldir
	ret
m1:	add	hl,bc
	ex	de,hl
	add	hl,bc
	ex	de,hl
	dec	hl
	dec	de
	lddr
	ret

	ENDIF

; D addr1,addr2[,bank]
; Dump region addr1...addr2

dump:	push	hl
	ld	l,c
	ld	h,b
	ld	a,(argset)
	bit	2,a		; bank specified?
	jr	nz,dmp1		; yes
	ld	hl,0		; else default to system bank
dmp1:	call	ubank		; select user bank
	pop	hl

dmp2:	call	out_addr	; output address
	push	hl
	call	dmps3		; align start
dmph:   ldup	a,(hl)		; get byte
	call	outbyte		; output as hexadecimal
	call	?stop		; check for ^C abort
	cpw	hl,de
	jr	z,enddmp	; exit loop when done
	inc	hl
	ld	a,l
	and	0Fh		; max 16 bytes per line
	jr	nz,dmph
	pop	hl
	call	dmpa		; dump ASCII as well
	jr	dmp2

enddmp:	inc	hl
	ld	a,l
	neg	a
	ld	l,a
	call	dmps3		; align end
	pop	hl
dmpa:	ld	c,1
	call	dmps		; align start
dmpa1:	ldup	a,(hl)		; get byte
	cp	' '		; printable?
	jr	c,outdot
	cp	7Fh
	jr	c,char
outdot:	ld	a,'.'		; output a dot if non-printable
char:	call	mon_putch
	call	?stop		; check for ^C abort
	cpw	hl,de
	ret	z		; return when done
	inc	hl
	ld	a,l
	and	0Fh		; max 16 chars per line
	jr	nz,dmpa1
	ret

dmps3:	ld	c,3
dmps:	ld	a,l
	and	0Fh
	ret	z
	push	de
	ld	b,a
	ld	e,c
dmp4:	ld	d,e
dmp5:	call	space
	dec	d
	jr	nz,dmp5
	djnz	dmp4
	pop	de
	ret

; select user bank (in HL)

ubank:	push	hl
	iopage	MMUP
	xor	a
	out	(PDR),a
	pop	hl
	addw	hl,($SBASE)
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	set	3,l
	ld	c,BMP
	ld	b,16
	ld	a,10h
ub1:	outw	(c),hl
	add	hl,a
	djnz	ub1
	ret

; F addr1,addr2,byte
; Fill region addr1...addr2 with byte

fill:	ld	(hl),c
	call	next
	jr	fill

	IF	CMD_C

; C addr1,addr2,addr3
; Compare region addr1...addr2 with region at addr3

comp:	ld	a,(bc)
	cp	(hl)
	jr	z,same
	call	out_addr
	ld	a,(hl)
	call	outbyte
	ld	a,(bc)
	call	outbyte
same:	inc	bc
	call	next
	jr	comp

	ENDIF

	IF	CMD_S

; S addr1,addr2,byte
; Search region addr1...addr2 for byte

search:	ld	a,c
	cp	(hl)
	jr	nz,scont
	call	out_addr
	dec	hl
	ld	a,(hl)
	call	outbyte
	ld	a,'('
	call	mon_putch
	inc	hl
	ld	a,(hl)
	call	ahex
	ld	a,')'
	call	mon_putch
	call	space
	inc	hl
	ld	a,(hl)
	call	outbyte
	dec	hl
scont:	call	next
	jr	search

	ENDIF

; E addr[,bank]
; Examine and modify memory starting at addr

exam:	ex	de,hl
	ld	a,(argset)
	bit	1,a		; bank specified?
	jr	nz,e1		; yes
	ld	hl,0		; else default to system bank
e1:	call	ubank		; select user bank
	ex	de,hl
e3:	call	out_addr
	ldup	a,(hl)		; fetch byte from user space
	call	outbyte
	push	hl
	call	getln
	pop	hl
	ld	de,lnbuf
	ld	a,(de)
	or	a		; skip this location?
	jr	z,cont		; yes
	cp	'.'		; end?
	ret	z		; yes
	push	hl
	call	gethex
	ld	a,l
	pop	hl
	ldup	(hl),a		; store new value
cont:	inc	hl
	jr	e3

; I[W] port[,iopage]
; Input from port

inport:	ld	a,(argset)
	bit	1,a
	jr	nz,i1
	ld	de,0
i1:	call	cr_lf
	ld	a,e
	call	ahex		; output I/O page
	ld	a,':'
	call	mon_putch
	ld	a,l
	call	ahex		; output port address
	ld	a,'='
	call	mon_putch
	ex	de,hl
	ld	c,IOP
	ldctl	(c),hl		; select I/O page
	ex	de,hl
	ld	c,l		; get port address into BC
	ld	b,h
	ld	a,(argset)
	bit	7,a		; word mode?
	jr	z,iw
	inw	hl,(c)		; yes
	jp	hlhex
iw:	in	a,(c)		; no
	jp	ahex

; O[W] port,value[,iopage]
; Output to port

outport:push	hl
	ld	l,c
	ld	c,IOP
	ldctl	(c),hl		; select I/O page
	pop	bc		; get port address into BC
	ex	de,hl		; and value to output into HL
	ld	a,(argset)
	bit	7,a		; word mode?
	jr	z,ow
	outw	(c),hl		; yes
	ret
ow:	ld	a,l
	out	(c),a		; no
	ret

; W addr[,bank]
; Display word at addr

word:	ex	de,hl
	ld	a,(argset)
	bit	1,a		; bank specified?
	jr	nz,dw1		; yes
	ld	hl,0		; else default to system bank
dw1:	call	ubank		; select user bank
	ex	de,hl
	call	out_addr
	ldup	a,(hl)		; fetch word from user space
	ld	e,a
	inc	hl
	ldup	a,(hl)
	ld	d,a
	ex	de,hl
	jp	hlhex

	IF	CMD_B

; B addr
; set Breakpoint at addr. It will be automatically removed when reached.

setbrk:	ld	(brkadr),hl
	ld	a,(hl)
	ld	(brkval),a
	ld	a,DBGRST OR 0C7h
	ld	(hl),a
	ld	a,0C3h
	ld	(DBGRST),a
	ld	hl,$DBTRP
	ld	(DBGRST+1),hl
	ret

	ENDIF

; G addr
; Go (execute) program at addr. Program may use return 
; instruction to return to monitor

run:
;;	ei
	call	r1
;;	di
	ret
r1:	jp	(hl)

; display 16-bit address followed by a colon and a space

out_addr:
	call	cr_lf
	call	hlhex
	ld	a,':'
	call	mon_putch
	jr	space

; display 8-bit value followed by a space

outbyte:
	call	ahex
space:	ld	a,' '
	jp	mon_putch

; display 16-bit hexadecimal value

hlhex:	ld	a,h
	call	ahex
	ld	a,l
ahex:	push	af
	rrca
	rrca
	rrca
	rrca
	call	nhex
	pop	af
nhex:	and	0Fh
	add	a,90h
	daa
	adc	a,40h
	daa
	jp	mon_putch

; display newline

cr_lf:	push	af
	ld	a,CR
	call	mon_putch
	ld	a,LF
	call	mon_putch
	pop	af
	ret

; Trap (breakpoint) entry point. Saves all registers and current MMU status.

$DBTRP:	di
	ld	(pgm_sp),sp
	ld	sp,reg_stack
	push	af
	push	bc
	push	de
	push	hl
	ex	af,af'
	push	af
	ex	af,af'
	exx	
	push	bc
	push	de
	push	hl
	exx
	push	ix
	push	iy
	ldctl	hl,usp
	push	hl
	ld	c,IOP
	ldctl	hl,(c)
	push	hl
	ld	sp,mon_stack
	call	savmmu
	ld	hl,(pgm_sp)
	ldw	de,(hl)
	ld	(pgm_pc),de
	ld	hl,brk_msg
	call	cr_lf
	call	print
	ld	hl,atpc_msg
	call	print
	call	chkbrk
	ld	hl,(pgm_pc)
	call	hlhex
	call	cr_lf
	jp	$DBMON

chkbrk:	ld	hl,(brkadr)
	ld	de,(pgm_pc)
	dec	de		; adjust user's PC
	cpw	hl,de		; breakpoint set by us?
	ret	nz
	ld	a,(brkval)
	ld	(hl),a		; remove it if yes
	ld	hl,0
	ld	(brkadr),hl
	ld	hl,(pgm_sp)
	ldw	(hl),de
	ld	(pgm_pc),de	; save corrected user's PC
	ret

brk_msg:
	db	"Break",0
atpc_msg:
	db	" at PC=",0

$DMTRP:	di
	push	hl
	push	bc
	ld	c,IOP
	ldctl	hl,(c)
	ld	(pgm_iop),hl
	pop	bc
	pop	hl
	ex	(sp),hl
	push	af
	call	cr_lf
	call	print
	inc	hl
	pop	af
	ex	(sp),hl
	push	hl
	push	bc
	ld	c,IOP
	ld	hl,(pgm_iop)
	ldctl	(c),hl
	pop	bc
	pop	hl
	jp	$DBTRP

; N[1]
; coNtinue execution from last breakpoint

continue:
	call	rstmmu
	ld	a,h
	or	l		; if 1st arg is <>0, continue w/o enabling ints
	jr	nz,di_continue
	ld	sp,pgm_iop
	pop	hl
	ld	c,IOP
	ldctl	(c),hl
	pop	hl
	ldctl	usp,hl
	pop	iy
	pop	ix
	exx
	pop	hl
	pop	de
	pop	bc
	exx
	ex	af,af'
	pop	af
	ex	af,af'
	pop	hl
	pop	de
	pop	bc
	pop	af
	ld	sp,(pgm_sp)
	ei
	ret
di_continue:
	ld	sp,pgm_iop
	pop	hl
	ld	c,IOP
	ldctl	(c),hl
	pop	hl
	ldctl	usp,hl
	pop	iy
	pop	ix
	exx
	pop	hl
	pop	de
	pop	bc
	exx
	ex	af,af'
	pop	af
	ex	af,af'
	pop	hl
	pop	de
	pop	bc
	pop	af
	ld	sp,(pgm_sp)
	ret

; X
; show the contents of the CPU registers

showregs:
	ld	hl,regdmp
showr1:	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	a,d
	or	e
	ret	z
	ex	de,hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	push	bc
	ex	de,hl
	call	print
	ex	(sp),hl
	call	hlhex
	pop	hl
	inc	hl
	jr	showr1

regdmp:	dw	pgm_af
	db	CR,LF,"AF=",0
	dw	pgm_bc
	db	" BC=",0
	dw	pgm_de
	db	" DE=",0
	dw	pgm_hl
	db	" HL=",0
	dw	pgm_ix
	db	" IX=",0
	dw	pgm_iy
	db	" IY=",0
	dw	pgm_iop
	db	" IOP=",0
	dw	pgm_af1
	db	CR,LF,"AF'",0
	dw	pgm_bc1
	db	" BC'",0
	dw	pgm_de1
	db	" DE'",0
	dw	pgm_hl1
	db	" HL'",0
	dw	pgm_pc
	db	" PC=",0
	dw	pgm_sp
	db	" SP=",0
	dw	pgm_usp
	db	" USP=",0
	dw	0

; M
; Show Z180 MMU state

showmmu:
	ld	hl,mmsg1
	call	print
	iopage	MMUP
	ld	c,MMCR
	inw	hl,(c)
	call	hlhex
	ld	ix,mmu_pdr	; point to saved MMU registers
	ld	hl,mmsg2
	call	print
	call	sm3
	call	cr_lf
	call	sm1
	ld	hl,mmsg3
	call	print
	call	sm3
	call	cr_lf
sm1:	ld	b,8
sm2:	call	space
	djnz	sm2
sm3:	ld	b,8
sm4:	ldw	hl,(ix)
	call	hlhex
	call	space
	inc	ix
	inc	ix
	djnz	sm4
	ret

mmsg1:	db	CR,LF,'MMCR=',0
mmsg2:	db	CR,LF,'Usr PDR ',0
mmsg3:	db	CR,LF,'Sys PDR ',0

; /
; Show tasks

;	include	TCB.INC

tasks:	ld	hl,tmsg1
	call	print
	ld	ix,($RLIST##)
	call	prlist
	ld	hl,tmsg2
	call	print
	ld	ix,($WLIST##)
prlist:	ld	a,ixh
	or	ixl
	ret	z
	lda	hl,(ix+T.NAME)
	ld	b,6
pt1:	ld	a,(hl)
	call	mon_putch	; display task name
	inc	hl
	djnz	pt1
	call	space
	push	ix
	pop	hl
	call	hlhex
	call	space
	ld	a,(ix+T.ST)	; display status
	call	outbyte
	ld	a,(ix+T.ST+1)
	call	outbyte
	ldw	hl,(ix+T.PCB)
	ldw	hl,(hl+P.BASE)
	ld	a,h
	call	nhex		; display 12-bit bank
	ld	a,l
	call	outbyte
	ldw	hl,(ix+T.CTX)
	ldw	hl,(hl+TX.REGS+24)
	call	hlhex		; display PC
	call	cr_lf
	ld	ix,(ix+T.ACTL)	; next in active list
	jr	prlist		; loop

tmsg1:	db	CR,LF,'RLIST:',CR,LF,0
tmsg2:	db	'WLIST:',CR,LF,0

; get char from terminal

mon_getch:
	call	mon_status
	or	a
	jr	z,mon_getch
	in	a,(rdr)
	and	7Fh
	ret

; output char to terminal

mon_putch:
	push	af
	push	bc
	push	hl
	iopage	uartp
	pop	hl
	pop	bc
wait:	in	a,(tcs)
	rra
	jr	nc,wait
	pop	af
	out	(tdr),a
	ret

; check terminal input status

mon_status:
	push	bc
	push	hl
	iopage	uartp
	pop	hl
	pop	bc
	in	a,(rcs)
	and	10h
	ret	z
	or	0FFh		; return FF if ready, 0 otherwise
	ret

; save MMU page descriptor registers

savmmu:	iopage	MMUP
	xor	a
	out	(PDR),a
	ld	b,32
	ld	c,BMP
	ld	hl,mmu_pdr
	inirw
	ret

; restore MMU page descriptor registers

rstmmu:	iopage	MMUP
	xor	a
	out	(PDR),a
	ld	b,32
	ld	c,BMP
	ld	hl,mmu_pdr
	otirw
	ret

$DBMSG:	ex	(sp),hl
	push	af
	push	bc
	push	hl
	ld	c,IOP
	ldctl	hl,(c)
	ex	(sp),hl
dms0:	ld	a,(hl)
	inc	hl
	or	a
	jr	z,dms2
	call	mon_putch
	jr	dms0
dms2:	ex	(sp),hl
	ld	c,IOP
	ldctl	(c),hl
	pop	hl
	pop	bc
	pop	af
	ex	(sp),hl
	ret

;;	dseg

arg1:	ds	2		; first command argument
arg2:	ds	2		; second command argument
arg3:	ds	2		; third command argument
argset:	ds	1		; bit mask of specified arguments

lnbuf:	ds	30		; command line buffer
lnptr:	ds	2		; command line pointer

pgm_iop:ds	2		; saved user registers
pgm_usp:ds	2
pgm_iy:	ds	2
pgm_ix:	ds	2
pgm_hl1:ds	2
pgm_de1:ds	2
pgm_bc1:ds	2
pgm_af1:ds	2
pgm_hl:	ds	2
pgm_de:	ds	2
pgm_bc:	ds	2
pgm_af:	ds	2
reg_stack equ	$
pgm_sp:	ds	2
pgm_pc:	ds	2

mmu_pdr:ds	64

brkadr:	ds	2
brkval:	ds	1

	ds	48		; debugger stack
mon_stack:

