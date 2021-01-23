$ALLOC	equ	018BFh	; allocate pool memory
$ALUN	equ	0150Dh	; assign LUN to device
$ASGN	equ	01380h	; assign logical name to physical device
$ASNTP	equ	0266Ah	; assign next task to partition
$CHKPT	equ	0280Bh	; initiate task checkpoint
$CLASN	equ	013F2h	; delete all logical dev assignments of the given type
$CLIST	equ	04F86h	; list of CLI control blocks
$CLKQ	equ	04F84h	; ptr to head of clock queue
$CONN	equ	0165Bh	; connect to task
$CPCB	equ	04F82h	; address of PCB of current task
$CPLST	equ	04F78h	; ptr to head of checkpoint PCB list
$CTCB	equ	04F80h	; address of TCB of current task
$DBMON	equ	0F334h	; debugger-monitor main entry point
$DBTRP	equ	0F58Bh	; debugger-monitor bpt entry
$DEASN	equ	013DFh	; delete logical device assignment
$DMTRP	equ	0F605h	; debugger-monitor bpt entry with message
$ERSEQ	equ	04F93h	; error sequence number
$FEAT	equ	04F60h	; feature bits
$FILBF	equ	04FB2h	; ptr to file I/O buffer area
$FIXTK	equ	03233h	; fix task in memory
$FNDEV	equ	01431h	; find device and get UCB address
$FNPAR	equ	0261Eh	; find partition by name
$FNTCB	equ	00F9Eh	; find TCB by name
$FNTSK	equ	00F76h	; find task by name
$FORKQ	equ	04F9Eh	; pointer to list of fork blocks
$FREE	equ	0191Eh	; free pool memory
$GTUCB	equ	014F9h	; get UCB address
$HOSTN	equ	04F61h	; host/node name
$IOEND	equ	01D08h	; end I/O processing for ASTs
$LDCKR	equ	00A42h	; resume task after checkpoint read
$LDCKW	equ	00A72h	; queue task to partition after chkpnt write
$LDGO	equ	009BEh	; start task after load
$LGOUT	equ	0162Ch	; logout user
$LKTCB	equ	00884h	; link TCB to the specified list
$LOGDV	equ	04F74h	; ptr to head of logical device assignment list
$LOGIN	equ	015C9h	; login user
$MCOPY	equ	0F21Ch	; inter-bank memory copy routine
$MEMSZ	equ	04F70h	; total memory size
$MFLGS	equ	04F9Dh	; MCR flags
$MVTBL	equ	04FA0h	; ptr to head of mounted device list
$PHYDV	equ	04F72h	; ptr to head of physical device list
$PLIST	equ	04F76h	; ptr to list of partitions
$POLSZ	equ	04F6Eh	; size of pool area
$POOL	equ	04F6Ch	; ptr to start of pool area
$REDIR	equ	0147Ch	; follow UCB redirection links
$REMTK	equ	0322Ah	; remove task from STD
$RLIST	equ	04F7Ch	; ptr to head of ready tasks list
$RNDC	equ	04F88h	; round-robin interval in ticks
$RNDH	equ	04F8Ah	; highest priority for round-robin scheduling
$RNDL	equ	04F8Bh	; lowest priority for round-robin scheduling
$SBANK	equ	0F2EEh	; system bank number
$SCRBF	equ	04FA2h	; scratch buffer address
$SWPC	equ	04F8Dh	; swapping interval in ticks
$SWPCT	equ	04F8Eh	; swapping interval counter
$SWPRI	equ	04F8Ch	; priority range for swapping
$SWSTK	equ	04FB4h	; stub to call kernel routines
$TLIST	equ	04F7Ah	; ptr to head of installed tasks list
$UBANK	equ	0F2F0h	; bank of current task
$UCBDN	equ	015B5h	; get device name and unit from UCB
$UGETB	equ	0F296h	; inter-bank get byte routine
$UGETW	equ	0F2B8h	; inter-bank get word routine
$UNFTK	equ	0323Ch	; unfix task
$UPUTB	equ	0F2A6h	; inter-bank put byte routine
$UPUTW	equ	0F2CCh	; inter-bank put word routine
$WLIST	equ	04F7Eh	; ptr to head of waiting tasks list
IODONE	equ	01CE8h	; end I/O processing
LDRTCB	equ	04F91h	; LDR... TCB
MCRTCB	equ	04F8Fh	; MCR... TCB
NVOK	equ	04855h	; P112 NV RAM OK flag
NVRAM	equ	0483Dh	; P112 NV RAM contents
SETFLG	equ	01103h	; set task's event flag
SFLAGS	equ	04F95h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	04F57h	; start of system data area
SYSEND	equ	0502Eh	; end of system image
SYSENT	equ	0F0B2h	; kernel services entry point
SYSLVL	equ	0F170h	; current system level
SYSTOP	equ	04F6Ah	; top of pool area
SYSTYP	equ	04F5Fh	; system type
SYSVER	equ	04F5Dh	; system version number
TICCNT	equ	0F2E3h	; absolute time count in ticks
TICSEC	equ	0F2E2h	; ticks per second
TKILL	equ	00BE9h	; abort task
UPTIME	equ	0F2E7h	; system up time (ticks and seconds)
