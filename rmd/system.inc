$ALLOC	equ	018D3h	; allocate pool memory
$ALUN	equ	0151Fh	; assign LUN to device
$ASGN	equ	01392h	; assign logical name to physical device
$ASNTP	equ	02717h	; assign next task to partition
$CHKPT	equ	028B4h	; initiate task checkpoint
$CLASN	equ	01404h	; delete all logical dev assignments of the given type
$CLIST	equ	054F7h	; list of CLI control blocks
$CLKQ	equ	054F5h	; ptr to head of clock queue
$CONN	equ	0166Eh	; connect to task
$CPCB	equ	054F3h	; address of PCB of current task
$CPLST	equ	054E9h	; ptr to head of checkpoint PCB list
$CTCB	equ	054F1h	; address of TCB of current task
$DBMON	equ	0F406h	; debugger-monitor main entry point
$DBTRP	equ	0F65Dh	; debugger-monitor bpt entry
$DEASN	equ	013F1h	; delete logical device assignment
$DMTRP	equ	0F6D7h	; debugger-monitor bpt entry with message
$ERSEQ	equ	05504h	; error sequence number
$FEAT	equ	054D1h	; feature bits
$FILBF	equ	05523h	; ptr to file I/O buffer area
$FIXTK	equ	03351h	; fix task in memory
$FNDEV	equ	01443h	; find device and get UCB address
$FNPAR	equ	026CAh	; find partition by name
$FNTCB	equ	00FB0h	; find TCB by name
$FNTSK	equ	00F88h	; find task by name
$FORKQ	equ	0550Fh	; pointer to list of fork blocks
$FREE	equ	01932h	; free pool memory
$GTUCB	equ	0150Bh	; get UCB address
$HOSTN	equ	054D2h	; host/node name
$IOEND	equ	01D6Bh	; end I/O processing for ASTs
$LDCKR	equ	00A75h	; resume task after checkpoint read
$LDCKW	equ	00AA5h	; queue task to partition after chkpnt write
$LDGO	equ	009E7h	; start task after load
$LGOUT	equ	0163Fh	; logout user
$LKTCB	equ	008ADh	; link TCB to the specified list
$LOGDV	equ	054E5h	; ptr to head of logical device assignment list
$LOGIN	equ	015DBh	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	054E1h	; total memory size
$MFLGS	equ	0550Eh	; MCR flags
$MVTBL	equ	05511h	; ptr to head of mounted device list
$PHYDV	equ	054E3h	; ptr to head of physical device list
$PLIST	equ	054E7h	; ptr to list of partitions
$POLSZ	equ	054DFh	; size of pool area
$POOL	equ	054DDh	; ptr to start of pool area
$REDIR	equ	0148Eh	; follow UCB redirection links
$REMTK	equ	03348h	; remove task from STD
$RLIST	equ	054EDh	; ptr to head of ready tasks list
$RNDC	equ	054F9h	; round-robin interval in ticks
$RNDH	equ	054FBh	; highest priority for round-robin scheduling
$RNDL	equ	054FCh	; lowest priority for round-robin scheduling
$SBANK	equ	0F3BFh	; system bank number
$SCRBF	equ	05513h	; scratch buffer address
$SWPC	equ	054FEh	; swapping interval in ticks
$SWPCT	equ	054FFh	; swapping interval counter
$SWPRI	equ	054FDh	; priority range for swapping
$SWSTK	equ	05525h	; stub to call kernel routines
$TLIST	equ	054EBh	; ptr to head of installed tasks list
$UBANK	equ	0F3C1h	; bank of current task
$UCBDN	equ	015C7h	; get device name and unit from UCB
$UGETB	equ	0F36Fh	; inter-bank get byte routine
$UGETW	equ	0F38Dh	; inter-bank get word routine
$UNFTK	equ	0335Ah	; unfix task
$UPUTB	equ	0F37Dh	; inter-bank put byte routine
$UPUTW	equ	0F39Fh	; inter-bank put word routine
$WLIST	equ	054EFh	; ptr to head of waiting tasks list
IODONE	equ	01D4Eh	; end I/O processing
LDRTCB	equ	05502h	; LDR... TCB
MCRTCB	equ	05500h	; MCR... TCB
NVOK	equ	04C65h	; P112 NV RAM OK flag
NVRAM	equ	04C4Dh	; P112 NV RAM contents
SETFLG	equ	01115h	; set task's event flag
SFLAGS	equ	05506h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	054C8h	; start of system data area
SYSEND	equ	0559Eh	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	054DBh	; top of pool area
SYSTYP	equ	054D0h	; system type
SYSVER	equ	054CEh	; system version number
TICCNT	equ	0F3B4h	; absolute time count in ticks
TICSEC	equ	0F3B3h	; ticks per second
TKILL	equ	00BFAh	; abort task
UPTIME	equ	0F3B8h	; system up time (ticks and seconds)
