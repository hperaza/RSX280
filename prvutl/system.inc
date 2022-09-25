$ALLOC	equ	01892h	; allocate pool memory
$ALUN	equ	014DEh	; assign LUN to device
$ASGN	equ	01351h	; assign logical name to physical device
$ASNTP	equ	02717h	; assign next task to partition
$CHKPT	equ	028B4h	; initiate task checkpoint
$CLASN	equ	013C3h	; delete all logical dev assignments of the given type
$CLIST	equ	055DEh	; list of CLI control blocks
$CLKQ	equ	055DCh	; ptr to head of clock queue
$CONN	equ	0162Dh	; connect to task
$CPCB	equ	055DAh	; address of PCB of current task
$CPLST	equ	055D0h	; ptr to head of checkpoint PCB list
$CTCB	equ	055D8h	; address of TCB of current task
$DBMON	equ	0F406h	; debugger-monitor main entry point
$DBTRP	equ	0F65Dh	; debugger-monitor bpt entry
$DEASN	equ	013B0h	; delete logical device assignment
$DMTRP	equ	0F6D7h	; debugger-monitor bpt entry with message
$ERSEQ	equ	055EBh	; error sequence number
$FEAT	equ	055B8h	; feature bits
$FILBF	equ	0560Ah	; ptr to file I/O buffer area
$FIXTK	equ	03405h	; fix task in memory
$FNDEV	equ	01402h	; find device and get UCB address
$FNPAR	equ	026CAh	; find partition by name
$FNTCB	equ	00FAAh	; find TCB by name
$FNTSK	equ	00F82h	; find task by name
$FORKQ	equ	055F6h	; pointer to list of fork blocks
$FREE	equ	018F1h	; free pool memory
$GTUCB	equ	014CAh	; get UCB address
$HOSTN	equ	055B9h	; host/node name
$IOEND	equ	01D41h	; end I/O processing for ASTs
$LDCKR	equ	00A6Fh	; resume task after checkpoint read
$LDCKW	equ	00A9Fh	; queue task to partition after chkpnt write
$LDGO	equ	009E1h	; start task after load
$LGOUT	equ	015FEh	; logout user
$LKTCB	equ	008A7h	; link TCB to the specified list
$LOGDV	equ	055CCh	; ptr to head of logical device assignment list
$LOGIN	equ	0159Ah	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	055C8h	; total memory size
$MFLGS	equ	055F5h	; MCR flags
$MVTBL	equ	055F8h	; ptr to head of mounted device list
$PHYDV	equ	055CAh	; ptr to head of physical device list
$PLIST	equ	055CEh	; ptr to list of partitions
$POLSZ	equ	055C6h	; size of pool area
$POOL	equ	055C4h	; ptr to start of pool area
$REDIR	equ	0144Dh	; follow UCB redirection links
$REMTK	equ	033FCh	; remove task from STD
$RLIST	equ	055D4h	; ptr to head of ready tasks list
$RNDC	equ	055E0h	; round-robin interval in ticks
$RNDH	equ	055E2h	; highest priority for round-robin scheduling
$RNDL	equ	055E3h	; lowest priority for round-robin scheduling
$SBANK	equ	0F3BFh	; system bank number
$SCRBF	equ	055FAh	; scratch buffer address
$SWPC	equ	055E5h	; swapping interval in ticks
$SWPCT	equ	055E6h	; swapping interval counter
$SWPRI	equ	055E4h	; priority range for swapping
$SWSTK	equ	0560Ch	; stub to call kernel routines
$TLIST	equ	055D2h	; ptr to head of installed tasks list
$UBANK	equ	0F3C1h	; bank of current task
$UCBDN	equ	01586h	; get device name and unit from UCB
$UGETB	equ	0F36Fh	; inter-bank get byte routine
$UGETW	equ	0F38Dh	; inter-bank get word routine
$UNFTK	equ	0340Eh	; unfix task
$UPUTB	equ	0F37Dh	; inter-bank put byte routine
$UPUTW	equ	0F39Fh	; inter-bank put word routine
$VALBF	equ	01F3Ah	; validate buffer (current task)
$VALTB	equ	01F1Dh	; validate buffer (arbitrary task)
$WLIST	equ	055D6h	; ptr to head of waiting tasks list
IODONE	equ	01D24h	; end I/O processing
LDRTCB	equ	055E9h	; LDR... TCB
MCRTCB	equ	055E7h	; MCR... TCB
NVOK	equ	04D1Dh	; P112 NV RAM OK flag
NVRAM	equ	04D05h	; P112 NV RAM contents
SETFLG	equ	010FCh	; set task's event flag
SFLAGS	equ	055EDh	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	055AFh	; start of system data area
SYSEND	equ	05685h	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	055C2h	; top of pool area
SYSTYP	equ	055B7h	; system type
SYSVER	equ	055B5h	; system version number
TICCNT	equ	0F3B4h	; absolute time count in ticks
TICSEC	equ	0F3B3h	; ticks per second
TKILL	equ	00BF4h	; abort task
UPTIME	equ	0F3B8h	; system up time (ticks and seconds)
