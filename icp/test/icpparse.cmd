; .PARSE test
.enable substitution
.sets command "TESTFILE IND,MCR,,LOA"
.parse command " ," file a1 a2 a3 a4 a5
; "'command'" -> "'file'", "'a1'", "'a2'", "'a3'", "'a4'", "'a5'"
.parse "dy:[dir]fname.ext;ver" "[].;" dev dir file ext ver
; 'dev'['dir']'file'.'ext';'ver'
