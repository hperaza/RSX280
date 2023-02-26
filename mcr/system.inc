$ALLOC	equ	015B7h	; allocate pool memory
$ALUN	equ	011C5h	; assign LUN to device
$ASGN	equ	01038h	; assign logical name to physical device
$ASNTP	equ	02494h	; assign next task to partition
$AVAIL	equ	016A0h	; compute available pool memory
$CHKPT	equ	0263Dh	; initiate task checkpoint
$CLASN	equ	010AAh	; delete all logical dev assignments of the given type
$CLIST	equ	053F2h	; list of CLI control blocks
$CLKQ	equ	053F0h	; ptr to head of clock queue
$CONN	equ	0131Eh	; connect to task
$CPCB	equ	053EEh	; address of PCB of current task
$CPLST	equ	053E4h	; ptr to head of checkpoint PCB list
$CTCB	equ	053ECh	; address of TCB of current task
$DBBRK	equ	00CE3h	; break task execution
$DBCON	equ	00C38h	; connect task to external debugger
$DBDIS	equ	00CABh	; disconnect task from external debugger
$DBRES	equ	00D0Dh	; resume task stopped for debugger
$DEASN	equ	01097h	; delete logical device assignment
$ERSEQ	equ	053FFh	; error sequence number
$FEAT	equ	053CCh	; feature bits
$FILBF	equ	0541Eh	; ptr to file I/O buffer area
$FIXTK	equ	0065Fh	; fix task in memory
$FNDEV	equ	010E9h	; find device and get UCB address
$FNPAR	equ	02447h	; find partition by name
$FNTCB	equ	00A9Eh	; find TCB by name
$FNTSK	equ	00A76h	; find task by name
$FORKQ	equ	0540Ah	; pointer to list of fork blocks
$FREE	equ	01616h	; free pool memory
$GTUCB	equ	011B1h	; get UCB address
$HOSTN	equ	053CDh	; host/node name
$IOEND	equ	01A80h	; end I/O processing for ASTs
$KDMON	equ	0F44Ah	; kernel debugger-monitor main entry point
$KDTRP	equ	0F761h	; kernel debugger-monitor trap entry point
$KMTRP	equ	0F7DBh	; kernel debugger-monitor trap entry with message
$LDCKR	equ	00551h	; resume task after checkpoint read
$LDCKW	equ	00581h	; queue task to partition after chkpnt write
$LDGO	equ	004BAh	; start task after load
$LGOUT	equ	012EFh	; logout user
$LKTCB	equ	00380h	; link TCB to the specified list
$LOGDV	equ	053E0h	; ptr to head of logical device assignment list
$LOGIN	equ	0128Bh	; login user
$MCOPY	equ	0F341h	; inter-bank memory copy routine
$MEMSZ	equ	053DCh	; total memory size
$MFLGS	equ	05409h	; MCR flags
$MVTBL	equ	0540Ch	; ptr to head of mounted device list
$PHYDV	equ	053DEh	; ptr to head of physical device list
$PLIST	equ	053E2h	; ptr to list of partitions
$POLSZ	equ	053DAh	; size of pool area
$POOL	equ	053D8h	; ptr to start of pool area
$REDIR	equ	01134h	; follow UCB redirection links
$REMTK	equ	00869h	; remove task from STD
$RLIST	equ	053E8h	; ptr to head of ready tasks list
$RNDC	equ	053F4h	; round-robin interval in ticks
$RNDH	equ	053F6h	; highest priority for round-robin scheduling
$RNDL	equ	053F7h	; lowest priority for round-robin scheduling
$SBANK	equ	0F404h	; system bank number
$SBASE	equ	0F402h	; physical system base bank
$SCRBF	equ	0540Eh	; scratch buffer address
$SWPC	equ	053F9h	; swapping interval in ticks
$SWPCT	equ	053FAh	; swapping interval counter
$SWPRI	equ	053F8h	; priority range for swapping
$SWSTK	equ	05420h	; stub to call kernel routines
$TLIST	equ	053E6h	; ptr to head of installed tasks list
$UBANK	equ	0F406h	; bank of current task
$UCBDN	equ	01277h	; get device name and unit from UCB
$UGETB	equ	0F3B2h	; inter-bank get byte routine
$UGETW	equ	0F3D0h	; inter-bank get word routine
$UNFTK	equ	0069Bh	; unfix task
$UPUTB	equ	0F3C0h	; inter-bank put byte routine
$UPUTW	equ	0F3E2h	; inter-bank put word routine
$VALBF	equ	01C79h	; validate buffer (current task)
$VALTB	equ	01C5Ch	; validate buffer (arbitrary task)
$WLIST	equ	053EAh	; ptr to head of waiting tasks list
IODONE	equ	01A63h	; end I/O processing
LDRTCB	equ	053FDh	; LDR... TCB
MCRTCB	equ	053FBh	; MCR... TCB
NVOK	equ	04B31h	; P112 NV RAM OK flag
NVRAM	equ	04B19h	; P112 NV RAM contents
SETFLG	equ	00DD1h	; set task's event flag
SFLAGS	equ	05401h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	053C3h	; start of system data area
SYSEND	equ	05499h	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F290h	; current system level
SYSTOP	equ	053D6h	; top of pool area
SYSTYP	equ	053CBh	; system type
SYSVER	equ	053C9h	; system version number
TICCNT	equ	0F3F7h	; absolute time count in ticks
TICSEC	equ	0F3F6h	; ticks per second
TKILL	equ	006E1h	; abort task
UPTIME	equ	0F3FBh	; system up time (ticks and seconds)
URQST	equ	02B0Dh	; request task, TDB in user space
