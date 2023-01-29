$ALLOC	equ	013F8h	; allocate pool memory
$ALUN	equ	01010h	; assign LUN to device
$ASGN	equ	00E83h	; assign logical name to physical device
$ASNTP	equ	022C3h	; assign next task to partition
$AVAIL	equ	014E1h	; compute available pool memory
$CHKPT	equ	02460h	; initiate task checkpoint
$CLASN	equ	00EF5h	; delete all logical dev assignments of the given type
$CLIST	equ	051FAh	; list of CLI control blocks
$CLKQ	equ	051F8h	; ptr to head of clock queue
$CONN	equ	01169h	; connect to task
$CPCB	equ	051F6h	; address of PCB of current task
$CPLST	equ	051ECh	; ptr to head of checkpoint PCB list
$CTCB	equ	051F4h	; address of TCB of current task
$DBMON	equ	0F42Ch	; debugger-monitor main entry point
$DBTRP	equ	0F743h	; debugger-monitor bpt entry
$DEASN	equ	00EE2h	; delete logical device assignment
$DMTRP	equ	0F7BDh	; debugger-monitor bpt entry with message
$ERSEQ	equ	05207h	; error sequence number
$FEAT	equ	051D4h	; feature bits
$FILBF	equ	05226h	; ptr to file I/O buffer area
$FIXTK	equ	00652h	; fix task in memory
$FNDEV	equ	00F34h	; find device and get UCB address
$FNPAR	equ	02276h	; find partition by name
$FNTCB	equ	00A7Eh	; find TCB by name
$FNTSK	equ	00A56h	; find task by name
$FORKQ	equ	05212h	; pointer to list of fork blocks
$FREE	equ	01457h	; free pool memory
$GTUCB	equ	00FFCh	; get UCB address
$HOSTN	equ	051D5h	; host/node name
$IOEND	equ	018C1h	; end I/O processing for ASTs
$LDCKR	equ	0054Bh	; resume task after checkpoint read
$LDCKW	equ	0057Bh	; queue task to partition after chkpnt write
$LDGO	equ	004BAh	; start task after load
$LGOUT	equ	0113Ah	; logout user
$LKTCB	equ	00380h	; link TCB to the specified list
$LOGDV	equ	051E8h	; ptr to head of logical device assignment list
$LOGIN	equ	010D6h	; login user
$MCOPY	equ	0F323h	; inter-bank memory copy routine
$MEMSZ	equ	051E4h	; total memory size
$MFLGS	equ	05211h	; MCR flags
$MVTBL	equ	05214h	; ptr to head of mounted device list
$PHYDV	equ	051E6h	; ptr to head of physical device list
$PLIST	equ	051EAh	; ptr to list of partitions
$POLSZ	equ	051E2h	; size of pool area
$POOL	equ	051E0h	; ptr to start of pool area
$REDIR	equ	00F7Fh	; follow UCB redirection links
$REMTK	equ	00859h	; remove task from STD
$RLIST	equ	051F0h	; ptr to head of ready tasks list
$RNDC	equ	051FCh	; round-robin interval in ticks
$RNDH	equ	051FEh	; highest priority for round-robin scheduling
$RNDL	equ	051FFh	; lowest priority for round-robin scheduling
$SBANK	equ	0F3E6h	; system bank number
$SBASE	equ	0F3E4h	; physical system base bank
$SCRBF	equ	05216h	; scratch buffer address
$SWPC	equ	05201h	; swapping interval in ticks
$SWPCT	equ	05202h	; swapping interval counter
$SWPRI	equ	05200h	; priority range for swapping
$SWSTK	equ	05228h	; stub to call kernel routines
$TLIST	equ	051EEh	; ptr to head of installed tasks list
$UBANK	equ	0F3E8h	; bank of current task
$UCBDN	equ	010C2h	; get device name and unit from UCB
$UGETB	equ	0F394h	; inter-bank get byte routine
$UGETW	equ	0F3B2h	; inter-bank get word routine
$UNFTK	equ	0068Eh	; unfix task
$UPUTB	equ	0F3A2h	; inter-bank put byte routine
$UPUTW	equ	0F3C4h	; inter-bank put word routine
$VALBF	equ	01ABAh	; validate buffer (current task)
$VALTB	equ	01A9Dh	; validate buffer (arbitrary task)
$WLIST	equ	051F2h	; ptr to head of waiting tasks list
IODONE	equ	018A4h	; end I/O processing
LDRTCB	equ	05205h	; LDR... TCB
MCRTCB	equ	05203h	; MCR... TCB
NVOK	equ	04939h	; P112 NV RAM OK flag
NVRAM	equ	04921h	; P112 NV RAM contents
SETFLG	equ	00C2Ah	; set task's event flag
SFLAGS	equ	05209h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	051CBh	; start of system data area
SYSEND	equ	052A1h	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F272h	; current system level
SYSTOP	equ	051DEh	; top of pool area
SYSTYP	equ	051D3h	; system type
SYSVER	equ	051D1h	; system version number
TICCNT	equ	0F3D9h	; absolute time count in ticks
TICSEC	equ	0F3D8h	; ticks per second
TKILL	equ	006D4h	; abort task
UPTIME	equ	0F3DDh	; system up time (ticks and seconds)
