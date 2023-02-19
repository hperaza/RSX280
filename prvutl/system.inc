$ALLOC	equ	015BEh	; allocate pool memory
$ALUN	equ	011CCh	; assign LUN to device
$ASGN	equ	0103Fh	; assign logical name to physical device
$ASNTP	equ	0249Bh	; assign next task to partition
$AVAIL	equ	016A7h	; compute available pool memory
$CHKPT	equ	02644h	; initiate task checkpoint
$CLASN	equ	010B1h	; delete all logical dev assignments of the given type
$CLIST	equ	053F9h	; list of CLI control blocks
$CLKQ	equ	053F7h	; ptr to head of clock queue
$CONN	equ	01325h	; connect to task
$CPCB	equ	053F5h	; address of PCB of current task
$CPLST	equ	053EBh	; ptr to head of checkpoint PCB list
$CTCB	equ	053F3h	; address of TCB of current task
$DBBRK	equ	00CEAh	; break task execution
$DBCON	equ	00C38h	; connect task to external debugger
$DBDIS	equ	00CB2h	; disconnect task from external debugger
$DBRES	equ	00D14h	; resume task stopped for debugger
$DEASN	equ	0109Eh	; delete logical device assignment
$ERSEQ	equ	05406h	; error sequence number
$FEAT	equ	053D3h	; feature bits
$FILBF	equ	05425h	; ptr to file I/O buffer area
$FIXTK	equ	0065Fh	; fix task in memory
$FNDEV	equ	010F0h	; find device and get UCB address
$FNPAR	equ	0244Eh	; find partition by name
$FNTCB	equ	00A9Eh	; find TCB by name
$FNTSK	equ	00A76h	; find task by name
$FORKQ	equ	05411h	; pointer to list of fork blocks
$FREE	equ	0161Dh	; free pool memory
$GTUCB	equ	011B8h	; get UCB address
$HOSTN	equ	053D4h	; host/node name
$IOEND	equ	01A87h	; end I/O processing for ASTs
$KDMON	equ	0F44Ah	; kernel debugger-monitor main entry point
$KDTRP	equ	0F761h	; kernel debugger-monitor trap entry point
$KMTRP	equ	0F7DBh	; kernel debugger-monitor trap entry with message
$LDCKR	equ	00551h	; resume task after checkpoint read
$LDCKW	equ	00581h	; queue task to partition after chkpnt write
$LDGO	equ	004BAh	; start task after load
$LGOUT	equ	012F6h	; logout user
$LKTCB	equ	00380h	; link TCB to the specified list
$LOGDV	equ	053E7h	; ptr to head of logical device assignment list
$LOGIN	equ	01292h	; login user
$MCOPY	equ	0F341h	; inter-bank memory copy routine
$MEMSZ	equ	053E3h	; total memory size
$MFLGS	equ	05410h	; MCR flags
$MVTBL	equ	05413h	; ptr to head of mounted device list
$PHYDV	equ	053E5h	; ptr to head of physical device list
$PLIST	equ	053E9h	; ptr to list of partitions
$POLSZ	equ	053E1h	; size of pool area
$POOL	equ	053DFh	; ptr to start of pool area
$REDIR	equ	0113Bh	; follow UCB redirection links
$REMTK	equ	00869h	; remove task from STD
$RLIST	equ	053EFh	; ptr to head of ready tasks list
$RNDC	equ	053FBh	; round-robin interval in ticks
$RNDH	equ	053FDh	; highest priority for round-robin scheduling
$RNDL	equ	053FEh	; lowest priority for round-robin scheduling
$SBANK	equ	0F404h	; system bank number
$SBASE	equ	0F402h	; physical system base bank
$SCRBF	equ	05415h	; scratch buffer address
$SWPC	equ	05400h	; swapping interval in ticks
$SWPCT	equ	05401h	; swapping interval counter
$SWPRI	equ	053FFh	; priority range for swapping
$SWSTK	equ	05427h	; stub to call kernel routines
$TLIST	equ	053EDh	; ptr to head of installed tasks list
$UBANK	equ	0F406h	; bank of current task
$UCBDN	equ	0127Eh	; get device name and unit from UCB
$UGETB	equ	0F3B2h	; inter-bank get byte routine
$UGETW	equ	0F3D0h	; inter-bank get word routine
$UNFTK	equ	0069Bh	; unfix task
$UPUTB	equ	0F3C0h	; inter-bank put byte routine
$UPUTW	equ	0F3E2h	; inter-bank put word routine
$VALBF	equ	01C80h	; validate buffer (current task)
$VALTB	equ	01C63h	; validate buffer (arbitrary task)
$WLIST	equ	053F1h	; ptr to head of waiting tasks list
IODONE	equ	01A6Ah	; end I/O processing
LDRTCB	equ	05404h	; LDR... TCB
MCRTCB	equ	05402h	; MCR... TCB
NVOK	equ	04B38h	; P112 NV RAM OK flag
NVRAM	equ	04B20h	; P112 NV RAM contents
SETFLG	equ	00DD8h	; set task's event flag
SFLAGS	equ	05408h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	053CAh	; start of system data area
SYSEND	equ	054A0h	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F290h	; current system level
SYSTOP	equ	053DDh	; top of pool area
SYSTYP	equ	053D2h	; system type
SYSVER	equ	053D0h	; system version number
TICCNT	equ	0F3F7h	; absolute time count in ticks
TICSEC	equ	0F3F6h	; ticks per second
TKILL	equ	006E1h	; abort task
UPTIME	equ	0F3FBh	; system up time (ticks and seconds)
URQST	equ	02B14h	; request task, TDB in user space
