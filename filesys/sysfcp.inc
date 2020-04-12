; SYSFCP configuration file

; Number of disk cache buffers

NBUFF	equ	20	; 20 * 512 = 10K

; Event flag numbers

TMOEFN	equ	1	; dummy event flag for mark-time requests
ASTEFN	equ	2	; event flag for data-received and mark-time AST
QIOEFN	equ	3	; event flag for disk I/O
QIOLUN	equ	1	; LUN for disk I/O

 