$ALLOC	equ	018E0h	; allocate pool memory
$ALUN	equ	0152Ch	; assign LUN to device
$ASGN	equ	0139Fh	; assign logical name to physical device
$ASNTP	equ	02723h	; assign next task to partition
$CHKPT	equ	028C0h	; initiate task checkpoint
$CLASN	equ	01411h	; delete all logical dev assignments of the given type
$CLIST	equ	05386h	; list of CLI control blocks
$CLKQ	equ	05384h	; ptr to head of clock queue
$CONN	equ	0167Bh	; connect to task
$CPCB	equ	05382h	; address of PCB of current task
$CPLST	equ	05378h	; ptr to head of checkpoint PCB list
$CTCB	equ	05380h	; address of TCB of current task
$DBMON	equ	0F406h	; debugger-monitor main entry point
$DBTRP	equ	0F65Dh	; debugger-monitor bpt entry
$DEASN	equ	013FEh	; delete logical device assignment
$DMTRP	equ	0F6D7h	; debugger-monitor bpt entry with message
$ERSEQ	equ	05393h	; error sequence number
$FEAT	equ	05360h	; feature bits
$FILBF	equ	053B2h	; ptr to file I/O buffer area
$FIXTK	equ	0335Dh	; fix task in memory
$FNDEV	equ	01450h	; find device and get UCB address
$FNPAR	equ	026D7h	; find partition by name
$FNTCB	equ	00FBDh	; find TCB by name
$FNTSK	equ	00F95h	; find task by name
$FORKQ	equ	0539Eh	; pointer to list of fork blocks
$FREE	equ	0193Fh	; free pool memory
$GTUCB	equ	01518h	; get UCB address
$HOSTN	equ	05361h	; host/node name
$IOEND	equ	01D78h	; end I/O processing for ASTs
$LDCKR	equ	00A82h	; resume task after checkpoint read
$LDCKW	equ	00AB2h	; queue task to partition after chkpnt write
$LDGO	equ	009F4h	; start task after load
$LGOUT	equ	0164Ch	; logout user
$LKTCB	equ	008BAh	; link TCB to the specified list
$LOGDV	equ	05374h	; ptr to head of logical device assignment list
$LOGIN	equ	015E8h	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	05370h	; total memory size
$MFLGS	equ	0539Dh	; MCR flags
$MVTBL	equ	053A0h	; ptr to head of mounted device list
$PHYDV	equ	05372h	; ptr to head of physical device list
$PLIST	equ	05376h	; ptr to list of partitions
$POLSZ	equ	0536Eh	; size of pool area
$POOL	equ	0536Ch	; ptr to start of pool area
$REDIR	equ	0149Bh	; follow UCB redirection links
$REMTK	equ	03354h	; remove task from STD
$RLIST	equ	0537Ch	; ptr to head of ready tasks list
$RNDC	equ	05388h	; round-robin interval in ticks
$RNDH	equ	0538Ah	; highest priority for round-robin scheduling
$RNDL	equ	0538Bh	; lowest priority for round-robin scheduling
$SBANK	equ	0F3BFh	; system bank number
$SCRBF	equ	053A2h	; scratch buffer address
$SWPC	equ	0538Dh	; swapping interval in ticks
$SWPCT	equ	0538Eh	; swapping interval counter
$SWPRI	equ	0538Ch	; priority range for swapping
$SWSTK	equ	053B4h	; stub to call kernel routines
$TLIST	equ	0537Ah	; ptr to head of installed tasks list
$UBANK	equ	0F3C1h	; bank of current task
$UCBDN	equ	015D4h	; get device name and unit from UCB
$UGETB	equ	0F36Fh	; inter-bank get byte routine
$UGETW	equ	0F38Dh	; inter-bank get word routine
$UNFTK	equ	03366h	; unfix task
$UPUTB	equ	0F37Dh	; inter-bank put byte routine
$UPUTW	equ	0F39Fh	; inter-bank put word routine
$WLIST	equ	0537Eh	; ptr to head of waiting tasks list
IODONE	equ	01D5Bh	; end I/O processing
LDRTCB	equ	05391h	; LDR... TCB
MCRTCB	equ	0538Fh	; MCR... TCB
NVOK	equ	04B20h	; P112 NV RAM OK flag
NVRAM	equ	04B08h	; P112 NV RAM contents
SETFLG	equ	01122h	; set task's event flag
SFLAGS	equ	05395h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	05357h	; start of system data area
SYSEND	equ	0542Dh	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	0536Ah	; top of pool area
SYSTYP	equ	0535Fh	; system type
SYSVER	equ	0535Dh	; system version number
TICCNT	equ	0F3B4h	; absolute time count in ticks
TICSEC	equ	0F3B3h	; ticks per second
TKILL	equ	00C07h	; abort task
UPTIME	equ	0F3B8h	; system up time (ticks and seconds)
