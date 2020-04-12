; Generic Queue Item Fields

QU.LNK	equ	0		; link to next item in priority list (2 bytes)
QU.PRI	equ	QU.LNK + 2	; priority, same as requesting task (1 byte)
QU.SIZ	equ	QU.PRI + 1	; size of data field (2 bytes)
QU.DAT	equ	QU.SIZ + 2	; data field, variable length

; Header size

QHSZ	equ	QU.DAT

