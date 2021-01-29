; LUNs used for different operations

LUNMD	equ	1	; used for master directory search operations
LUNDR	equ	2	; used for user directory operations
LUNSRC	equ	3	; used for source file in copy operations
LUNDST	equ	4	; used for destination file in copy operations
LUNTI	equ	5	; used for terminal I/O (prompt and entering commands)
LUNCML	equ	6	; used for indirect command files and command prompt
LUNTMP	equ	7	; used for temp file storage

; Switch bit masks

; 1st byte - directory listing switches

S1.LI	equ	0	; list directory
S1.BR	equ	1	; brief mode
S1.FU	equ	2	; full mode
S1.FR	equ	3	; display disk usage statistics
S1.SH	equ	4	; show hidden files
S1.VI	equ	5	; display volume information
S1.ID	equ	6	; display PIP version
S1.NM	equ	7	; suppress certain error messages

; 2nd byte - simple file operations

S2.DE	equ	0	; delete file
S2.PU	equ	1	; purge files
S2.SD	equ	2	; selective delete (used with /DE)
S2.UN	equ	3	; unlock file
S2.PR	equ	4	; set file protection
S2.OW	equ	5	; set file owner
S2.CR	equ	6	; create or extend file size
S2.TR	equ	7	; truncate file

; 3rd and 4th bytes - copy operations

S3.CO	equ	0	; make output file contiguous
S3.VF	equ	1	; verify copy operation
S3.VE	equ	2	; verbose operation
S3.NV	equ	3	; set new output file version
S3.AP	equ	4	; append to an existing file
S3.ME	equ	5	; merge multiple files into one
S3.SU	equ	6	; supersede output version
S3.BF	equ	7	; set buffer size for copy operations

S4.MV	equ	0	; move file to another directory
S4.RE	equ	1	; rename file
S4.CD	equ	2	; preserve creation date of file copy
S4.FO	equ	3	; preserve output file owner of file copy
S4.LD	equ	4	; list files being deleted (used with /DE)
S4.BL	equ	5	; pre-allocate blocks to contiguous file (used with /CO)

; 5th byte - directory operations and other misc options

S5.MD	equ	0	; create directory
S5.DF	equ	1	; set current directory
S5.WI	equ	2	; wide output for full directory listing
