	.enable substitution
	.disable display
	.sets fname p1
	.if fname = "" .asks fname Enter file name: 
	.openr #1 'fname'
.loop:
	.read #1 line
	.ift <eof> .goto done
	;'line'
	.goto loop
.done:
	.close #1
