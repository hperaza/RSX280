$ALLOC	equ	01371h	; allocate pool memory
$ALUN	equ	00F9Dh	; assign LUN to device
$ASGN	equ	00E10h	; assign logical name to physical device
$ASNTP	equ	02210h	; assign next task to partition
$AVAIL	equ	0145Ah	; compute available pool memory
$CHKPT	equ	023ADh	; initiate task checkpoint
$CLASN	equ	00E82h	; delete all logical dev assignments of the given type
$CLIST	equ	04BE7h	; list of CLI control blocks
$CLKQ	equ	04BE5h	; ptr to head of clock queue
$CONN	equ	010F6h	; connect to task
$CPCB	equ	04BE3h	; address of PCB of current task
$CPLST	equ	04BD9h	; ptr to head of checkpoint PCB list
$CTCB	equ	04BE1h	; address of TCB of current task
$DBMON	equ	0F40Eh	; debugger-monitor main entry point
$DBTRP	equ	0F725h	; debugger-monitor bpt entry
$DEASN	equ	00E6Fh	; delete logical device assignment
$DMTRP	equ	0F79Fh	; debugger-monitor bpt entry with message
$ERSEQ	equ	04BF4h	; error sequence number
$FEAT	equ	04BC1h	; feature bits
$FILBF	equ	04C13h	; ptr to file I/O buffer area
$FIXTK	equ	02EFEh	; fix task in memory
$FNDEV	equ	00EC1h	; find device and get UCB address
$FNPAR	equ	021C3h	; find partition by name
$FNTCB	equ	00A69h	; find TCB by name
$FNTSK	equ	00A41h	; find task by name
$FORKQ	equ	04BFFh	; pointer to list of fork blocks
$FREE	equ	013D0h	; free pool memory
$GTUCB	equ	00F89h	; get UCB address
$HOSTN	equ	04BC2h	; host/node name
$IOEND	equ	0183Ah	; end I/O processing for ASTs
$LDCKR	equ	0054Bh	; resume task after checkpoint read
$LDCKW	equ	0057Bh	; queue task to partition after chkpnt write
$LDGO	equ	004BAh	; start task after load
$LGOUT	equ	010C7h	; logout user
$LKTCB	equ	00380h	; link TCB to the specified list
$LOGDV	equ	04BD5h	; ptr to head of logical device assignment list
$LOGIN	equ	01063h	; login user
$MCOPY	equ	0F305h	; inter-bank memory copy routine
$MEMSZ	equ	04BD1h	; total memory size
$MFLGS	equ	04BFEh	; MCR flags
$MVTBL	equ	04C01h	; ptr to head of mounted device list
$PHYDV	equ	04BD3h	; ptr to head of physical device list
$PLIST	equ	04BD7h	; ptr to list of partitions
$POLSZ	equ	04BCFh	; size of pool area
$POOL	equ	04BCDh	; ptr to start of pool area
$REDIR	equ	00F0Ch	; follow UCB redirection links
$REMTK	equ	02EF5h	; remove task from STD
$RLIST	equ	04BDDh	; ptr to head of ready tasks list
$RNDC	equ	04BE9h	; round-robin interval in ticks
$RNDH	equ	04BEBh	; highest priority for round-robin scheduling
$RNDL	equ	04BECh	; lowest priority for round-robin scheduling
$SBANK	equ	0F3C8h	; system bank number
$SBASE	equ	0F3C6h	; physical system base bank
$SCRBF	equ	04C03h	; scratch buffer address
$SWPC	equ	04BEEh	; swapping interval in ticks
$SWPCT	equ	04BEFh	; swapping interval counter
$SWPRI	equ	04BEDh	; priority range for swapping
$SWSTK	equ	04C15h	; stub to call kernel routines
$TLIST	equ	04BDBh	; ptr to head of installed tasks list
$UBANK	equ	0F3CAh	; bank of current task
$UCBDN	equ	0104Fh	; get device name and unit from UCB
$UGETB	equ	0F376h	; inter-bank get byte routine
$UGETW	equ	0F394h	; inter-bank get word routine
$UNFTK	equ	02F07h	; unfix task
$UPUTB	equ	0F384h	; inter-bank put byte routine
$UPUTW	equ	0F3A6h	; inter-bank put word routine
$VALBF	equ	01A33h	; validate buffer (current task)
$VALTB	equ	01A16h	; validate buffer (arbitrary task)
$WLIST	equ	04BDFh	; ptr to head of waiting tasks list
IODONE	equ	0181Dh	; end I/O processing
LDRTCB	equ	04BF2h	; LDR... TCB
MCRTCB	equ	04BF0h	; MCR... TCB
NVOK	equ	04896h	; P112 NV RAM OK flag
NVRAM	equ	04881h	; P112 NV RAM contents
SETFLG	equ	00BBBh	; set task's event flag
SFLAGS	equ	04BF6h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	04BB8h	; start of system data area
SYSEND	equ	04C8Eh	; end of system image
SYSENT	equ	0F0F6h	; kernel services entry point
SYSLVL	equ	0F254h	; current system level
SYSTOP	equ	04BCBh	; top of pool area
SYSTYP	equ	04BC0h	; system type
SYSVER	equ	04BBEh	; system version number
TICCNT	equ	0F3BBh	; absolute time count in ticks
TICSEC	equ	0F3BAh	; ticks per second
TKILL	equ	006C0h	; abort task
UPTIME	equ	0F3BFh	; system up time (ticks and seconds)
