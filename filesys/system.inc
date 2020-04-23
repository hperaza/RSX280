$ALLOC	equ	018B9h	; allocate pool memory
$ALUN	equ	01507h	; assign LUN to device
$ASGN	equ	0137Ah	; assign logical name to physical device
$ASNTP	equ	02665h	; assign next task to partition
$CHKPT	equ	02806h	; initiate task checkpoint
$CLASN	equ	013ECh	; delete all logical dev assignments of the given type
$CLIST	equ	04989h	; list of CLI control blocks
$CLKQ	equ	04987h	; ptr to head of clock queue
$CONN	equ	01655h	; connect to task
$CPCB	equ	04985h	; address of PCB of current task
$CPLST	equ	0497Bh	; ptr to head of checkpoint PCB list
$CTCB	equ	04983h	; address of TCB of current task
$DBMON	equ	0F336h	; debugger-monitor main entry point
$DBTRP	equ	0F58Dh	; debugger-monitor bpt entry
$DEASN	equ	013D9h	; delete logical device assignment
$DMTRP	equ	0F607h	; debugger-monitor bpt entry with message
$ERSEQ	equ	04996h	; error sequence number
$FEAT	equ	04963h	; feature bits
$FILBF	equ	049B5h	; ptr to file I/O buffer area
$FIXTK	equ	0322Eh	; fix task in memory
$FNDEV	equ	0142Bh	; find device and get UCB address
$FNPAR	equ	02619h	; find partition by name
$FNTCB	equ	00FA7h	; find TCB by name
$FNTSK	equ	00F7Fh	; find task by name
$FORKQ	equ	049A1h	; pointer to list of fork blocks
$FREE	equ	01918h	; free pool memory
$GTUCB	equ	014F3h	; get UCB address
$HOSTN	equ	04964h	; host/node name
$IOEND	equ	01D02h	; end I/O processing for ASTs
$LDCKR	equ	00A42h	; resume task after checkpoint read
$LDCKW	equ	00A72h	; queue task to partition after chkpnt write
$LDGO	equ	009BEh	; start task after load
$LGOUT	equ	01626h	; logout user
$LKTCB	equ	00884h	; link TCB to the specified list
$LOGDV	equ	04977h	; ptr to head of logical device assignment list
$LOGIN	equ	015C3h	; login user
$MCOPY	equ	0F21Eh	; inter-bank memory copy routine
$MEMSZ	equ	04973h	; total memory size
$MFLGS	equ	049A0h	; MCR flags
$MVTBL	equ	049A3h	; ptr to head of mounted device list
$PHYDV	equ	04975h	; ptr to head of physical device list
$PLIST	equ	04979h	; ptr to list of partitions
$POLSZ	equ	04971h	; size of pool area
$POOL	equ	0496Fh	; ptr to start of pool area
$REDIR	equ	01476h	; follow UCB redirection links
$REMTK	equ	03225h	; remove task from STD
$RLIST	equ	0497Fh	; ptr to head of ready tasks list
$RNDC	equ	0498Bh	; round-robin interval in ticks
$RNDH	equ	0498Dh	; highest priority for round-robin scheduling
$RNDL	equ	0498Eh	; lowest priority for round-robin scheduling
$SBANK	equ	0F2F0h	; system bank number
$SCRBF	equ	049A5h	; scratch buffer address
$SWPC	equ	04990h	; swapping interval in ticks
$SWPCT	equ	04991h	; swapping interval counter
$SWPRI	equ	0498Fh	; priority range for swapping
$SWSTK	equ	049B7h	; stub to call kernel routines
$TLIST	equ	0497Dh	; ptr to head of installed tasks list
$UBANK	equ	0F2F2h	; bank of current task
$UCBDN	equ	015AFh	; get device name and unit from UCB
$UGETB	equ	0F298h	; inter-bank get byte routine
$UGETW	equ	0F2BAh	; inter-bank get word routine
$UNFTK	equ	03237h	; unfix task
$UPUTB	equ	0F2A8h	; inter-bank put byte routine
$UPUTW	equ	0F2CEh	; inter-bank put word routine
$WLIST	equ	04981h	; ptr to head of waiting tasks list
IODONE	equ	01CE2h	; end I/O processing
LDRTCB	equ	04994h	; LDR... TCB
MCRTCB	equ	04992h	; MCR... TCB
NVOK	equ	04510h	; P112 NV RAM OK flag
NVRAM	equ	044F8h	; P112 NV RAM contents
SETFLG	equ	010FDh	; set task's event flag
SFLAGS	equ	04998h	; system flags
SYSCOM	equ	0F000h	; start of common area
SYSDAT	equ	0495Ah	; start of system data area
SYSEND	equ	04A31h	; end of system image
SYSENT	equ	0F0B4h	; kernel services entry point
SYSLVL	equ	0F172h	; current system level
SYSTOP	equ	0496Dh	; top of pool area
SYSTYP	equ	04962h	; system type
SYSVER	equ	04960h	; system version number
TICCNT	equ	0F2E5h	; absolute time count in ticks
TICSEC	equ	0F2E4h	; ticks per second
TKILL	equ	00BEAh	; abort task
UPTIME	equ	0F2E9h	; system up time (ticks and seconds)
