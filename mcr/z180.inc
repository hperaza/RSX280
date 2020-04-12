	.xlist

; Macros to define the Z-180 extended instructions.
;
; D-X Designs Pty. Ltd.  4/12/95  D. R. Brooks
;
; WARNING! These instructions are NOT supported by
;          Z80MU. However the IO instructions are
;          necessary to ensure correct access to
;          the internal machine registers, which
;          fully decode all 16 address bits.
;
; Adapted from Zilog (file 182macro.lib)
; Enhanced to provide conditional definition for the
;  following chips (define the appropriate symbol) 
; Z80180        Default
; Z80S180
; Z80181
; Z80182

; First, check only one CPU is defined
ifdef Z80180
?test	equ	0	;These will throw a "Multiple
endif			; Definition" error if 2 CPU's
ifdef Z80S180		; are defined
?test	equ	1
endif
ifdef Z80181
?test	equ	2
endif
ifdef Z80182
?test	equ	3
endif

;========== Z180 Internal Interrupt Vectors ========

; The following vectors are offsets from the value 
; loaded in IL, the Interrupt Vector Low register.

VINT1	equ	0	;External INT-1 pin
VINT2	equ	2	;External INT-2 pin
VPRT0	equ	4	;Timer 0
VPRT1	equ	6	;Timer 1
VDMA0	equ	8	;DMA Ch-0
VDMA1	equ	0ah	;DMA Ch-1
VCSIO	equ	0ch	;Clocked serial I/O
VASC0	equ	0eh	;Asynch. comms. Ch-0
VASC1	equ	10h	;Asynch. comms. Ch-1

;========== Z180 System Control Registers ==========

;NB These registers may be relocated to multiples of
; 40H, by setting the IO Control Register (ICR = 3FH)
; The addresses below are valid with ICR=0 (else they
; are offsets from the ICR base value).

;ASCI Registers
cntla0	equ	00h	;ASCI Control Reg A Ch0
cntla1	equ	01h	;ASCI Control Reg A Ch1
cntlb0	equ	02h	;ASCI Control Reg B Ch0
cntlb1	equ	03h	;ASCI Control Reg B Ch1
stat0	equ	04h	;ASCI Status Reg Ch0
stat1	equ	05h	;ASCI Status Reg Ch1
tdr0	equ	06h	;ASCI TX Data Reg Ch0
tdr1	equ	07h	;ASCI TX Data Reg Ch1
rdr0	equ	08h	;ASCI RX Data Reg Ch0
rdr1	equ	09h	;ASCI RX Data Reg Ch1
brk0	equ	12h	;Break Control Reg Ch0
brk1	equ	13h	;Break Control reg Ch1

;CSI/O Registers
cntr	equ	0ah	;CSI/O Control Reg
trdr	equ	0bh	;CSI/O TX/RX Data Reg

;Timer Registers
tmdr0l	equ	0ch	;Timer Data Reg Ch0-Low
tmdr0h	equ	0dh	;Timer Data Reg Ch0-High
rldr0l	equ	0eh	;Timer Reload Reg Ch0-Low
rldr0h	equ	0fh	;Timer Reload Reg Ch0-High
tcr	equ	10h	;Timer Control Reg
tmdr1l	equ	14h	;Timer Data Reg Ch1-Low
tmdr1h	equ	15h	;Timer Data Reg Ch1-High
rldr1l	equ	16h	;Timer Reload Reg Ch1-Low
rldr1h	equ	17h	;Timer Reload Reg Ch1-High
frc	equ	18h	;Free-Running Counter

ifdef	Z80S180
ccr	equ	1fh	;CPU Control Reg
endif

;DMA Registers
sar0l	equ	20h	;DMA Source Addr Reg Ch0-Low
sar0h	equ	21h	;DMA Source Addr Reg Ch0-High
sar0b	equ	22h	;DMA Source Addr Reg Ch0-B
dar0l	equ	23h	;DMA Destn  Addr Reg Ch0-Low
dar0h	equ	24h	;DMA Destn  Addr Reg Ch0-High
dar0b	equ	25h	;DMA Destn  Addr Reg Ch0-B
bcr0l	equ	26h	;DMA Byte Count Reg Ch0-Low
bcr0h	equ	27h	;DMA Byte Count Reg Ch0-High
mar1l	equ	28h	;DMA Memory Addr Reg Ch1-Low
mar1h	equ	29h	;DMA Memory Addr Reg Ch1-High
mar1b	equ	2ah	;DMA Memory Addr Reg Ch1-B
iar1l	equ	2bh	;DMA I/O Addr Reg Ch1-Low
iar1h	equ	2ch	;DMA I/O Addr Reg Ch1-High
bcr1l	equ	2eh	;DMA Byte Count Reg Ch1-Low
bcr1h	equ	2fh	;DMA Byte Count Reg Ch1-High
dstat	equ	30h	;DMA Status Reg
dmode	equ	31h	;DMA Mode Reg
dcntl	equ	32h	;DMA/WAIT Control Reg

;System Control Registers
il	equ	33h	;INT Vector Low Reg
itc	equ	34h	;INT/TRAP Control Reg
rcr	equ	36h	;Refresh Control Reg
cbr	equ	38h	;MMU Common Base Reg
bbr	equ	39h	;MMU Bank Base Reg
cbar	equ	3ah	;MMU Common/Bank Area Reg
omcr	equ	3eh	;Operation Mode Control Reg
icr	equ	3fh	;I/O Control Reg

ifdef Z80181		;Features unique to Z80181
	;Integral PIO device
p1ddr	equ	0e0h	;Data Direction Reg 1
p1dp	equ	0e1h	;Port 1 Data
p2ddr	equ	0e2h	;Data Direction Reg 2
p2dp	equ	0e3h	;Port 2 Data
	;Integral CTC device
ctc0	equ	0e4h	;CTC Channel 0
ctc1	equ	0e5h	;CTC Channel 1
ctc2	equ	0e6h	;CTC Channel 2
ctc3	equ	0e7h	;CTC Channel 3
scccr	equ	0e8h	;SCC Control Reg
sccdr	equ	0e9h	;SCC Data Reg
	;Chip-select output pins
ramubr	equ	0eah	;RAM Upper Boundary
ramlbr	equ	0ebh	;RAM Lower Boundary
rombr	equ	0ech	;ROM Boundary
scr	equ	0edh	;System Control Reg
endif

ifdef Z80182		;Features unique to Z80182
ccr	equ	1fh	;CPU control reg.
intype	equ	0dfh	;Interrupt edge/pin mux reg.
wsgcs	equ	0d8h	;Wait-State Generator CS
enh182	equ	0d9h	;Z80182 Enhancements Reg
pinmux	equ	0dfh	;Interrupt Edge/Pin Mux Reg
ramubr	equ	0e6h	;RAM End Boundary
ramlbr	equ	0e7h	;RAM Start Boundary
rombr	equ	0e8h	;ROM Boundary
fifoctl	equ	0e9h	;FIFO Control Reg
rtotc	equ	0eah	;RX Time-Out Time Const
ttotc	equ	0ebh	;TX Time-Out Time Const
fcr	equ	0ech	;FIFO Register
scr	equ	0efh	;System Pin Control
rbr	equ	0f0h	;MIMIC RX Buffer Reg
thr	equ	0f0h	;MIMIC TX Holding Reg
ier	equ	0f1h	;Interrupt Enable Reg
lcr	equ	0f3h	;Line Control Reg
mcr	equ	0f4h	;Modem Control Reg
lsr	equ	0f5h	;Line Status Reg
msr	equ	0f6h	;Modem Status Reg
mscr	equ	0f7h	;MIMIC Scratch Reg
dlatl	equ	0f8h	;Divisor latch LS
dlatm	equ	0f9h	;Divisor latch MS
ttcr	equ	0fah	;TX Time Constant
rtcr	equ	0fbh	;RX Time Constant
ivec	equ	0fch	;MIMIC Interrupt Vector
mimie	equ	0fdh	;MIMIC Interrupt Enable Reg
iusip	equ	0feh	;MIMIC Interrupt Under-Service Reg
mmcr	equ	0ffh	;MIMIC Master Control Reg

;Z80182 PIO Registers
ddra	equ	0edh	;Data Direction Reg A
dra	equ	0eeh	;Port A Data
ddrb	equ	0e4h	;Data Direction Reg B
drb	equ	0e5h	;Port B Data
ddrc	equ	0ddh	;Data Direction Reg C
drc	equ	0deh	;Port C data

;ESCC Registers
sccacnt	equ	0e0h	;ESCC Control Channel A
sccad	equ	0e1h	;ESCC Data Channel A
sccbcnt	equ	0e2h	;ESCC Control Channel B
sccbd	equ	0e3h	;ESCC Data Channel B
endif

;[E]SCC Internal Register Definitions
RR0	equ	00h
RR1	equ	01h
RR2	equ	02h
RR3	equ	03h
RR6	equ	06h
RR7	equ	07h
RR10	equ	0ah
RR12	equ	0ch
RR13	equ	0dh
RR15	equ	0fh

WR0	equ	00h
WR1	equ	01h
WR2	equ	02h
WR3	equ	03h
WR4	equ	04h
WR5	equ	05h
WR6	equ	06h
WR7	equ	07h
WR9	equ	09h
WR10	equ	0ah
WR11	equ	0bh
WR12	equ	0ch
WR13	equ	0dh
WR14	equ	0eh
WR15	equ	0fh


;==== Define the additional Z-180 instructions =====

	ifdef	Z180MACROS

;Machine registers (internal use only)
?b	equ	0
?c	equ	1
?d	equ	2
?e	equ	3
?h	equ	4
?l	equ	5
?a	equ	7

??bc	equ	0
??de	equ	1
??hl	equ	2
??sp	equ	3

slp	macro
	db	0edh
	db	 76h
	endm

mlt	macro	?r
	db	0edh
	db	 4ch+(??&?r AND 3) SHL 4
	endm

in0	macro	?r, ?p
	db	0edh
	db	0+(?&?r AND 7) SHL 3
	db	?p
	endm

out0	macro	?p, ?r
	db	0edh
	db	1+(?&?r AND 7) SHL 3
	db	?p
	endm

otim	macro
	db	0edh
	db	 83h
	endm

otimr	macro
	db	0edh
	db	 93h
	endm

otdm	macro
	db	0edh
	db	 8bh
	endm

otdmr	macro
	db	0edh
	db	 9bh
	endm

tstio	macro	?p
	db	0edh
	db	 74h
	db	?p
	endm

tst	macro	?r
	db	0edh
   ifidn   <?r>,<(hl)>
	db	 34h
   else
     ifdef   ?&?r
	db	4+(?&?r AND 7) SHL 3
     else
	db	 64h
	db	?r
     endif
   endif
	endm

	endif

	.list
