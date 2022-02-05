$ALLOC	equ	01905h	; allocate pool memory
$ALUN	equ	01551h	; assign LUN to device
$ASGN	equ	013C4h	; assign logical name to physical device
$ASNTP	equ	02721h	; assign next task to partition
$CHKPT	equ	028BEh	; initiate task checkpoint
$CLASN	equ	01436h	; delete all logical dev assignments of the given type
$CLIST	equ	05384h	; list of CLI control blocks
$CLKQ	equ	05382h	; ptr to head of clock queue
$CONN	equ	016A0h	; connect to task
$CPCB	equ	05380h	; address of PCB of current task
$CPLST	equ	05376h	; ptr to head of checkpoint PCB list
$CTCB	equ	0537Eh	; address of TCB of current task
$DBMON	equ	0F406h	; debugger-monitor main entry point
$DBTRP	equ	0F65Dh	; debugger-monitor bpt entry
$DEASN	equ	01423h	; delete logical device assignment
$DMTRP	equ	0F6D7h	; debugger-monitor bpt entry with message
$ERSEQ	equ	05391h	; error sequence number
$FEAT	equ	0535Eh	; feature bits
$FILBF	equ	053B0h	; ptr to file I/O buffer area
$FIXTK	equ	0335Bh	; fix task in memory
$FNDEV	equ	01475h	; find device and get UCB address
$FNPAR	equ	026D5h	; find partition by name
$FNTCB	equ	00FE2h	; find TCB by name
$FNTSK	equ	00FBAh	; find task by name
$FORKQ	equ	0539Ch	; pointer to list of fork blocks
$FREE	equ	01964h	; free pool memory
$GTUCB	equ	0153Dh	; get UCB address
$HOSTN	equ	0535Fh	; host/node name
$IOEND	equ	01D76h	; end I/O processing for ASTs
$LDCKR	equ	00A82h	; resume task after checkpoint read
$LDCKW	equ	00AB2h	; queue task to partition after chkpnt write
$LDGO	equ	009F4h	; start task after load
$LGOUT	equ	01671h	; logout user
$LKTCB	equ	008BAh	; link TCB to the specified list
$LOGDV	equ	05372h	; ptr to head of logical device assignment list
$LOGIN	equ	0160Dh	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	0536Eh	; total memory size
$MFLGS	equ	0539Bh	; MCR flags
$MVTBL	equ	0539Eh	; ptr to head of mounted device list
$PHYDV	equ	05370h	; ptr to head of physical device list
$PLIST	equ	05374h	; ptr to list of partitions
$POLSZ	equ	0536Ch	; size of pool area
$POOL	equ	0536Ah	; ptr to start of pool area
$REDIR	equ	014C0h	; follow UCB redirection links
$REMTK	equ	03352h	; remove task from STD
$RLIST	equ	0537Ah	; ptr to head of ready tasks list
$RNDC	equ	05386h	; round-robin interval in ticks
$RNDH	equ	05388h	; highest priority for round-robin scheduling
$RNDL	equ	05389h	; lowest priority for round-robin scheduling
$SBANK	equ	0F3BFh	; system bank number
$SCRBF	equ	053A0h	; scratch buffer address
$SWPC	equ	0538Bh	; swapping interval in ticks
$SWPCT	equ	0538Ch	; swapping interval counter
$SWPRI	equ	0538Ah	; priority range for swapping
$SWSTK	equ	053B2h	; stub to call kernel routines
$TLIST	equ	05378h	; ptr to head of installed tasks list
$UBANK	equ	0F3C1h	; bank of current task
$UCBDN	equ	015F9h	; get device name and unit from UCB
$UGETB	equ	0F36Fh	; inter-bank get byte routine
$UGETW	equ	0F38Dh	; inter-bank get word routine
$UNFTK	equ	03364h	; unfix task
$UPUTB	equ	0F37Dh	; inter-bank put byte routine
$UPUTW	equ	0F39Fh	; inter-bank put word routine
$WLIST	equ	0537Ch	; ptr to head of waiting tasks list
IODONE	equ	01D59h	; end I/O processing
LDRTCB	equ	0538Fh	; LDR... TCB
MCRTCB	equ	0538Dh	; MCR... TCB
NVOK	equ	04B1Eh	; P112 NV RAM OK flag
NVRAM	equ	04B06h	; P112 NV RAM contents
SETFLG	equ	01147h	; set task's event flag
SFLAGS	equ	05393h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	05355h	; start of system data area
SYSEND	equ	0542Bh	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	05368h	; top of pool area
SYSTYP	equ	0535Dh	; system type
SYSVER	equ	0535Bh	; system version number
TICCNT	equ	0F3B4h	; absolute time count in ticks
TICSEC	equ	0F3B3h	; ticks per second
TKILL	equ	00C2Ch	; abort task
UPTIME	equ	0F3B8h	; system up time (ticks and seconds)
