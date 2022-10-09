$ALLOC	equ	01352h	; allocate pool memory
$ALUN	equ	00F9Eh	; assign LUN to device
$ASGN	equ	00E11h	; assign logical name to physical device
$ASNTP	equ	021D7h	; assign next task to partition
$CHKPT	equ	02374h	; initiate task checkpoint
$CLASN	equ	00E83h	; delete all logical dev assignments of the given type
$CLIST	equ	05081h	; list of CLI control blocks
$CLKQ	equ	0507Fh	; ptr to head of clock queue
$CONN	equ	010EDh	; connect to task
$CPCB	equ	0507Dh	; address of PCB of current task
$CPLST	equ	05073h	; ptr to head of checkpoint PCB list
$CTCB	equ	0507Bh	; address of TCB of current task
$DBMON	equ	0F406h	; debugger-monitor main entry point
$DBTRP	equ	0F65Dh	; debugger-monitor bpt entry
$DEASN	equ	00E70h	; delete logical device assignment
$DMTRP	equ	0F6D7h	; debugger-monitor bpt entry with message
$ERSEQ	equ	0508Eh	; error sequence number
$FEAT	equ	0505Bh	; feature bits
$FILBF	equ	050ADh	; ptr to file I/O buffer area
$FIXTK	equ	02EC5h	; fix task in memory
$FNDEV	equ	00EC2h	; find device and get UCB address
$FNPAR	equ	0218Ah	; find partition by name
$FNTCB	equ	00A6Ah	; find TCB by name
$FNTSK	equ	00A42h	; find task by name
$FORKQ	equ	05099h	; pointer to list of fork blocks
$FREE	equ	013B1h	; free pool memory
$GTUCB	equ	00F8Ah	; get UCB address
$HOSTN	equ	0505Ch	; host/node name
$IOEND	equ	01801h	; end I/O processing for ASTs
$LDCKR	equ	0052Fh	; resume task after checkpoint read
$LDCKW	equ	0055Fh	; queue task to partition after chkpnt write
$LDGO	equ	004A1h	; start task after load
$LGOUT	equ	010BEh	; logout user
$LKTCB	equ	00367h	; link TCB to the specified list
$LOGDV	equ	0506Fh	; ptr to head of logical device assignment list
$LOGIN	equ	0105Ah	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	0506Bh	; total memory size
$MFLGS	equ	05098h	; MCR flags
$MVTBL	equ	0509Bh	; ptr to head of mounted device list
$PHYDV	equ	0506Dh	; ptr to head of physical device list
$PLIST	equ	05071h	; ptr to list of partitions
$POLSZ	equ	05069h	; size of pool area
$POOL	equ	05067h	; ptr to start of pool area
$REDIR	equ	00F0Dh	; follow UCB redirection links
$REMTK	equ	02EBCh	; remove task from STD
$RLIST	equ	05077h	; ptr to head of ready tasks list
$RNDC	equ	05083h	; round-robin interval in ticks
$RNDH	equ	05085h	; highest priority for round-robin scheduling
$RNDL	equ	05086h	; lowest priority for round-robin scheduling
$SBANK	equ	0F3BFh	; system bank number
$SCRBF	equ	0509Dh	; scratch buffer address
$SWPC	equ	05088h	; swapping interval in ticks
$SWPCT	equ	05089h	; swapping interval counter
$SWPRI	equ	05087h	; priority range for swapping
$SWSTK	equ	050AFh	; stub to call kernel routines
$TLIST	equ	05075h	; ptr to head of installed tasks list
$UBANK	equ	0F3C1h	; bank of current task
$UCBDN	equ	01046h	; get device name and unit from UCB
$UGETB	equ	0F36Fh	; inter-bank get byte routine
$UGETW	equ	0F38Dh	; inter-bank get word routine
$UNFTK	equ	02ECEh	; unfix task
$UPUTB	equ	0F37Dh	; inter-bank put byte routine
$UPUTW	equ	0F39Fh	; inter-bank put word routine
$VALBF	equ	019FAh	; validate buffer (current task)
$VALTB	equ	019DDh	; validate buffer (arbitrary task)
$WLIST	equ	05079h	; ptr to head of waiting tasks list
IODONE	equ	017E4h	; end I/O processing
LDRTCB	equ	0508Ch	; LDR... TCB
MCRTCB	equ	0508Ah	; MCR... TCB
NVOK	equ	047C0h	; P112 NV RAM OK flag
NVRAM	equ	047A8h	; P112 NV RAM contents
SETFLG	equ	00BBCh	; set task's event flag
SFLAGS	equ	05090h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	05052h	; start of system data area
SYSEND	equ	05128h	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	05065h	; top of pool area
SYSTYP	equ	0505Ah	; system type
SYSVER	equ	05058h	; system version number
TICCNT	equ	0F3B4h	; absolute time count in ticks
TICSEC	equ	0F3B3h	; ticks per second
TKILL	equ	006B4h	; abort task
UPTIME	equ	0F3B8h	; system up time (ticks and seconds)
