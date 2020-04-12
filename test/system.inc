$ALLOC	equ	018F0h	; allocate pool memory
$ALUN	equ	0150Fh	; assign LUN to device
$ASGN	equ	01382h	; assign logical name to physical device
$ASNTP	equ	0269Bh	; assign next task to partition
$CHKPT	equ	0283Ch	; initiate task checkpoint
$CLASN	equ	013F4h	; delete all logical dev assignments of the given type
$CLIST	equ	04979h	; list of CLI control blocks
$CLKQ	equ	04977h	; ptr to head of clock queue
$CONN	equ	01669h	; connect to task
$CPCB	equ	04975h	; address of PCB of current task
$CPLST	equ	0496Bh	; ptr to head of checkpoint PCB list
$CTCB	equ	04973h	; address of TCB of current task
$DBMON	equ	0F336h	; debugger-monitor main entry point
$DBTRP	equ	0F58Dh	; debugger-monitor bpt entry
$DEASN	equ	013E1h	; delete logical device assignment
$DMTRP	equ	0F607h	; debugger-monitor bpt entry with message
$ERSEQ	equ	04986h	; error sequence number
$FEAT	equ	0495Ch	; feature bits
$FILBF	equ	049A5h	; ptr to file I/O buffer area
$FIXTK	equ	0328Ch	; fix task in memory
$FNDEV	equ	01433h	; find device and get UCB address
$FNPAR	equ	0264Fh	; find partition by name
$FNTCB	equ	00FA7h	; find TCB by name
$FNTSK	equ	00F7Fh	; find task by name
$FORKQ	equ	04991h	; pointer to list of fork blocks
$FREE	equ	0194Fh	; free pool memory
$GTUCB	equ	014FBh	; get UCB address
$HOSTN	equ	04953h	; host/node name
$IOEND	equ	01D38h	; end I/O processing for ASTs
$LDCKR	equ	00A42h	; resume task after checkpoint read
$LDCKW	equ	00A72h	; queue task to partition after chkpnt write
$LDGO	equ	009BEh	; start task after load
$LGOUT	equ	01634h	; logout user
$LKTCB	equ	00884h	; link TCB to the specified list
$LOGDV	equ	04967h	; ptr to head of logical device assignment list
$LOGIN	equ	015CBh	; login user
$MCOPY	equ	0F21Eh	; inter-bank memory copy routine
$MEMSZ	equ	04963h	; total memory size
$MFLGS	equ	04990h	; MCR flags
$MVTBL	equ	04993h	; ptr to head of mounted device list
$PHYDV	equ	04965h	; ptr to head of physical device list
$PLIST	equ	04969h	; ptr to list of partitions
$POLSZ	equ	04961h	; size of pool area
$POOL	equ	0495Fh	; ptr to start of pool area
$REDIR	equ	0147Eh	; follow UCB redirection links
$REMTK	equ	03283h	; remove task from STD
$RLIST	equ	0496Fh	; ptr to head of ready tasks list
$RNDC	equ	0497Bh	; round-robin interval in ticks
$RNDH	equ	0497Dh	; highest priority for round-robin scheduling
$RNDL	equ	0497Eh	; lowest priority for round-robin scheduling
$SBANK	equ	0F2F0h	; system bank number
$SCRBF	equ	04995h	; scratch buffer address
$SWPC	equ	04980h	; swapping interval in ticks
$SWPCT	equ	04981h	; swapping interval counter
$SWPRI	equ	0497Fh	; priority range for swapping
$SWSTK	equ	049A7h	; stub to call kernel routines
$TLIST	equ	0496Dh	; ptr to head of installed tasks list
$UBANK	equ	0F2F2h	; bank of current task
$UCBDN	equ	015B7h	; get device name and unit from UCB
$UGETB	equ	0F298h	; inter-bank get byte routine
$UGETW	equ	0F2BAh	; inter-bank get word routine
$UNFTK	equ	03295h	; unfix task
$UPUTB	equ	0F2A8h	; inter-bank put byte routine
$UPUTW	equ	0F2CEh	; inter-bank put word routine
$WLIST	equ	04971h	; ptr to head of waiting tasks list
IODONE	equ	01D18h	; end I/O processing
LDRTCB	equ	04984h	; LDR... TCB
MCRTCB	equ	04982h	; MCR... TCB
NVOK	equ	04525h	; P112 NV RAM OK flag
NVRAM	equ	0450Dh	; P112 NV RAM contents
SETFLG	equ	010FDh	; set task's event flag
SFLAGS	equ	04988h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	0494Bh	; start of system data area
SYSEND	equ	04A21h	; end of system image
SYSENT	equ	0F0B4h	; kernel services entry point
SYSLVL	equ	0F172h	; current system level
SYSTOP	equ	0495Dh	; top of pool area
SYSVER	equ	04951h	; system version number
TICCNT	equ	0F2E5h	; absolute time count in ticks
TICSEC	equ	0F2E4h	; ticks per second
TKILL	equ	00BEAh	; abort task
UPTIME	equ	0F2E9h	; system up time (ticks and seconds)
