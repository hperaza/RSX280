;	**********************************************************
;	*                                                        *
;	*     Test for the RSX180 Indirect Command Processor     *
;	*                                                        *
;	**********************************************************
;
	.ENABLE SUBSTITUTION
	.SETS MYFILE <FILSPC>
;
; 1a) Simple .SETT, .SETF, .SETN, .SETL, .SETS
;
	.SETT LT
	; LT = 'LT' (expected T)
	.SETF LF
	; LF = 'LF' (expected F)
	.SETN N1 123
	; N1 = 'N1' (expected 123)
	.SETN N2 -456
	; N2 = 'N2%S' (expected -456)
	.SETL LL 1
	; LL = 'LL' (expected T)
	.SETL LL 0
	; LL = 'LL' (expected F)
	.SETS STR "A string"
	; STR = "'STR'" (expected "A string")
;
; 1b) .SETN, .SETL and .SETS with expressions
;
	.SETN N1 2+3*5
	; N1 = 'N1' (expected 17)
	.SETN N2 2*3+5
	; N2 = 'N2' (expected 11)
	.SETN N3 (2+3)*5
	; N3 = 'N3' (expected 25)
	.SETN N4 N1-N3/5+N2
	; N4 = 'N4' (expected 23)
	.SETL L1 N1-N3/5+N2
	; L1 = 'L1' (expected T)
	.SETL L2 7&8
	; L2 = 'L2' (expected F)
	.SETL L3 7!8
	; L3 = 'L3' (expected T)
	.SETS STR STR+" with spaces"&"."
	; STR = "'STR'" (expected "A string with spaces.")
	.SETS QS "String with embedded ""quotes"""
	; QS = "'QS'" (expected "String with embedded "quotes"")
;
; 2a) .ASK, .ASKN and .ASKS without options
;
	.ASK L
	; Answer: L = 'L'
	.ASKN N
	; Answer: N = 'N'
	.ASKS S
	; Answer: S = "'S'" <STRLEN>='<STRLEN>'
;
; 2b) .ASK, .ASKN and .ASKS with prompt string
;
	.ASK L Yes or No
	; Answer: L = 'L'
	.ASKN N Enter a number
	; Answer: N = 'N'
	.ASKS S Enter a string
	; Answer: S = "'S'" <STRLEN>='<STRLEN>'
;
; 2c) .ASK, .ASKN and .ASKS with options
;
	.SETN TMO 10
	.ASK [L] L
	; Answer: L = 'L'
	.ASK [<TRUE>:'TMO'S] L Yes or No
	; Answer: L = 'L'
	.SETN N 10
	.ASKN [N:N+10:N+5:5S] N Enter a number
	; Answer: N = 'N', <TIMOUT>='<TIMOUT>', <DEFAUL>='<DEFAUL>'
	.ASKN [:::5S] N Enter another number
	; Answer: N = 'N', <TIMOUT>='<TIMOUT>', <DEFAUL>='<DEFAUL>'
	.ASKN [1:10] N And another
	; Answer: N = 'N'
	.SETS DEFS "Default string"
	.ASKS [10:20:DEFS:20S] S Enter a string
	; Answer: S = "'S'" <STRLEN>='<STRLEN>'
	.ASKS [::"Hello"] S Enter another string
	; Answer: S = "'S'" <STRLEN>='<STRLEN>', <TIMOUT>='<TIMOUT>', <DEFAUL>='<DEFAUL>'
;
; 3a) .GOTO test (forward): skip comment lines, nothing should be printed
;     between the vvvv and ^^^^ lines.
	; vvvv
	.GOTO skip
	; THIS TEXT SHOULD NOT APPEAR
.skip:
	; ^^^^
;
; 3b) .GOTO test (backwards): print in a loop numbers from 1 to 10
;
	.SETN i 1
.loop:
;	i = 'i'
	.INC i
	.IF i <= 10 .GOTO loop
;
; 4) .PARSE test
;
	.SETS cmd "TESTFILE IND,MCR,,LOA"
	.PARSE cmd " ," file a1 a2 a3 a4 a5
	; "'cmd'" -> "'file'", "'a1'", "'a2'", "'a3'", "'a4'", "'a5'"  <STRLEN>='<STRLEN>'
	.PARSE "dy:[dir]myfile.ext;5" "[].;" dev dir file ext ver
	; dy:[dir]myfile.ext;5 => 'dev'['dir']'file'.'ext';'ver'  <STRLEN>='<STRLEN>'
	;   device    = 'dev'
	;   directory = 'dir'
	;   file name = 'file'
	;   extension = 'ext'
	;   version   = 'ver'
	;
	.SETS TM "'<TIME>'"
	.PARSE TM ":" HH MM SS
	;   current time = 'TM' -> 'HH' hours, 'MM' minutes, 'SS' seconds
;
; 5) .INC and .DEC test
;
	.ENABLE OVERFLOW
	.ASKN n Enter a number
	.SETN n1 n
	.INC n
	; After increment N='n'
	.SETN n n1
	.DEC n
	; After decrement N='n'
	.DISABLE OVERFLOW
;
; 6) .GOSUB and .RETURN
;
; 6a) Single call test
;
	.GOSUB sub1
;
; 6b) Nested call
;
	.GOSUB sub2
	.GOTO continue
.sub1:
	; ***** on sub 1
	.RETURN
.sub2:
	; ***** on sub 2 before call to sub 1
	.GOSUB sub1
	; ***** on sub 2 after call to sub 1
	.RETURN
.continue:
;
; 6c) Passing arguments
;
	.SETS expect "abc def ghi"
	.GOSUB sub3 abc def ghi
	.SETS expect S
	.GOSUB sub3 'S'
	.SETS expect "123"
	.GOSUB sub3 123!456
	.GOTO cont2
.sub3:
	; **** on sub 3, got 'COMMAN' ('expect' expected)
	.RETURN
.cont2:
;
; 7) .IF test
;
	.ASKN n1 Enter first number
	.ASKN n2 Enter second number
	;
	.IF n1 = n2  ; 'n1' = 'n2'
	.IF n1 < n2  ; 'n1' < 'n2'
	.IF n1 > n2  ; 'n1' > 'n2'
	.IF n1 <= n2 ; 'n1' <= 'n2'
	.IF n1 >= n2 ; 'n1' >= 'n2'
	.IF n1 <> n2 ; 'n1' <> 'n2'
	;
	.ASKS s1 Enter first string
	.ASKS s2 Enter second string
	;
	.IF s1 = s2  ; 's1' = 's2'
	.IF s1 <> s2 ; 's1' <> 's2'
	.IF s1 > s2  ; 's1' > 's2'
	.IF s1 >= s2 ; 's1' >= 's2'
	.IF s1 < s2  ; 's1' < 's2'
	.IF s1 <= s2 ; 's1' <= 's2'
;
; 7a) .IFT/.IFF
;
	.IFT LT ; 'LT' is True
	.IFF LT ; 'LT' is False
;
; 7b) .IFDF/IFNDF
;
	.IFDF n1 ; n1 is defined and equals 'n1'
	.IFNDF n1 ; n1 is undefined
	.IFDF n8 ; n8 is defined and equals 'n8'
	.IFNDF n8 ; n8 is undefined
;
; 7c) .IFENABLED/.IFDISABLED
;
	.IFENABLED SUBSTITUTION	; SUBSTITUTION is enabled
	.IFDISABLED SUBSTITUTION; SUBSTITUTION is disabled
	.ENABLE GLOBAL
	.IFENABLED GLOBAL .DISABLE GLOBAL
	.IFENABLED GLOBAL	; GLOBAL is enabled (wrong)
	.IFDISABLED GLOBAL	; GLOBAL is disabled (correct)
;
; 7d) .IFINS/.IFNINS
;
	.IFINS  ...RMD ; Task ...RMD is installed
	.IFNINS ...RMD ; Task ...RMD is not installed
;
; 7e) .IFACT/.IFNACT
;
	.SETS TASK "...RMD"
	.IFACT  'TASK' ; Task 'TASK' is active
	.IFNACT 'TASK' ; Task 'TASK' is inactive
;
; 8) Substitution test
;
	.SETS str "This is a test string"
	.SETN num -10
; 8a) Numeric output
;
	;     Decimal (default):              >'num'<
	;     Decimal signed:                 >'num%S'<
	;     Hexadecimal:                    >'num%H'<
	;     Octal:                          >'num%O'<
	;     Left-justified:                 >'num%L10'<
	;     Right-justified:                >'num%R10'<
	;     Right-justified with zero fill: >'num%DZR10'<
;
; 8b) String output
;
	;     Default:                        >'str'<
	;     Left-justified:                 >'str%L30'<
	;     Right-justified:                >'str%R30'<
	;     Truncated:                      >'str%L10'<
	;     Condensed:                      >'str%C'<
	;     Compressed:                     >'str%B'<
;
; 8c) Numeric as char output
;
	.SETN esc 27
	.SETN nc 65
	;     ASCII value of 'nc' corresponds to char 'nc%V'
	;     'esc%V'[1mBold 'esc%V'[0mNormal 'esc%V'[4mUnderline'esc%V'[0m
;
; 8d) String char output as numeric
;
	.SETS sc "A"
	;     Char 'sc' has an ASCII value of 'sc%V'
;
; 9) .TEST
;
	; Testing 'L'
	.TEST L
	.GOSUB tstvar
	; Testing 'N'
	.TEST N
	.GOSUB tstvar
	; Testing 'S'
	.TEST S
	.GOSUB tstvar
	.GOTO tstskip
.tstvar:
	.IF <SYMTYP> = 0 ; variable is logical
	.IF <SYMTYP> = 2 ; variable is numeric
	.IF <SYMTYP> = 4 ; variable is string
	.IF <SYMTYP> <> 4 .GOTO tstret
	.IFT <ALPHAN> ; * string contains alphanumeric characters
	.IFT <NUMBER> ; * string contains only numeric characters
	              ; * string length is '<STRLEN>'
.tstret:
	.RETURN
.tstskip:
;
; 9a) substring search with .TEST
;
	.SETS S1 "ABCDEF"
	.SETS S2 "DE"
	.TEST S1 S2
	.IF <STRLEN> NE 0 ; "'S1'" contains "'S2'" at position '<STRLEN>'
	.IF <STRLEN> EQ 0 ; "'S2'" was not found in "'S1'"
;
; 10) .BEGIN - .END blocks
;
; 10a) Testing local labels
;
	.ENABLE GLOBAL
	.SETN $LEVEL 0
	.GOTO lab1

	.BEGIN
.lab1:	; THIS LINE SHOULD NEVER APPEAR
	.INC $LEVEL
	.END

.lab1:	; This line should appear only once
	.INC $LEVEL

	.BEGIN
	.GOTO lab1
	; THIS LINE SHOULD NEVER APPEAR
	.INC $LEVEL
.lab1:
	.END
	
	.IF $LEVEL = 1  ; Test passed
	.IF $LEVEL <> 1	; Something went wrong
;
; 10b) Testing local variables
;
	.BEGIN
	.SETN LV1 100
	.END
	
	.IFNDF LV1 ; Test passed
	.IFDF LV1  ; Something went wrong
;
; 10c) Testing .EXIT within block
;
	.SETN $LEVEL 0
	.BEGIN
	; This line should appear
	.EXIT 5
	; THIS LINE SHOULD NEVER APPEAR
	.INC $LEVEL
	.END

	.IF $LEVEL = 0	; Test passed, <EXSTAT> = '<EXSTAT>' (5 expected)
	.IF $LEVEL <> 0	; Something went wrong
;
; 11) File I/O test
;
; 11a) .OPEN and .DATA
;
	.OPEN #1 TEST
	.SETS S1 "Hello, world!"
	; Writing "'S1'" to file '<FILSPC>'
	.DATA #1 'S1'
	.CLOSE #1
;
; 11b) .OPENA and .ENABLE DATA
;
	.OPENA #1 TEST
	.SETS S1 "Test from AT."
	; Appending "'S1'" to file '<FILSPC>'
.ENABLE DATA #1
'S1'
.DISABLE DATA
	.CLOSE #1
;
; 11c) .OPENR and .READ
;
	.OPENR #1 TEST
	.READ #1 S2
	; "'S2'" read from file '<FILSPC>'
	.READ #1 S2
	; "'S2'" read from file '<FILSPC>'
	.CLOSE #1
;
; 12) .TESTFILE
;
	.TESTFILE 'MYFILE'
	; This file is '<FILSPC>', <FILERR>='<FILERR>%S'
	.ASKS NAME Enter file name
	.TESTFILE 'NAME'
	; <FILSPC>='<FILSPC>', <FILERR>='<FILERR>%S'
	.ASKS NAME Enter device name
	.TESTFILE 'NAME'
	; <FILSPC>='<FILSPC>', <FILERR>='<FILERR>%S'
;
; 13) .TESTDEVICE
;
	.TESTDEVICE SY:
	.PARSE <EXSTRI> "," NAME UST UCW FLAGS
	.SETN IST 'UST'
	.SETN ICW 'UCW'
	; Testing device SY:
	;   Physical name        = 'NAME'
	;   Status word          = 'IST%HZR4'h
	;   Characteristics word = 'ICW%HZR4'h
	;   Flags                = 'FLAGS'
;
; 14) .TESTPARTITION
;
	.TESTPARTITION *
	.PARSE <EXSTRI> "," NAME BASE SIZE TYPE
	.SETN IBASE 'BASE'
	.SETN ISIZE 'SIZE'*4
	; This task runs in partition "'NAME'":
	;   Partition Base address = 'IBASE%HZR2'000h
	;   Partition Size         = 'ISIZE'K
	;   Partition Type         = 'TYPE'
;
; 15) .DELAY
;
	; Sleeping for 5 seconds
	.DELAY 5S
;
; 16) .PAUSE
;
	.PAUSE
;
; 17) .ONERR
;
	.ONERR errtrp
	; Forcing a "Label not found" error
	.GOTO badlabel
	; THIS LINE SHOULD NOT APPEAR
	.BEGIN
	; THIS LINE SHOULD NOT APPEAR EITHER
	.END
	.GOTO cont2
.errtrp:
	; Error trap entered:
	;   Error number   is '<ERRNUM>' (11 expected)
	;   Error severity is '<ERRSEV>' (1 expected)
.cont2:	; Continuing after trap...
;
; This file doesn''t test:
;
;   .CHAIN filename
;   .ERASE [LOCAL|GLOBAL|SYMBOL name]
;   .STOP [value]
;   .WAIT taskname
;   .XQT taskname args ...
;   nested command files
;
; Several special variables are also missing:
;   <MEMSIZ> <LOGDEV> <FILATR>
;
.;.DEBUG
