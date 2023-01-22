$ALLOC	equ	013F3h	; allocate pool memory
$ALUN	equ	0100Bh	; assign LUN to device
$ASGN	equ	00E7Eh	; assign logical name to physical device
$ASNTP	equ	022BEh	; assign next task to partition
$AVAIL	equ	014DCh	; compute available pool memory
$CHKPT	equ	0245Bh	; initiate task checkpoint
$CLASN	equ	00EF0h	; delete all logical dev assignments of the given type
$CLIST	equ	051F1h	; list of CLI control blocks
$CLKQ	equ	051EFh	; ptr to head of clock queue
$CONN	equ	01164h	; connect to task
$CPCB	equ	051EDh	; address of PCB of current task
$CPLST	equ	051E3h	; ptr to head of checkpoint PCB list
$CTCB	equ	051EBh	; address of TCB of current task
$DBMON	equ	0F42Ch	; debugger-monitor main entry point
$DBTRP	equ	0F743h	; debugger-monitor bpt entry
$DEASN	equ	00EDDh	; delete logical device assignment
$DMTRP	equ	0F7BDh	; debugger-monitor bpt entry with message
$ERSEQ	equ	051FEh	; error sequence number
$FEAT	equ	051CBh	; feature bits
$FILBF	equ	0521Dh	; ptr to file I/O buffer area
$FIXTK	equ	03011h	; fix task in memory
$FNDEV	equ	00F2Fh	; find device and get UCB address
$FNPAR	equ	02271h	; find partition by name
$FNTCB	equ	00A7Dh	; find TCB by name
$FNTSK	equ	00A55h	; find task by name
$FORKQ	equ	05209h	; pointer to list of fork blocks
$FREE	equ	01452h	; free pool memory
$GTUCB	equ	00FF7h	; get UCB address
$HOSTN	equ	051CCh	; host/node name
$IOEND	equ	018BCh	; end I/O processing for ASTs
$LDCKR	equ	0054Bh	; resume task after checkpoint read
$LDCKW	equ	0057Bh	; queue task to partition after chkpnt write
$LDGO	equ	004BAh	; start task after load
$LGOUT	equ	01135h	; logout user
$LKTCB	equ	00380h	; link TCB to the specified list
$LOGDV	equ	051DFh	; ptr to head of logical device assignment list
$LOGIN	equ	010D1h	; login user
$MCOPY	equ	0F323h	; inter-bank memory copy routine
$MEMSZ	equ	051DBh	; total memory size
$MFLGS	equ	05208h	; MCR flags
$MVTBL	equ	0520Bh	; ptr to head of mounted device list
$PHYDV	equ	051DDh	; ptr to head of physical device list
$PLIST	equ	051E1h	; ptr to list of partitions
$POLSZ	equ	051D9h	; size of pool area
$POOL	equ	051D7h	; ptr to start of pool area
$REDIR	equ	00F7Ah	; follow UCB redirection links
$REMTK	equ	03008h	; remove task from STD
$RLIST	equ	051E7h	; ptr to head of ready tasks list
$RNDC	equ	051F3h	; round-robin interval in ticks
$RNDH	equ	051F5h	; highest priority for round-robin scheduling
$RNDL	equ	051F6h	; lowest priority for round-robin scheduling
$SBANK	equ	0F3E6h	; system bank number
$SBASE	equ	0F3E4h	; physical system base bank
$SCRBF	equ	0520Dh	; scratch buffer address
$SWPC	equ	051F8h	; swapping interval in ticks
$SWPCT	equ	051F9h	; swapping interval counter
$SWPRI	equ	051F7h	; priority range for swapping
$SWSTK	equ	0521Fh	; stub to call kernel routines
$TLIST	equ	051E5h	; ptr to head of installed tasks list
$UBANK	equ	0F3E8h	; bank of current task
$UCBDN	equ	010BDh	; get device name and unit from UCB
$UGETB	equ	0F394h	; inter-bank get byte routine
$UGETW	equ	0F3B2h	; inter-bank get word routine
$UNFTK	equ	0301Ah	; unfix task
$UPUTB	equ	0F3A2h	; inter-bank put byte routine
$UPUTW	equ	0F3C4h	; inter-bank put word routine
$VALBF	equ	01AB5h	; validate buffer (current task)
$VALTB	equ	01A98h	; validate buffer (arbitrary task)
$WLIST	equ	051E9h	; ptr to head of waiting tasks list
IODONE	equ	0189Fh	; end I/O processing
LDRTCB	equ	051FCh	; LDR... TCB
MCRTCB	equ	051FAh	; MCR... TCB
NVOK	equ	04930h	; P112 NV RAM OK flag
NVRAM	equ	04918h	; P112 NV RAM contents
SETFLG	equ	00C29h	; set task's event flag
SFLAGS	equ	05200h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	051C2h	; start of system data area
SYSEND	equ	05298h	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F272h	; current system level
SYSTOP	equ	051D5h	; top of pool area
SYSTYP	equ	051CAh	; system type
SYSVER	equ	051C8h	; system version number
TICCNT	equ	0F3D9h	; absolute time count in ticks
TICSEC	equ	0F3D8h	; ticks per second
TKILL	equ	006D4h	; abort task
UPTIME	equ	0F3DDh	; system up time (ticks and seconds)
