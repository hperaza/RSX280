$ALLOC	equ	018D6h	; allocate pool memory
$ALUN	equ	01526h	; assign LUN to device
$ASGN	equ	01399h	; assign logical name to physical device
$ASNTP	equ	026C2h	; assign next task to partition
$CHKPT	equ	0285Fh	; initiate task checkpoint
$CLASN	equ	0140Bh	; delete all logical dev assignments of the given type
$CLIST	equ	052A3h	; list of CLI control blocks
$CLKQ	equ	052A1h	; ptr to head of clock queue
$CONN	equ	01671h	; connect to task
$CPCB	equ	0529Fh	; address of PCB of current task
$CPLST	equ	05295h	; ptr to head of checkpoint PCB list
$CTCB	equ	0529Dh	; address of TCB of current task
$DBMON	equ	0F406h	; debugger-monitor main entry point
$DBTRP	equ	0F65Dh	; debugger-monitor bpt entry
$DEASN	equ	013F8h	; delete logical device assignment
$DMTRP	equ	0F6D7h	; debugger-monitor bpt entry with message
$ERSEQ	equ	052B0h	; error sequence number
$FEAT	equ	0527Dh	; feature bits
$FILBF	equ	052CFh	; ptr to file I/O buffer area
$FIXTK	equ	032AAh	; fix task in memory
$FNDEV	equ	0144Ah	; find device and get UCB address
$FNPAR	equ	02676h	; find partition by name
$FNTCB	equ	00FB7h	; find TCB by name
$FNTSK	equ	00F8Fh	; find task by name
$FORKQ	equ	052BBh	; pointer to list of fork blocks
$FREE	equ	01935h	; free pool memory
$GTUCB	equ	01512h	; get UCB address
$HOSTN	equ	0527Eh	; host/node name
$IOEND	equ	01D17h	; end I/O processing for ASTs
$LDCKR	equ	00A57h	; resume task after checkpoint read
$LDCKW	equ	00A87h	; queue task to partition after chkpnt write
$LDGO	equ	009D4h	; start task after load
$LGOUT	equ	01642h	; logout user
$LKTCB	equ	0089Ah	; link TCB to the specified list
$LOGDV	equ	05291h	; ptr to head of logical device assignment list
$LOGIN	equ	015E2h	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	0528Dh	; total memory size
$MFLGS	equ	052BAh	; MCR flags
$MVTBL	equ	052BDh	; ptr to head of mounted device list
$PHYDV	equ	0528Fh	; ptr to head of physical device list
$PLIST	equ	05293h	; ptr to list of partitions
$POLSZ	equ	0528Bh	; size of pool area
$POOL	equ	05289h	; ptr to start of pool area
$REDIR	equ	01495h	; follow UCB redirection links
$REMTK	equ	032A1h	; remove task from STD
$RLIST	equ	05299h	; ptr to head of ready tasks list
$RNDC	equ	052A5h	; round-robin interval in ticks
$RNDH	equ	052A7h	; highest priority for round-robin scheduling
$RNDL	equ	052A8h	; lowest priority for round-robin scheduling
$SBANK	equ	0F3BFh	; system bank number
$SCRBF	equ	052BFh	; scratch buffer address
$SWPC	equ	052AAh	; swapping interval in ticks
$SWPCT	equ	052ABh	; swapping interval counter
$SWPRI	equ	052A9h	; priority range for swapping
$SWSTK	equ	052D1h	; stub to call kernel routines
$TLIST	equ	05297h	; ptr to head of installed tasks list
$UBANK	equ	0F3C1h	; bank of current task
$UCBDN	equ	015CEh	; get device name and unit from UCB
$UGETB	equ	0F36Fh	; inter-bank get byte routine
$UGETW	equ	0F38Dh	; inter-bank get word routine
$UNFTK	equ	032B3h	; unfix task
$UPUTB	equ	0F37Dh	; inter-bank put byte routine
$UPUTW	equ	0F39Fh	; inter-bank put word routine
$WLIST	equ	0529Bh	; ptr to head of waiting tasks list
IODONE	equ	01CFAh	; end I/O processing
LDRTCB	equ	052AEh	; LDR... TCB
MCRTCB	equ	052ACh	; MCR... TCB
NVOK	equ	04A42h	; P112 NV RAM OK flag
NVRAM	equ	04A2Ah	; P112 NV RAM contents
SETFLG	equ	0111Ch	; set task's event flag
SFLAGS	equ	052B2h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	05274h	; start of system data area
SYSEND	equ	0534Ah	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	05287h	; top of pool area
SYSTYP	equ	0527Ch	; system type
SYSVER	equ	0527Ah	; system version number
TICCNT	equ	0F3B4h	; absolute time count in ticks
TICSEC	equ	0F3B3h	; ticks per second
TKILL	equ	00C01h	; abort task
UPTIME	equ	0F3B8h	; system up time (ticks and seconds)
