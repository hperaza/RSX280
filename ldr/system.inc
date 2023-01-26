$ALLOC	equ	013F4h	; allocate pool memory
$ALUN	equ	0100Ch	; assign LUN to device
$ASGN	equ	00E7Fh	; assign logical name to physical device
$ASNTP	equ	022BFh	; assign next task to partition
$AVAIL	equ	014DDh	; compute available pool memory
$CHKPT	equ	0245Ch	; initiate task checkpoint
$CLASN	equ	00EF1h	; delete all logical dev assignments of the given type
$CLIST	equ	051F6h	; list of CLI control blocks
$CLKQ	equ	051F4h	; ptr to head of clock queue
$CONN	equ	01165h	; connect to task
$CPCB	equ	051F2h	; address of PCB of current task
$CPLST	equ	051E8h	; ptr to head of checkpoint PCB list
$CTCB	equ	051F0h	; address of TCB of current task
$DBMON	equ	0F42Ch	; debugger-monitor main entry point
$DBTRP	equ	0F743h	; debugger-monitor bpt entry
$DEASN	equ	00EDEh	; delete logical device assignment
$DMTRP	equ	0F7BDh	; debugger-monitor bpt entry with message
$ERSEQ	equ	05203h	; error sequence number
$FEAT	equ	051D0h	; feature bits
$FILBF	equ	05222h	; ptr to file I/O buffer area
$FIXTK	equ	00652h	; fix task in memory
$FNDEV	equ	00F30h	; find device and get UCB address
$FNPAR	equ	02272h	; find partition by name
$FNTCB	equ	00A7Eh	; find TCB by name
$FNTSK	equ	00A56h	; find task by name
$FORKQ	equ	0520Eh	; pointer to list of fork blocks
$FREE	equ	01453h	; free pool memory
$GTUCB	equ	00FF8h	; get UCB address
$HOSTN	equ	051D1h	; host/node name
$IOEND	equ	018BDh	; end I/O processing for ASTs
$LDCKR	equ	0054Bh	; resume task after checkpoint read
$LDCKW	equ	0057Bh	; queue task to partition after chkpnt write
$LDGO	equ	004BAh	; start task after load
$LGOUT	equ	01136h	; logout user
$LKTCB	equ	00380h	; link TCB to the specified list
$LOGDV	equ	051E4h	; ptr to head of logical device assignment list
$LOGIN	equ	010D2h	; login user
$MCOPY	equ	0F323h	; inter-bank memory copy routine
$MEMSZ	equ	051E0h	; total memory size
$MFLGS	equ	0520Dh	; MCR flags
$MVTBL	equ	05210h	; ptr to head of mounted device list
$PHYDV	equ	051E2h	; ptr to head of physical device list
$PLIST	equ	051E6h	; ptr to list of partitions
$POLSZ	equ	051DEh	; size of pool area
$POOL	equ	051DCh	; ptr to start of pool area
$REDIR	equ	00F7Bh	; follow UCB redirection links
$REMTK	equ	00859h	; remove task from STD
$RLIST	equ	051ECh	; ptr to head of ready tasks list
$RNDC	equ	051F8h	; round-robin interval in ticks
$RNDH	equ	051FAh	; highest priority for round-robin scheduling
$RNDL	equ	051FBh	; lowest priority for round-robin scheduling
$SBANK	equ	0F3E6h	; system bank number
$SBASE	equ	0F3E4h	; physical system base bank
$SCRBF	equ	05212h	; scratch buffer address
$SWPC	equ	051FDh	; swapping interval in ticks
$SWPCT	equ	051FEh	; swapping interval counter
$SWPRI	equ	051FCh	; priority range for swapping
$SWSTK	equ	05224h	; stub to call kernel routines
$TLIST	equ	051EAh	; ptr to head of installed tasks list
$UBANK	equ	0F3E8h	; bank of current task
$UCBDN	equ	010BEh	; get device name and unit from UCB
$UGETB	equ	0F394h	; inter-bank get byte routine
$UGETW	equ	0F3B2h	; inter-bank get word routine
$UNFTK	equ	0068Eh	; unfix task
$UPUTB	equ	0F3A2h	; inter-bank put byte routine
$UPUTW	equ	0F3C4h	; inter-bank put word routine
$VALBF	equ	01AB6h	; validate buffer (current task)
$VALTB	equ	01A99h	; validate buffer (arbitrary task)
$WLIST	equ	051EEh	; ptr to head of waiting tasks list
IODONE	equ	018A0h	; end I/O processing
LDRTCB	equ	05201h	; LDR... TCB
MCRTCB	equ	051FFh	; MCR... TCB
NVOK	equ	04935h	; P112 NV RAM OK flag
NVRAM	equ	0491Dh	; P112 NV RAM contents
SETFLG	equ	00C2Ah	; set task's event flag
SFLAGS	equ	05205h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	051C7h	; start of system data area
SYSEND	equ	0529Dh	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F272h	; current system level
SYSTOP	equ	051DAh	; top of pool area
SYSTYP	equ	051CFh	; system type
SYSVER	equ	051CDh	; system version number
TICCNT	equ	0F3D9h	; absolute time count in ticks
TICSEC	equ	0F3D8h	; ticks per second
TKILL	equ	006D4h	; abort task
UPTIME	equ	0F3DDh	; system up time (ticks and seconds)
