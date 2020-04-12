	.enable substitution
.start:
	.asks s1 Enter String 1
	.asks s2 Enter String 2
	.if s1 = s2 ; 's1' = 's2'
	.if s1 <> s2 ; 's1' <> 's2'
	.if s1 > s2 ; 's1' > 's2'
	.if s1 >= s2 ; 's1' >= 's2'
	.if s1 < s2 ; 's1' < 's2'
	.if s1 <= s2 ; 's1' <= 's2'
	.ask a Again
	.ift a .goto start
