	.enable substitution
.start:
	.askn n1 Enter N1
	.askn n2 Enter N2
	.if n1 = n2 ; 'N1' = 'N2'
	.if n1 <> n2 ; 'N1' <> 'N2'
	.if n1 > n2 ; 'N1' > 'N2'
	.if n1 >= n2 ; 'N1' >= 'N2'
	.if n1 < n2 ; 'N1' < 'N2'
	.if n1 <= n2 ; 'N1' <= 'N2'
	.ask a Again
	.ift a .goto start
