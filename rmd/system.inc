$ALLOC	equ	018D3h	; allocate pool memory
$ALUN	equ	0151Fh	; assign LUN to device
$ASGN	equ	01392h	; assign logical name to physical device
$ASNTP	equ	02717h	; assign next task to partition
$CHKPT	equ	028B4h	; initiate task checkpoint
$CLASN	equ	01404h	; delete all logical dev assignments of the given type
$CLIST	equ	054B8h	; list of CLI control blocks
$CLKQ	equ	054B6h	; ptr to head of clock queue
$CONN	equ	0166Eh	; connect to task
$CPCB	equ	054B4h	; address of PCB of current task
$CPLST	equ	054AAh	; ptr to head of checkpoint PCB list
$CTCB	equ	054B2h	; address of TCB of current task
$DBMON	equ	0F406h	; debugger-monitor main entry point
$DBTRP	equ	0F65Dh	; debugger-monitor bpt entry
$DEASN	equ	013F1h	; delete logical device assignment
$DMTRP	equ	0F6D7h	; debugger-monitor bpt entry with message
$ERSEQ	equ	054C5h	; error sequence number
$FEAT	equ	05492h	; feature bits
$FILBF	equ	054E4h	; ptr to file I/O buffer area
$FIXTK	equ	03365h	; fix task in memory
$FNDEV	equ	01443h	; find device and get UCB address
$FNPAR	equ	026CAh	; find partition by name
$FNTCB	equ	00FB0h	; find TCB by name
$FNTSK	equ	00F88h	; find task by name
$FORKQ	equ	054D0h	; pointer to list of fork blocks
$FREE	equ	01932h	; free pool memory
$GTUCB	equ	0150Bh	; get UCB address
$HOSTN	equ	05493h	; host/node name
$IOEND	equ	01D6Bh	; end I/O processing for ASTs
$LDCKR	equ	00A75h	; resume task after checkpoint read
$LDCKW	equ	00AA5h	; queue task to partition after chkpnt write
$LDGO	equ	009E7h	; start task after load
$LGOUT	equ	0163Fh	; logout user
$LKTCB	equ	008ADh	; link TCB to the specified list
$LOGDV	equ	054A6h	; ptr to head of logical device assignment list
$LOGIN	equ	015DBh	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	054A2h	; total memory size
$MFLGS	equ	054CFh	; MCR flags
$MVTBL	equ	054D2h	; ptr to head of mounted device list
$PHYDV	equ	054A4h	; ptr to head of physical device list
$PLIST	equ	054A8h	; ptr to list of partitions
$POLSZ	equ	054A0h	; size of pool area
$POOL	equ	0549Eh	; ptr to start of pool area
$REDIR	equ	0148Eh	; follow UCB redirection links
$REMTK	equ	0335Ch	; remove task from STD
$RLIST	equ	054AEh	; ptr to head of ready tasks list
$RNDC	equ	054BAh	; round-robin interval in ticks
$RNDH	equ	054BCh	; highest priority for round-robin scheduling
$RNDL	equ	054BDh	; lowest priority for round-robin scheduling
$SBANK	equ	0F3BFh	; system bank number
$SCRBF	equ	054D4h	; scratch buffer address
$SWPC	equ	054BFh	; swapping interval in ticks
$SWPCT	equ	054C0h	; swapping interval counter
$SWPRI	equ	054BEh	; priority range for swapping
$SWSTK	equ	054E6h	; stub to call kernel routines
$TLIST	equ	054ACh	; ptr to head of installed tasks list
$UBANK	equ	0F3C1h	; bank of current task
$UCBDN	equ	015C7h	; get device name and unit from UCB
$UGETB	equ	0F36Fh	; inter-bank get byte routine
$UGETW	equ	0F38Dh	; inter-bank get word routine
$UNFTK	equ	0336Eh	; unfix task
$UPUTB	equ	0F37Dh	; inter-bank put byte routine
$UPUTW	equ	0F39Fh	; inter-bank put word routine
$WLIST	equ	054B0h	; ptr to head of waiting tasks list
IODONE	equ	01D4Eh	; end I/O processing
LDRTCB	equ	054C3h	; LDR... TCB
MCRTCB	equ	054C1h	; MCR... TCB
NVOK	equ	04BF7h	; P112 NV RAM OK flag
NVRAM	equ	04BDFh	; P112 NV RAM contents
SETFLG	equ	01115h	; set task's event flag
SFLAGS	equ	054C7h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	05489h	; start of system data area
SYSEND	equ	0555Fh	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	0549Ch	; top of pool area
SYSTYP	equ	05491h	; system type
SYSVER	equ	0548Fh	; system version number
TICCNT	equ	0F3B4h	; absolute time count in ticks
TICSEC	equ	0F3B3h	; ticks per second
TKILL	equ	00BFAh	; abort task
UPTIME	equ	0F3B8h	; system up time (ticks and seconds)
