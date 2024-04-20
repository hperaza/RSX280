; program version

v.major	equ	1
v.minor	equ	2

; cell structure

c.link	equ	0		; must be first
c.row	equ	c.link + 2	; row
c.col	equ	c.row  + 1	; column
c.flag	equ	c.col  + 1	; 'A' = alpha, 'F' = formula
c.rdy	equ	c.flag + 1	; re-calc status, on file = cell width
c.val	equ	c.rdy  + 1	; computed value
c.align	equ	c.val  + 6	; 0 = align left, 1 = align right, 2 = center
c.prec	equ	c.align  + 1	; numeric format and precision
c.len	equ	c.prec + 1	; length of c.form field, including trailing null
c.form	equ	c.len  + 1	; cell contents

csize	equ	c.form + 0	; cell structure size, not counting c.form

; arrow (dir)

a.none	equ	0
a.up	equ	1
a.down	equ	2
a.left	equ	3
a.rght	equ	4

; states

s.wait	equ	1	; wait for keypress
s.curs	equ	2	; cursor
s.cmnd	equ	3	; command
s.form	equ	4	; formulas
s.parm	equ	5	; parameter
s.exec	equ	6	; execute command
s.exef	equ	7	; execute formula
s.exep	equ	8	; execute parameter

; color attributes for color mode

;   0 = black    1 = red      2 = green    3 = yellow
;   4 = blue     5 = magenta  6 = cyan     7 = white

ca.fg	equ	7	; spreadsheet area: white
ca.bg	equ	4	;  on blue

ca.sfg	equ	0	; selected cell: black
ca.sbg	equ	7	;  on white

;ca.hfg	equ	0	; heading: black
;ca.hbg	equ	2	;  on green
ca.hfg	equ	0	; heading: black
ca.hbg	equ	3	;  on yellow

;ca.tfg	equ	7	; cell row/column headers: white
;ca.tbg	equ	1	;  on red
ca.tfg	equ	0	; cell row/column headers: black
ca.tbg	equ	6	;  on cyan

ca.cfg	equ	6	; raw cell contents: cyan
ca.cbg	equ	4	;  on blue

; stack size (increase if long expressions cause a 'stack overflow' error)

stksiz	equ	256

