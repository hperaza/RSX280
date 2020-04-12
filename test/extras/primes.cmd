.;
.;	Calculate prime numbers using the Sieve of Eratosthenes
.;	T. R. Wyant
.;	E. I. DuPont de Nemours
.;	P. O. Box 27001
.;	Richmond, VA 23261
.;	December 4, 1991
.;	Adapted for RSX180 Feb 10, 2017
.;
.;	The upper limit on number of primes is rather arbitrary (the
.;	hundredth prime being only 541). On the other hand, who really
.;	wants to use a .CMD file as a prime number generator anyway?
.;
	.ENABLE SUBSTITUTION
	.SETS MYFILE <FILSPC>
	; 'MYFILE'
	.SETN P$1 2
	.SETN NP 0
	.SETN NM 1
	.ASKN FP Number of primes to find (10 to 100)
	.SETS START <TIME>
.LOOP:
	.INC NM
	.SETN PI 0
.SIEVE:
	.INC PI
	.IF PI > NP .GOTO PRIME
	.IF Q$'PI' > NM .GOTO PRIME
	.SETN REM NM%P$'PI'
	.IF REM = 0 .GOTO LOOP
	.GOTO SIEVE
.PRIME:
	.INC NP
	.SETN P$'NP' NM
	.IF NM < 256 .SETN Q$'NP' NM*NM
	.IF NP < FP .GOTO LOOP
	.SETS FINISH <TIME>
	.SETN PI 0
.DISPLY:
	.INC PI
	.SETN NM P$'PI'
	; 'NM' is prime.
	.IF PI < NP .GOTO DISPLY
	;
	; Finish: 'FINISH'
	;  Start: 'START'
	.EXIT
