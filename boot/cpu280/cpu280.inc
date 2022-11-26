;***********************************************************************
;**  Port and Global declarations for CPU280 System software          **
;**  RSX280 version, adapted from 950126 Tilmann Reh by H.Peraza      **
;***********************************************************************

;============================================================
; I/O pages and Onboard I/O addresses:
;============================================================

busp	equ	0	; I/O Page externer Bus (ECB)
retip	equ	20h	; I/O Page Onboard & Bus-M1 (RETI)
boardp	equ	40h	; I/O Page On-Board-I/O

rtc	equ	0	; RTC base addresse RTC (64 addresses)
rtca	equ	rtc+10	; Register A
rtcb	equ	rtc+11	; Register B
rtcc	equ	rtc+12	; Register C
rtcd	equ	rtc+13	; Register D
reti1	equ	rtc+14	; NVRAM: 'Opcode' ED
reti2	equ	rtc+15	; NVRAM: 'Opcode' 4D
checklo	equ	rtc+16	; NVRAM: EPROM-Checksum Low-Byte
checkhi	equ	rtc+17	; NVRAM: EPROM-Checksum High-Byte
cpufreq	equ	rtc+18	; NVRAM: CPU freq (x 614400 Hz)

s$drv0	equ	rtc+19	; Setup: Drive A
s$drv1	equ	rtc+20	; Setup: Drive B
s$drv2	equ	rtc+21	; Setup: Drive C
s$drv3	equ	rtc+22	; Setup: Drive D
s$c1h	equ	rtc+23	; Setup: CRT1 hardware param
s$c1bd	equ	rtc+24	; Setup: CRT1 baud rate
s$c2h	equ	rtc+25	; Setup: CRT2 hardware param
s$c2bd	equ	rtc+26	; Setup: CRT2 baud rate
s$ci	equ	rtc+27	; Setup: CI: Device Nummer
s$co	equ	rtc+28	; Setup: CO: Device Nummer
s$ai	equ	rtc+29	; Setup: AI: Device Nummer
s$ao	equ	rtc+30	; Setup: AO: Device Nummer
s$lst	equ	rtc+31	; Setup: LST: Device Nummer
s$mem	equ	rtc+32	; Setup/Hardware test: memory size (* 512k)
s$dse	equ	rtc+33	; Setup: Summer Time
s$bdrv	equ	rtc+34	; Setup: Boot drive
s$chain	equ	rtc+35	; Setup: Drive Search Chain (4 Byte)
			; Next free NVRAM address: rtc+39

fdcsta	equ	40h	; FDC Master Status Register
fdcdat	equ	41h	; FDC Data (& Command) Register
fdcdack	equ	60h	; FDC DMA Acknowledge Port
fdcldor	equ	80h	; FDC Load Operations Register
fdcldcr	equ	0A0h	; FDC Load Control Register
uartdat	equ	0C0h	; TP-UART Data Register
uartctl	equ	0C1h	; TP-UART Control/Status Register
gpi	equ	0E0h	; GP Input

rts1_en	equ	gpi+0	; RTS1 bit output for V.24
rts1_di	equ	gpi+1
drv_0	equ	gpi+2	; DRV bit output for FDC
drv_1	equ	gpi+3
tc_0	equ	gpi+4	; TC bit output for FDC
tc_1	equ	gpi+5
mot_off	equ	gpi+6	; Motor-On bit output for FDD's
mot_on	equ	gpi+7
warm_0	equ	gpi+8	; Warm start recognized flag
warm_1	equ	gpi+9
ld1_on	equ	gpi+10	; LED 1
ld1_off	equ	gpi+11
ld2_on	equ	gpi+12	; LED 2
ld2_off	equ	gpi+13
ld3_on	equ	gpi+14	; LED 3
ld3_off	equ	gpi+15

b$uj1	equ	0	; User Jumper 1
b$uj2	equ	1	; User Jumper 2
b$uj3	equ	2	; User Jumper 3
b$cts1	equ	3	; GPI: Bit CTS1
b$sqw	equ	4	; GPI: Bit SQW
b$warm	equ	5	; GPI: Bit WARM

; Bit masks for Master Status Register and Interrupt Status Register

i$a	equ	1	; Bit number: INTA interrupt
i$ct0	equ	2	; Bit number: CT0 and DMA0 interrupt
i$b	equ	4	; Bit number: INTB Interrupt
i$rx	equ	8	; Bit number: CT1, UART Rx and DMA1 interrupt
i$c	equ	10h	; Bit number: INTC interrupt
i$tx	equ	20h	; Bit number: UART Tx und DMA2 interrupt
i$dma3	equ	40h	; Bit number: CT2 und DMA3 interrupt

