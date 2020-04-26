$ALLOC	equ	018C8h	; allocate pool memory
$ALUN	equ	01516h	; assign LUN to device
$ASGN	equ	01389h	; assign logical name to physical device
$ASNTP	equ	02674h	; assign next task to partition
$CHKPT	equ	02815h	; initiate task checkpoint
$CLASN	equ	013FBh	; delete all logical dev assignments of the given type
$CLIST	equ	04998h	; list of CLI control blocks
$CLKQ	equ	04996h	; ptr to head of clock queue
$CONN	equ	01664h	; connect to task
$CPCB	equ	04994h	; address of PCB of current task
$CPLST	equ	0498Ah	; ptr to head of checkpoint PCB list
$CTCB	equ	04992h	; address of TCB of current task
$DBMON	equ	0F336h	; debugger-monitor main entry point
$DBTRP	equ	0F58Dh	; debugger-monitor bpt entry
$DEASN	equ	013E8h	; delete logical device assignment
$DMTRP	equ	0F607h	; debugger-monitor bpt entry with message
$ERSEQ	equ	049A5h	; error sequence number
$FEAT	equ	04972h	; feature bits
$FILBF	equ	049C4h	; ptr to file I/O buffer area
$FIXTK	equ	0323Dh	; fix task in memory
$FNDEV	equ	0143Ah	; find device and get UCB address
$FNPAR	equ	02628h	; find partition by name
$FNTCB	equ	00FA6h	; find TCB by name
$FNTSK	equ	00F7Eh	; find task by name
$FORKQ	equ	049B0h	; pointer to list of fork blocks
$FREE	equ	01927h	; free pool memory
$GTUCB	equ	01502h	; get UCB address
$HOSTN	equ	04973h	; host/node name
$IOEND	equ	01D11h	; end I/O processing for ASTs
$LDCKR	equ	00A42h	; resume task after checkpoint read
$LDCKW	equ	00A72h	; queue task to partition after chkpnt write
$LDGO	equ	009BEh	; start task after load
$LGOUT	equ	01635h	; logout user
$LKTCB	equ	00884h	; link TCB to the specified list
$LOGDV	equ	04986h	; ptr to head of logical device assignment list
$LOGIN	equ	015D2h	; login user
$MCOPY	equ	0F21Eh	; inter-bank memory copy routine
$MEMSZ	equ	04982h	; total memory size
$MFLGS	equ	049AFh	; MCR flags
$MVTBL	equ	049B2h	; ptr to head of mounted device list
$PHYDV	equ	04984h	; ptr to head of physical device list
$PLIST	equ	04988h	; ptr to list of partitions
$POLSZ	equ	04980h	; size of pool area
$POOL	equ	0497Eh	; ptr to start of pool area
$REDIR	equ	01485h	; follow UCB redirection links
$REMTK	equ	03234h	; remove task from STD
$RLIST	equ	0498Eh	; ptr to head of ready tasks list
$RNDC	equ	0499Ah	; round-robin interval in ticks
$RNDH	equ	0499Ch	; highest priority for round-robin scheduling
$RNDL	equ	0499Dh	; lowest priority for round-robin scheduling
$SBANK	equ	0F2F0h	; system bank number
$SCRBF	equ	049B4h	; scratch buffer address
$SWPC	equ	0499Fh	; swapping interval in ticks
$SWPCT	equ	049A0h	; swapping interval counter
$SWPRI	equ	0499Eh	; priority range for swapping
$SWSTK	equ	049C6h	; stub to call kernel routines
$TLIST	equ	0498Ch	; ptr to head of installed tasks list
$UBANK	equ	0F2F2h	; bank of current task
$UCBDN	equ	015BEh	; get device name and unit from UCB
$UGETB	equ	0F298h	; inter-bank get byte routine
$UGETW	equ	0F2BAh	; inter-bank get word routine
$UNFTK	equ	03246h	; unfix task
$UPUTB	equ	0F2A8h	; inter-bank put byte routine
$UPUTW	equ	0F2CEh	; inter-bank put word routine
$WLIST	equ	04990h	; ptr to head of waiting tasks list
IODONE	equ	01CF1h	; end I/O processing
LDRTCB	equ	049A3h	; LDR... TCB
MCRTCB	equ	049A1h	; MCR... TCB
NVOK	equ	0451Fh	; P112 NV RAM OK flag
NVRAM	equ	04507h	; P112 NV RAM contents
SETFLG	equ	0110Ch	; set task's event flag
SFLAGS	equ	049A7h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	04969h	; start of system data area
SYSEND	equ	04A40h	; end of system image
SYSENT	equ	0F0B4h	; kernel services entry point
SYSLVL	equ	0F172h	; current system level
SYSTOP	equ	0497Ch	; top of pool area
SYSTYP	equ	04971h	; system type
SYSVER	equ	0496Fh	; system version number
TICCNT	equ	0F2E5h	; absolute time count in ticks
TICSEC	equ	0F2E4h	; ticks per second
TKILL	equ	00BE9h	; abort task
UPTIME	equ	0F2E9h	; system up time (ticks and seconds)
