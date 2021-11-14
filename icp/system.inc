$ALLOC	equ	018D6h	; allocate pool memory
$ALUN	equ	01526h	; assign LUN to device
$ASGN	equ	01399h	; assign logical name to physical device
$ASNTP	equ	026C2h	; assign next task to partition
$CHKPT	equ	0285Fh	; initiate task checkpoint
$CLASN	equ	0140Bh	; delete all logical dev assignments of the given type
$CLIST	equ	0530Dh	; list of CLI control blocks
$CLKQ	equ	0530Bh	; ptr to head of clock queue
$CONN	equ	01671h	; connect to task
$CPCB	equ	05309h	; address of PCB of current task
$CPLST	equ	052FFh	; ptr to head of checkpoint PCB list
$CTCB	equ	05307h	; address of TCB of current task
$DBMON	equ	0F406h	; debugger-monitor main entry point
$DBTRP	equ	0F65Dh	; debugger-monitor bpt entry
$DEASN	equ	013F8h	; delete logical device assignment
$DMTRP	equ	0F6D7h	; debugger-monitor bpt entry with message
$ERSEQ	equ	0531Ah	; error sequence number
$FEAT	equ	052E7h	; feature bits
$FILBF	equ	05339h	; ptr to file I/O buffer area
$FIXTK	equ	032EAh	; fix task in memory
$FNDEV	equ	0144Ah	; find device and get UCB address
$FNPAR	equ	02676h	; find partition by name
$FNTCB	equ	00FB7h	; find TCB by name
$FNTSK	equ	00F8Fh	; find task by name
$FORKQ	equ	05325h	; pointer to list of fork blocks
$FREE	equ	01935h	; free pool memory
$GTUCB	equ	01512h	; get UCB address
$HOSTN	equ	052E8h	; host/node name
$IOEND	equ	01D17h	; end I/O processing for ASTs
$LDCKR	equ	00A57h	; resume task after checkpoint read
$LDCKW	equ	00A87h	; queue task to partition after chkpnt write
$LDGO	equ	009D4h	; start task after load
$LGOUT	equ	01642h	; logout user
$LKTCB	equ	0089Ah	; link TCB to the specified list
$LOGDV	equ	052FBh	; ptr to head of logical device assignment list
$LOGIN	equ	015E2h	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	052F7h	; total memory size
$MFLGS	equ	05324h	; MCR flags
$MVTBL	equ	05327h	; ptr to head of mounted device list
$PHYDV	equ	052F9h	; ptr to head of physical device list
$PLIST	equ	052FDh	; ptr to list of partitions
$POLSZ	equ	052F5h	; size of pool area
$POOL	equ	052F3h	; ptr to start of pool area
$REDIR	equ	01495h	; follow UCB redirection links
$REMTK	equ	032E1h	; remove task from STD
$RLIST	equ	05303h	; ptr to head of ready tasks list
$RNDC	equ	0530Fh	; round-robin interval in ticks
$RNDH	equ	05311h	; highest priority for round-robin scheduling
$RNDL	equ	05312h	; lowest priority for round-robin scheduling
$SBANK	equ	0F3BFh	; system bank number
$SCRBF	equ	05329h	; scratch buffer address
$SWPC	equ	05314h	; swapping interval in ticks
$SWPCT	equ	05315h	; swapping interval counter
$SWPRI	equ	05313h	; priority range for swapping
$SWSTK	equ	0533Bh	; stub to call kernel routines
$TLIST	equ	05301h	; ptr to head of installed tasks list
$UBANK	equ	0F3C1h	; bank of current task
$UCBDN	equ	015CEh	; get device name and unit from UCB
$UGETB	equ	0F36Fh	; inter-bank get byte routine
$UGETW	equ	0F38Dh	; inter-bank get word routine
$UNFTK	equ	032F3h	; unfix task
$UPUTB	equ	0F37Dh	; inter-bank put byte routine
$UPUTW	equ	0F39Fh	; inter-bank put word routine
$WLIST	equ	05305h	; ptr to head of waiting tasks list
IODONE	equ	01CFAh	; end I/O processing
LDRTCB	equ	05318h	; LDR... TCB
MCRTCB	equ	05316h	; MCR... TCB
NVOK	equ	04AA7h	; P112 NV RAM OK flag
NVRAM	equ	04A8Fh	; P112 NV RAM contents
SETFLG	equ	0111Ch	; set task's event flag
SFLAGS	equ	0531Ch	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	052DEh	; start of system data area
SYSEND	equ	053B4h	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	052F1h	; top of pool area
SYSTYP	equ	052E6h	; system type
SYSVER	equ	052E4h	; system version number
TICCNT	equ	0F3B4h	; absolute time count in ticks
TICSEC	equ	0F3B3h	; ticks per second
TKILL	equ	00C01h	; abort task
UPTIME	equ	0F3B8h	; system up time (ticks and seconds)
