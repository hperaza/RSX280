; Task Notification Message structure

TN.NAME	equ	0		; task name (6 bytes)
TN.RSN	equ	TN.NAME + 6	; reason (1 byte)
TN.CON	equ	TN.RSN + 1	; task terminal device and unit (3 bytes)
TN.TIME	equ	TN.CON + 3	; timestamp (7 bytes)
TN.REGS	equ	TN.TIME + 7	; task register area (24 bytes)

; Task Notification Message size

TKNSZ	equ	TN.REGS + 24

; Notification Message codes

TK.NONE	equ	0	; normal task exit: TKTN not requested, no msg output
TK.CLI	equ	1	; task aborted via directive or CLI
TK.IIT	equ	2	; illegal instruction trap
TK.RST	equ	3	; unvectored RST execution
TK.LDR	equ	4	; load failure, device I/O error
TK.ITH	equ	5	; invalid task header
TK.AST	equ	6	; bad AST stack
TK.SST	equ	7	; bad SST stack
TK.MEM	equ	8	; not enough pool for new TCB, etc.
TK.RUN	equ	9	; task failed to start execution
TK.PROT	equ	10	; memory protect violation
TK.CAF	equ	11	; checkpoint allocation failure
TK.PRV	equ	12	; privileged instruction trap
TK.EXT	equ	13	; extended instruction trap
TK.DIV	equ	14	; division exception trap
TK.SSB	equ	15	; ss-bit or breakpoint trap

TK.EIO	equ	80h	; task exited with outstanding I/O (ORed with above codes)

