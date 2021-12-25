$ALLOC	equ	0190Bh	; allocate pool memory
$ALUN	equ	01557h	; assign LUN to device
$ASGN	equ	013CAh	; assign logical name to physical device
$ASNTP	equ	026F7h	; assign next task to partition
$CHKPT	equ	02894h	; initiate task checkpoint
$CLASN	equ	0143Ch	; delete all logical dev assignments of the given type
$CLIST	equ	0535Ah	; list of CLI control blocks
$CLKQ	equ	05358h	; ptr to head of clock queue
$CONN	equ	016A6h	; connect to task
$CPCB	equ	05356h	; address of PCB of current task
$CPLST	equ	0534Ch	; ptr to head of checkpoint PCB list
$CTCB	equ	05354h	; address of TCB of current task
$DBMON	equ	0F406h	; debugger-monitor main entry point
$DBTRP	equ	0F65Dh	; debugger-monitor bpt entry
$DEASN	equ	01429h	; delete logical device assignment
$DMTRP	equ	0F6D7h	; debugger-monitor bpt entry with message
$ERSEQ	equ	05367h	; error sequence number
$FEAT	equ	05334h	; feature bits
$FILBF	equ	05386h	; ptr to file I/O buffer area
$FIXTK	equ	03331h	; fix task in memory
$FNDEV	equ	0147Bh	; find device and get UCB address
$FNPAR	equ	026ABh	; find partition by name
$FNTCB	equ	00FE8h	; find TCB by name
$FNTSK	equ	00FC0h	; find task by name
$FORKQ	equ	05372h	; pointer to list of fork blocks
$FREE	equ	0196Ah	; free pool memory
$GTUCB	equ	01543h	; get UCB address
$HOSTN	equ	05335h	; host/node name
$IOEND	equ	01D4Ch	; end I/O processing for ASTs
$LDCKR	equ	00A88h	; resume task after checkpoint read
$LDCKW	equ	00AB8h	; queue task to partition after chkpnt write
$LDGO	equ	009FAh	; start task after load
$LGOUT	equ	01677h	; logout user
$LKTCB	equ	008C0h	; link TCB to the specified list
$LOGDV	equ	05348h	; ptr to head of logical device assignment list
$LOGIN	equ	01613h	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	05344h	; total memory size
$MFLGS	equ	05371h	; MCR flags
$MVTBL	equ	05374h	; ptr to head of mounted device list
$PHYDV	equ	05346h	; ptr to head of physical device list
$PLIST	equ	0534Ah	; ptr to list of partitions
$POLSZ	equ	05342h	; size of pool area
$POOL	equ	05340h	; ptr to start of pool area
$REDIR	equ	014C6h	; follow UCB redirection links
$REMTK	equ	03328h	; remove task from STD
$RLIST	equ	05350h	; ptr to head of ready tasks list
$RNDC	equ	0535Ch	; round-robin interval in ticks
$RNDH	equ	0535Eh	; highest priority for round-robin scheduling
$RNDL	equ	0535Fh	; lowest priority for round-robin scheduling
$SBANK	equ	0F3BFh	; system bank number
$SCRBF	equ	05376h	; scratch buffer address
$SWPC	equ	05361h	; swapping interval in ticks
$SWPCT	equ	05362h	; swapping interval counter
$SWPRI	equ	05360h	; priority range for swapping
$SWSTK	equ	05388h	; stub to call kernel routines
$TLIST	equ	0534Eh	; ptr to head of installed tasks list
$UBANK	equ	0F3C1h	; bank of current task
$UCBDN	equ	015FFh	; get device name and unit from UCB
$UGETB	equ	0F36Fh	; inter-bank get byte routine
$UGETW	equ	0F38Dh	; inter-bank get word routine
$UNFTK	equ	0333Ah	; unfix task
$UPUTB	equ	0F37Dh	; inter-bank put byte routine
$UPUTW	equ	0F39Fh	; inter-bank put word routine
$WLIST	equ	05352h	; ptr to head of waiting tasks list
IODONE	equ	01D2Fh	; end I/O processing
LDRTCB	equ	05365h	; LDR... TCB
MCRTCB	equ	05363h	; MCR... TCB
NVOK	equ	04AF4h	; P112 NV RAM OK flag
NVRAM	equ	04ADCh	; P112 NV RAM contents
SETFLG	equ	0114Dh	; set task's event flag
SFLAGS	equ	05369h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	0532Bh	; start of system data area
SYSEND	equ	05401h	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	0533Eh	; top of pool area
SYSTYP	equ	05333h	; system type
SYSVER	equ	05331h	; system version number
TICCNT	equ	0F3B4h	; absolute time count in ticks
TICSEC	equ	0F3B3h	; ticks per second
TKILL	equ	00C32h	; abort task
UPTIME	equ	0F3B8h	; system up time (ticks and seconds)
