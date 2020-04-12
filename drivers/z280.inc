;==================================================================
;
;	This file contains all of the EQUals for the internal
;	workings of the Z280 mpu.
;
;	Written by Daniel A. Jacobs	Mar. 13 1988
;
;==================================================================

; Master Control Register (MCR) equals:	 Z280 manual	Page 5-77

MSR	EQU	0	; Master Status Register	Page 3-4
ISR	EQU	16H	; Interrupt Status Reg.		Page 3-4 & 3-5
ITV	EQU	6	; Interrupt/Trap Vector
			;   table pointer		Page 3-5 & 6-11
IOP	EQU	8	; I/O Page register	(8bit)	Page 3-5
BTI	EQU	0FFH	; Bus Timing and
			;   Initialization reg.	(8bit)	Page 3-1 & 3-2
BTC	EQU	2	; Bus Timing and
			;   Control register	(8bit)	Page 3-2
SLR	EQU	4	; Stack Limit Register		Page 3-6
TCR	EQU	10H	; Trap Control Reg.	(8bit)	Page 3-5 & 3-6
CCR	EQU	12H	; Cache Control Reg.	(8bit)	Page 3-3 & 3-4
LAR	EQU	14H	; Local (global)
			;   Address Register	(8bit)	Page 3-3

;----------------------------------------------------------------------

; Memory Management Unit (MMU) equals:	 Z280 manual	Page 7-6

MMUP	EQU	0FFH	; MMU Page I/O
MMCR	EQU	0F0H	; MMU Master Control Reg.	Page 7-5
PDR	EQU	0F1H	; Page Descripter Reg. pointer	Page 7-2 & 7-5
DSP	EQU	0F5H	; Descriptor Select Port	Page 7-6
BMP	EQU	0F4H	; Block Move Port		Page 7-6
IP	EQU	0F2H	; Invalidation Port		Page 7-6

;---------------------------------------------------------------------

; Refresh Rate Register (RRR) equals:	Z280 manual	Page 9-1

RRRP	EQU	0FFH	; RRR Page I/O
RRR	EQU	0E8H	; Refresh Rate Register		Page 9-1

;---------------------------------------------------------------------

; Counter Timers (CT) equals:		Z280 manual	Page 9-7

CTP	EQU	0FEH	; CT Page I/O

CR0	EQU	0E0H	; Configuration reg. for CT/0	Page 9-5
CR1	EQU	0E8H	; Configuration reg. for CT/1	Page 9-5
CR2	EQU	0F8H	; Configuration reg. for CT/2	Page 9-5

CS0	EQU	0E1H	; Command Status reg for CT/0	Page 9-6
CS1	EQU	0E9H	; Command Status reg for CT/1	Page 9-6
CS2	EQU	0F9H	; Command Status reg for CT/2	Page 9-6

TC0	EQU	0E2H	; Time Constant reg. for CT/0	Page 9-6
TC1	EQU	0EAH	; Time Constant reg. for CT/1	Page 9-6
TC2	EQU	0FAH	; Time Constant reg. for CT/2	Page 9-6

CT0	EQU	0E3H	; Count Time reg.    for CT/0	Page 9-6
CT1	EQU	0EBH	; Count Time reg.    for CT/1	Page 9-6
CT2	EQU	0FBH	; Count Time reg.    for CT/2	Page 9-6

;---------------------------------------------------------------------

; Direct Memory Access (DMA) equals:	Z280 manual	Page 9-15

DMAP	EQU	0FFH	; DMA Page I/O
DMCR	EQU	1FH	; DMA Master Control Register	Page 9-13 & 9-14

DAL0	EQU	0	; Destination Address Low DMA0	Page 9-14 & 9-15
DAL1	EQU	8	; Destination Address Low DMA1	Page 9-14 & 9-15
DAL2	EQU	10H	; Destination Address Low DMA2	Page 9-14 & 9-15
DAL3	EQU	18H	; Destination Address Low DMA3	Page 9-14 & 9-15

DAH0	EQU	1	; Destination Address High DMA0	Page 9-14 & 9-15
DAH1	EQU	9	; Destination Address High DMA1	Page 9-14 & 9-15
DAH2	EQU	11H	; Destination Address High DMA2	Page 9-14 & 9-15
DAH3	EQU	19H	; Destination Address High DMA3	Page 9-14 & 9-15

SAL0	EQU	2	; Source Address Low DMA0	Page 9-14 & 9-15
SAL1	EQU	0AH	; Source Address Low DMA1	Page 9-14 & 9-15
SAL2	EQU	12H	; Source Address Low DMA2	Page 9-14 & 9-15
SAL3	EQU	1AH	; Source Address Low DMA3	Page 9-14 & 9-15

SAH0	EQU	3	; Source Address High DMA0	Page 9-14 & 9-15
SAH1	EQU	0BH	; Source Address High DMA1	Page 9-14 & 9-15
SAH2	EQU	13H	; Source Address High DMA2	Page 9-14 & 9-15
SAH3	EQU	1BH	; Source Address High DMA3	Page 9-14 & 9-15

DCR0	EQU	4	; Counter Register for DMA0	Page 9-14
DCR1	EQU	0CH	; Counter Register for DMA1	Page 9-14
DCR2	EQU	14H	; Counter Register for DMA2	Page 9-14
DCR3	EQU	1CH	; Counter Register for DMA3	Page 9-14

TDR0	EQU	5	; Transaction Descriptor DMA0	Page 9-13 & 9-14
TDR1	EQU	0DH	; Transaction Descriptor DMA1	Page 9-13 & 9-14
TDR2	EQU	15H	; Transaction Descriptor DMA2	Page 9-13 & 9-14
TDR3	EQU	1DH	; Transaction Descriptor DMA3	Page 9-13 & 9-14

;----------------------------------------------------------------------

; Universal Asynchronous Receiver/Transmitter (UART) equals:   Page 9-20

UARTP	EQU	0FEH	; UART Page I/O
UCR	EQU	10H	; UART Configuration Reg.	Page 9-18
TCS	EQU	12H	; Transmitter Control/Status	Page 9-19
RCS	EQU	14H	; Receiver Control/Status reg.	Page 9-20
RDR	EQU	16H	; Receiver Data Register
TDR	EQU	18H	; Transmitter Data Register

;======================================================================

IOPAGE	MACRO	page
	ld	c,IOP
	ld	l,page
	ldctl	(c),hl
	ENDM

OUTJMP	MACRO
	IF	1
	NOP
	NOP
	NOP
	NOP
	ENDIF
	ENDM

