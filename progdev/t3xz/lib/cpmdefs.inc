; Z80 Program Library
; Nils M Holm, 2019
; In the public domain

; CP/NC system definitions, mostly identical to CP/M 2.2

WARMBOOT	equ	0	; Jump here to reboot
BDOS		equ	5	; BDOS entry point

TOPRAM		equ	BDOS+2	; First page past TPA

FCB1		equ	5Ch	; File Control Blocks
FCB2		equ	6Ch

; FCB Fields

FCB_DRIVE	equ	 0	; Drive ID, 0=default, 1=A:, ...
FCB_NAME	equ	 1	; File name
FCB_TYPE	equ	 9	; File type
FCB_EXTENT	equ	12	; Extent number
FCB_USER	equ	13	; User number
;		equ	14	; --reserved--
FCB_RECORDS	equ	15	; Number of records in this extent
FCB_BLOCKS	equ	16	; Allocation blocks used
FCB_SEQREC	equ	32	; Sequential record
FCB_RRLOW	equ	33	; Random record
FCB_RRHIGH	equ	34	; Random record, high byte
FCB_RROVFL	equ	35	; Random record, overflow

FCBSIZE		equ	36	; FCB size

COMTAIL		equ	80h	; Command Tail

DMABUF		equ	80h	; Disk I/O (DMA) Buffer

IOBYTE		equ	0003h	; IOBYTE location

IOCONMASK	equ	 03h	; IOBYTE, Console Mask
IOAUXMASK	equ	00Ch	; IOBYTE, Auxiliary Device Mask
IOLSTMASK	equ	0C0h	; IOBYTE, List Device Mask

IOCONTTY	equ	  0	; CON: -> TTY:
IOCONCRT	equ	  1	; CON: -> CRT:
IOCONLPT	equ	  2	; CON: -> LPT: (copy)

IOAUXTTY	equ	  0	; AUX: -> TTY:
IOAUXCRT	equ	  4	; AUX: -> CRT:
IOAUXLPT	equ	  8	; AUX: -> LPT:

IOLSTTTY	equ	  0	; LST: -> TTY:
IOLSTCRT	equ	 40h	; LST: -> CRT:
IOLSTLPT	equ	 80h	; LST: -> LPT:

BDSYSRESET	equ	0	; System Reset (Warm Boot)
BDCONIN		equ	1	; Console Input, A = byte from CON:
BDCONOUT	equ	2	; Console Output, E = byte to write to CON:
BDAUXIN		equ	3	; Auxiliary Input, A = byte from AUX:
BDAUXOUT	equ	4	; Auxiliary Output, E = byte to write to AUX:
BDLSTOUT	equ	5	; List device Output, E = byte to write to LST:
BDDCONIO	equ	6	; Direct Console I/O
				; E=FF: A=byte from CON: or 0 if no input
				; E=/=FF: write to CON:
BDAUXINST	equ	7	; Get AUX: Input Status, A=FF: ready, else A=0
BDAUXOUTST	equ	8	; Get AUX: Output Status, A=FF: ready, else A=0
BDPRINTS	equ	9	; Print Console String
				; DE = '$'-terminated string
BDREADCSTR	equ	10	; Read Console String
				; DE = buffer address, first byte = maximum
				; bytes to read, second byte: bytes actually
				; read upon return
BDCONST		equ	11	; Console Status, A=0, if no incoming data
				;                 A=FF, if input ready
BDGETVER	equ	12	; Get Version, HL = version
				; H=1: MP/M, H=0: CP/M, L=0: CP/M 1.x,
				; else L=XXXXYYYY: CP/M XXXX.YYYY
BDDSKRESET	equ	13	; Disk System Reset
BDSELDSK	equ	14	; Select Disk, E=0: A:, etc
BDOPEN		equ	15	; Open File, DE=addr of FCB, A=FF: failure
BDCLOSE		equ	16	; Close File, DE=addr of FCB, A=FF: failure
BDSEARCH	equ	17	; Search First Match, DE=FCB, A=FF: failure
BDSRCHNEXT	equ	18	; Search Next Match, A=FF: failure
BDERASE		equ	19	; Erase File, DE=FCB, A=FF: failure
BDREADSEQ	equ	20	; Read Sequential, DE=FCB, A=/=0: failure
BDWRITESEQ	equ	21	; Write Sequential, DE=FCB, A=/=0: failure
BDCREATE	equ	22	; Create File, DE=FCB, A=FF: failure
BDRENAME	equ	23	; Rename File, DE=FCB, A=FF: failure
BDGETLOGVEC	equ	24	; Get Login vector (act. disks), HL = disk map
BDGETCURDSK	equ	25	; Get Current Disk, A = disk, 0=A:, etc
BDSETDMA	equ	26	; Set Disk I/O (DMA) Address, DE = addr
BDGETALVEC	equ	27	; Get Allocation Vector, HL = addr
BDSETDSKRO	equ	28	; Set Disk Read-Only, none
BDGETRODSK	equ	29	; Get Read-Only Disks, HL = disk map
BDSETFAT	equ	30	; Set File Attributes, DE=FCB, A=0: failure
BDGETDPB	equ	31	; Get Disk Parameter Block, HL = addr
BDGETSETUSR	equ	32	; Get/Set User, E=FF: get user,
				; E=/=FF: set user
BDREADRAN	equ	33	; Read Random, DE=FCB, A=0: success
BDWRITERAN	equ	34	; Write Random, DE=FCB
BDGETFSIZ	equ	35	; Get File Size, DE=FCB,
				; return in random record FCB fields
BDSETRANREC	equ	36	; Set Random Record, DE=FCB, see 35
BDRESETDSKS	equ	37	; Reset Disks, DE = disk map
BDWRITERANZ	equ	40	; Write Random with Zero-Fill, DE=FCB, see 35

BDDSKFREE	equ	46	; Get Disk Free Space, result in DMA 0-2

BDABSREAD	equ	192	; Read Absolute Record, DE = record
BDABSWRITE	equ	193	; Write Absolute Record, DE = record
BDSETDRIVE	equ	194	; Select Drive, E = drive (0 = A:, etc)
