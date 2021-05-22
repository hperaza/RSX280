[master]system
set /host=Z280RC		! set host name
set /par=syspar:0:15:task
set /par=syscom:*:1:task
set /par=ldrpar:*:1:task	! create 4K partition for loader
ins ldr				! install loader
fix ldr...			! fix loader in memory
set /par=fcppar:*:3:task	! create 12K partition for filesystem task
;set /par=par20k:*:5:sys
set /par=gen:*:*:sys		! everything else goes to GEN partition
ins sysfcp/acp=yes		! install filesystem task
ins tkn				! install task termination task
fix tktn
ins cot/acp=yes/cli=yes		! install console logger
;ins sav
ins mcr/ckp=no			! install command processor
ins sys				! install display part of command processor
ins ins				! install install
ins acs				! install allocate checkpoint file
ins icp				! install indirect command processor
ins hel				! install login processor
ins bye				! install logout processor
ins shf/ckp=no			! install shuffler
fix shf...
ins mou				! install mount
ins dmo				! install dismount
ins ufd				! install user file directory builder
ins ini				! install volume initialization task
ins rmd				! install resource monitoring display task
ins pip				! install pip
ins bro				! install broadcast task
ins mac				! install mac
ins tkb/inc=30000		! install task builder
ins lbr				! install object module librarian
ins zap				! install hex edtor
ins who
ins dmp				! install file dump utility
ins mce				! install command line editor
ins uptime
ins vdo				! install text editor
ins view/task=...mor		! install file viewer
ins basic/inc=15000		! install basic interpreter
ins ccl/task=...ca.		! install ccl as catch-all task
ins ted/inc=10000/pri=65	! install large-file text editor
ins md5
ins dcu
ins cpu
ins cal				! install calendar display
ins acnt/task=...pwd		! install user password change utility
;set /speed=tt0:115200
set /speed=tt1:19200
set /speed=tt2:19200
set /speed=tt3:19200
set /speed=tt4:19200
;set /speed=tt0:
set /speed=tt1:
set /speed=tt2:
set /speed=tt3:
set /speed=tt4:
set /lower=tt0:
set /lower=tt1:
set /crt=tt0:
set /crt=tt1:
set /logon			! enable user logins
dev
par
tas
set /pool
set /host
