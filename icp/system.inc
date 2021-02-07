$ALLOC	equ	018BFh	; allocate pool memory
$ALUN	equ	0150Dh	; assign LUN to device
$ASGN	equ	01380h	; assign logical name to physical device
$ASNTP	equ	0266Ah	; assign next task to partition
$CHKPT	equ	0280Bh	; initiate task checkpoint
$CLASN	equ	013F2h	; delete all logical dev assignments of the given type
$CLIST	equ	04F9Fh	; list of CLI control blocks
$CLKQ	equ	04F9Dh	; ptr to head of clock queue
$CONN	equ	0165Bh	; connect to task
$CPCB	equ	04F9Bh	; address of PCB of current task
$CPLST	equ	04F91h	; ptr to head of checkpoint PCB list
$CTCB	equ	04F99h	; address of TCB of current task
$DBMON	equ	0F412h	; debugger-monitor main entry point
$DBTRP	equ	0F669h	; debugger-monitor bpt entry
$DEASN	equ	013DFh	; delete logical device assignment
$DMTRP	equ	0F6E3h	; debugger-monitor bpt entry with message
$ERSEQ	equ	04FACh	; error sequence number
$FEAT	equ	04F79h	; feature bits
$FILBF	equ	04FCBh	; ptr to file I/O buffer area
$FIXTK	equ	0324Ch	; fix task in memory
$FNDEV	equ	01431h	; find device and get UCB address
$FNPAR	equ	0261Eh	; find partition by name
$FNTCB	equ	00F9Eh	; find TCB by name
$FNTSK	equ	00F76h	; find task by name
$FORKQ	equ	04FB7h	; pointer to list of fork blocks
$FREE	equ	0191Eh	; free pool memory
$GTUCB	equ	014F9h	; get UCB address
$HOSTN	equ	04F7Ah	; host/node name
$IOEND	equ	01D08h	; end I/O processing for ASTs
$LDCKR	equ	00A42h	; resume task after checkpoint read
$LDCKW	equ	00A72h	; queue task to partition after chkpnt write
$LDGO	equ	009BEh	; start task after load
$LGOUT	equ	0162Ch	; logout user
$LKTCB	equ	00884h	; link TCB to the specified list
$LOGDV	equ	04F8Dh	; ptr to head of logical device assignment list
$LOGIN	equ	015C9h	; login user
$MCOPY	equ	0F306h	; inter-bank memory copy routine
$MEMSZ	equ	04F89h	; total memory size
$MFLGS	equ	04FB6h	; MCR flags
$MVTBL	equ	04FB9h	; ptr to head of mounted device list
$PHYDV	equ	04F8Bh	; ptr to head of physical device list
$PLIST	equ	04F8Fh	; ptr to list of partitions
$POLSZ	equ	04F87h	; size of pool area
$POOL	equ	04F85h	; ptr to start of pool area
$REDIR	equ	0147Ch	; follow UCB redirection links
$REMTK	equ	03243h	; remove task from STD
$RLIST	equ	04F95h	; ptr to head of ready tasks list
$RNDC	equ	04FA1h	; round-robin interval in ticks
$RNDH	equ	04FA3h	; highest priority for round-robin scheduling
$RNDL	equ	04FA4h	; lowest priority for round-robin scheduling
$SBANK	equ	0F3CCh	; system bank number
$SCRBF	equ	04FBBh	; scratch buffer address
$SWPC	equ	04FA6h	; swapping interval in ticks
$SWPCT	equ	04FA7h	; swapping interval counter
$SWPRI	equ	04FA5h	; priority range for swapping
$SWSTK	equ	04FCDh	; stub to call kernel routines
$TLIST	equ	04F93h	; ptr to head of installed tasks list
$UBANK	equ	0F3CEh	; bank of current task
$UCBDN	equ	015B5h	; get device name and unit from UCB
$UGETB	equ	0F374h	; inter-bank get byte routine
$UGETW	equ	0F396h	; inter-bank get word routine
$UNFTK	equ	03255h	; unfix task
$UPUTB	equ	0F384h	; inter-bank put byte routine
$UPUTW	equ	0F3AAh	; inter-bank put word routine
$WLIST	equ	04F97h	; ptr to head of waiting tasks list
IODONE	equ	01CE8h	; end I/O processing
LDRTCB	equ	04FAAh	; LDR... TCB
MCRTCB	equ	04FA8h	; MCR... TCB
NVOK	equ	0486Eh	; P112 NV RAM OK flag
NVRAM	equ	04856h	; P112 NV RAM contents
SETFLG	equ	01103h	; set task's event flag
SFLAGS	equ	04FAEh	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	04F70h	; start of system data area
SYSEND	equ	05047h	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F255h	; current system level
SYSTOP	equ	04F83h	; top of pool area
SYSTYP	equ	04F78h	; system type
SYSVER	equ	04F76h	; system version number
TICCNT	equ	0F3C1h	; absolute time count in ticks
TICSEC	equ	0F3C0h	; ticks per second
TKILL	equ	00BE9h	; abort task
UPTIME	equ	0F3C5h	; system up time (ticks and seconds)
