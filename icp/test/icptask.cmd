; .IFINS/.IFNINS/.IFACT/.IFNACT test
	.enable substitution
	.sets null ""
.loop:
	.asks t Enter task name
	.if t = null .stop
	.ifins 't' .goto 1
; Task not installed
.1:
	.ifnins 't' .goto 2
; Task installed
.2:
	.ifact 't' .goto 3
; Task not active
.3:
	.ifnact 't' .goto 4
; Task active
.4:
	.goto loop
