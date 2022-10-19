$ALLOC	equ	01376h	; allocate pool memory
$ALUN	equ	00FB1h	; assign LUN to device
$ASGN	equ	00E24h	; assign logical name to physical device
$ASNTP	equ	021FBh	; assign next task to partition
$AVAIL	equ	0145Fh	; return available pool memory
$CHKPT	equ	02398h	; initiate task checkpoint
$CLASN	equ	00E96h	; delete all logical dev assignments of the given type
$CLIST	equ	050A7h	; list of CLI control blocks
$CLKQ	equ	050A5h	; ptr to head of clock queue
$CONN	equ	01100h	; connect to task
$CPCB	equ	050A3h	; address of PCB of current task
$CPLST	equ	05099h	; ptr to head of checkpoint PCB list
$CTCB	equ	050A1h	; address of TCB of current task
$DBMON	equ	0F406h	; debugger-monitor main entry point
$DBTRP	equ	0F65Dh	; debugger-monitor bpt entry
$DEASN	equ	00E83h	; delete logical device assignment
$DMTRP	equ	0F6D7h	; debugger-monitor bpt entry with message
$ERSEQ	equ	050B4h	; error sequence number
$FEAT	equ	05081h	; feature bits
$FILBF	equ	050D3h	; ptr to file I/O buffer area
$FIXTK	equ	02EE9h	; fix task in memory
$FNDEV	equ	00ED5h	; find device and get UCB address
$FNPAR	equ	021AEh	; find partition by name
$FNTCB	equ	00A7Dh	; find TCB by name
$FNTSK	equ	00A55h	; find task by name
$FORKQ	equ	050BFh	; pointer to list of fork blocks
$FREE	equ	013D5h	; free pool memory
$GTUCB	equ	00F9Dh	; get UCB address
$HOSTN	equ	05082h	; host/node name
$IOEND	equ	01825h	; end I/O processing for ASTs
$LDCKR	equ	00542h	; resume task after checkpoint read
$LDCKW	equ	00572h	; queue task to partition after chkpnt write
$LDGO	equ	004B4h	; start task after load
$LGOUT	equ	010D1h	; logout user
$LKTCB	equ	0037Ah	; link TCB to the specified list
$LOGDV	equ	05095h	; ptr to head of logical device assignment list
$LOGIN	equ	0106Dh	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	05091h	; total memory size
$MFLGS	equ	050BEh	; MCR flags
$MVTBL	equ	050C1h	; ptr to head of mounted device list
$PHYDV	equ	05093h	; ptr to head of physical device list
$PLIST	equ	05097h	; ptr to list of partitions
$POLSZ	equ	0508Fh	; size of pool area
$POOL	equ	0508Dh	; ptr to start of pool area
$REDIR	equ	00F20h	; follow UCB redirection links
$REMTK	equ	02EE0h	; remove task from STD
$RLIST	equ	0509Dh	; ptr to head of ready tasks list
$RNDC	equ	050A9h	; round-robin interval in ticks
$RNDH	equ	050ABh	; highest priority for round-robin scheduling
$RNDL	equ	050ACh	; lowest priority for round-robin scheduling
$SBANK	equ	0F3BFh	; system bank number
$SCRBF	equ	050C3h	; scratch buffer address
$SWPC	equ	050AEh	; swapping interval in ticks
$SWPCT	equ	050AFh	; swapping interval counter
$SWPRI	equ	050ADh	; priority range for swapping
$SWSTK	equ	050D5h	; stub to call kernel routines
$TLIST	equ	0509Bh	; ptr to head of installed tasks list
$UBANK	equ	0F3C1h	; bank of current task
$UCBDN	equ	01059h	; get device name and unit from UCB
$UGETB	equ	0F36Fh	; inter-bank get byte routine
$UGETW	equ	0F38Dh	; inter-bank get word routine
$UNFTK	equ	02EF2h	; unfix task
$UPUTB	equ	0F37Dh	; inter-bank put byte routine
$UPUTW	equ	0F39Fh	; inter-bank put word routine
$VALBF	equ	01A1Eh	; validate buffer (current task)
$VALTB	equ	01A01h	; validate buffer (arbitrary task)
$WLIST	equ	0509Fh	; ptr to head of waiting tasks list
IODONE	equ	01808h	; end I/O processing
LDRTCB	equ	050B2h	; LDR... TCB
MCRTCB	equ	050B0h	; MCR... TCB
NVOK	equ	047E6h	; P112 NV RAM OK flag
NVRAM	equ	047CEh	; P112 NV RAM contents
SETFLG	equ	00BCFh	; set task's event flag
SFLAGS	equ	050B6h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	05078h	; start of system data area
SYSEND	equ	0514Eh	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	0508Bh	; top of pool area
SYSTYP	equ	05080h	; system type
SYSVER	equ	0507Eh	; system version number
TICCNT	equ	0F3B4h	; absolute time count in ticks
TICSEC	equ	0F3B3h	; ticks per second
TKILL	equ	006C7h	; abort task
UPTIME	equ	0F3B8h	; system up time (ticks and seconds)
