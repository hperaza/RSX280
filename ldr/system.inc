$ALLOC	equ	01378h	; allocate pool memory
$ALUN	equ	00FB3h	; assign LUN to device
$ASGN	equ	00E26h	; assign logical name to physical device
$ASNTP	equ	021FDh	; assign next task to partition
$AVAIL	equ	01461h	; compute available pool memory
$CHKPT	equ	0239Ah	; initiate task checkpoint
$CLASN	equ	00E98h	; delete all logical dev assignments of the given type
$CLIST	equ	050A9h	; list of CLI control blocks
$CLKQ	equ	050A7h	; ptr to head of clock queue
$CONN	equ	01102h	; connect to task
$CPCB	equ	050A5h	; address of PCB of current task
$CPLST	equ	0509Bh	; ptr to head of checkpoint PCB list
$CTCB	equ	050A3h	; address of TCB of current task
$DBMON	equ	0F406h	; debugger-monitor main entry point
$DBTRP	equ	0F65Dh	; debugger-monitor bpt entry
$DEASN	equ	00E85h	; delete logical device assignment
$DMTRP	equ	0F6D7h	; debugger-monitor bpt entry with message
$ERSEQ	equ	050B6h	; error sequence number
$FEAT	equ	05083h	; feature bits
$FILBF	equ	050D5h	; ptr to file I/O buffer area
$FIXTK	equ	02EEBh	; fix task in memory
$FNDEV	equ	00ED7h	; find device and get UCB address
$FNPAR	equ	021B0h	; find partition by name
$FNTCB	equ	00A7Fh	; find TCB by name
$FNTSK	equ	00A57h	; find task by name
$FORKQ	equ	050C1h	; pointer to list of fork blocks
$FREE	equ	013D7h	; free pool memory
$GTUCB	equ	00F9Fh	; get UCB address
$HOSTN	equ	05084h	; host/node name
$IOEND	equ	01827h	; end I/O processing for ASTs
$LDCKR	equ	00544h	; resume task after checkpoint read
$LDCKW	equ	00574h	; queue task to partition after chkpnt write
$LDGO	equ	004B6h	; start task after load
$LGOUT	equ	010D3h	; logout user
$LKTCB	equ	0037Ch	; link TCB to the specified list
$LOGDV	equ	05097h	; ptr to head of logical device assignment list
$LOGIN	equ	0106Fh	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	05093h	; total memory size
$MFLGS	equ	050C0h	; MCR flags
$MVTBL	equ	050C3h	; ptr to head of mounted device list
$PHYDV	equ	05095h	; ptr to head of physical device list
$PLIST	equ	05099h	; ptr to list of partitions
$POLSZ	equ	05091h	; size of pool area
$POOL	equ	0508Fh	; ptr to start of pool area
$REDIR	equ	00F22h	; follow UCB redirection links
$REMTK	equ	02EE2h	; remove task from STD
$RLIST	equ	0509Fh	; ptr to head of ready tasks list
$RNDC	equ	050ABh	; round-robin interval in ticks
$RNDH	equ	050ADh	; highest priority for round-robin scheduling
$RNDL	equ	050AEh	; lowest priority for round-robin scheduling
$SBANK	equ	0F3BFh	; system bank number
$SCRBF	equ	050C5h	; scratch buffer address
$SWPC	equ	050B0h	; swapping interval in ticks
$SWPCT	equ	050B1h	; swapping interval counter
$SWPRI	equ	050AFh	; priority range for swapping
$SWSTK	equ	050D7h	; stub to call kernel routines
$TLIST	equ	0509Dh	; ptr to head of installed tasks list
$UBANK	equ	0F3C1h	; bank of current task
$UCBDN	equ	0105Bh	; get device name and unit from UCB
$UGETB	equ	0F36Fh	; inter-bank get byte routine
$UGETW	equ	0F38Dh	; inter-bank get word routine
$UNFTK	equ	02EF4h	; unfix task
$UPUTB	equ	0F37Dh	; inter-bank put byte routine
$UPUTW	equ	0F39Fh	; inter-bank put word routine
$VALBF	equ	01A20h	; validate buffer (current task)
$VALTB	equ	01A03h	; validate buffer (arbitrary task)
$WLIST	equ	050A1h	; ptr to head of waiting tasks list
IODONE	equ	0180Ah	; end I/O processing
LDRTCB	equ	050B4h	; LDR... TCB
MCRTCB	equ	050B2h	; MCR... TCB
NVOK	equ	047E8h	; P112 NV RAM OK flag
NVRAM	equ	047D0h	; P112 NV RAM contents
SETFLG	equ	00BD1h	; set task's event flag
SFLAGS	equ	050B8h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	0507Ah	; start of system data area
SYSEND	equ	05150h	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	0508Dh	; top of pool area
SYSTYP	equ	05082h	; system type
SYSVER	equ	05080h	; system version number
TICCNT	equ	0F3B4h	; absolute time count in ticks
TICSEC	equ	0F3B3h	; ticks per second
TKILL	equ	006C9h	; abort task
UPTIME	equ	0F3B8h	; system up time (ticks and seconds)
