.;
.; MCEBLD.CMD -- Command file to build the MCE Command Line Editor
.;
        .Sets VERS 	"V4.29"
.;
.;	Port of the DECUS RSX-11M MCE version V4.28
.;	Original MCE is Copyright (C) 1987-1998 J. H. Hamakers,
.;	pAkUiT International
.;
        .Enable Global
        .Enable Substitution
        .Disable Display
.;
        .Sets IDNPRE	"429"		   ! Ident code for MCEPRE (decimal)
        .Sets SAVFIL    "MCESAVED.DAT"	   ! Saved Answer file
.;
;MCEBLD.CMD -- Command file to build the MCE Command Line Editor 'VERS'
.;
        .If	<Cli> = "DCL" .Setf MCR
        .If	<Cli> = "MCR" .Sett MCR
        .IfDf	MCR	.Goto 10$
        .Disable	Quiet
.;
;MCEBLD -- CLI  "'<Cli>'" not supported
.;
        .Exit	<Severe>
.10$:
.;
        .If P1 = "MAC"	.Goto	50$
        .If P1 = "LINK"	.Goto	50$
        .Disable	Quiet
;
; This Command File builds YOUR version of MCE. It will ask which options
; do you want to be included in MCE. Then it will create a conditional
; assembly definition file named MCEPRE.INC, two command files, MCEASM.CMD
; and MCETKB.CMD, and optionally a MCE.HLP help file.
;
; If you already have created those files, then you can restart MCEBLD using
; the command "@MCEBLD MAC" to assemble and taskbuild, or "@MCEBLD LINK" to
; taskbuild only.
;
;				*****
;
; MCEBLD can use saved answers from a previous session.
;
; MCEBLD can use the saved answers as default and still ask the questions,
; or it can directly use the saved answers and skip the questions for which
; answers have been defined.
; 
.20$:
	.Setf	SAV
	.Ask Q MCEBLD -- Do you have a saved answer file? [D:N]: 
	.Iff Q	.Goto	40$
	.AskS [::SAVFIL] SAVFIL MCEBLD -- Name of the saved answer file [D:'SAVFIL']: 
	.TestFile 'SAVFIL'
	.If <FilErr> Eq 0	.Goto	30$
	.;
	;MCEBLD -- Error: file 'SAVFIL' not found!
	.;
	.Goto	20$
.30$:
        .Ask SAV MCEBLD -- Do you want to skip the questions? [D:N]: 
	@'SAVFIL'
.40$:
;
;				*****
;
; MCEBLD can save the answers for a future session
;
	.Ask SAVOUT MCEBLD -- Do you want to save the answers? [D:N]: 
	.Iff	SAVOUT	.Goto	50$
	.AskS [::SAVFIL] SAVFIL MCEBLD -- Name of the saved answer file [D:'SAVFIL']: 
.50$:
	.IfNdf $INIDR	.Sets	$INIDR "[SYSTEM]" ! Init-File directory
.;						  !  Change MCE.MAC to change
.;						  !    directory
	.IfNdf PLUS	.Setf	PLUS
.;
	.If <System> = 2 .Sett	PLUS	! RSX280
	.Sets	HLPCHR	" ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	.Setn	HLPNR	1
	.Sets	MAXHLP	HLPCHR[HLPNR:HLPNR]
        .If P1 = "MAC"	.Goto	MAC
        .If P1 = "LINK"	.Goto	LINK
;
;				*****
	.IfNdf $CLS	.Goto	55$
	.Ift SAV ;                 -- Multiple CLI support included    : '$CLS'
	.Ift SAV	.Goto	60$
.55$:
;
; The standard CLI (Command Line Interpreter) of RSX180 is MCR.
; If your system supports the use of alternate CLIs such as DCL then
; include Multiple CLI support in MCE.
;
	.IfNdf $CLS	.SetF	$CLS
	.Iff $CLS	.Sets	X "N"
	.Ift $CLS	.Sets	X "Y"
	.Ask [$CLS] $CLS MCEBLD -- Want the Multiple CLI support? [D:'X']: 
;
.60$:
;				*****
	.IfNdf $INI	.Goto	70$
	.Ift SAV ;                 -- MCE Init-File support            : '$INI'
	.Ift SAV	.Goto	140$
.70$:
;
; When Init-File support is included, MCE reads initialization files
	.Iff $CLS ; which can contain Command Definitions for Internal- and MCR-commands.
	.Ift $CLS ; which can contain Command Definitions for Internal- and CLI-commands.
;
; Files are searched in this order:
;
        .Iff $CLS	.Goto	100$
        .Iff PLUS	.Goto	80$
;     LB:'$INIDR'MCEINI.xxx
;     followed by one of the two:
;        either     SY:[CurDir]MCEINI.xxx
;        or         SYS$LOGIN:MCEINI.xxx
;     depending on the next question about the MCE Init-File user default 
;     directory.
	.Goto	90$
.80$:
;     LB:'$INIDR'MCEINI.xxx, followed by SY:[CurDir]MCEINI.xxx,
.90$:
;
; where "xxx" is the name of the CLI defined for the terminal invoking 
; MCE and "CurDir" is the default directory at the startup of MCE.
        .Goto	130$
.100$:
        .Iff PLUS	.Goto	110$
;     LB:'$INIDR'MCEINI.CMD
;     followed by one of the two:
;        either     SY:[CurDir]MCEINI.CMD
;        or         SYS$LOGIN:MCEINI.CMD
;     depending on the next question about the MCE Init-File user default 
;     directory.
	.Goto	120$
.110$:
;     LB:'$INIDR'MCEINI.CMD, followed by SY:[CurDir]MCEINI.CMD,
.120$:
;
; where "CurDir" is the default directory at the startup of MCE.
.130$:
;
; This allows having 2 sets of command definitions:
; A system-wide set in LB:'$INIDR', followed by a user''s private set.
;
	.IfNdf $INI	.Setf	$INI
	.Iff $INI	.Sets	X "N"
	.Ift $INI	.Sets	X "Y"
	.Ask [$INI] $INI MCEBLD -- Want the MCE Init-File support? [D:'X']: 
;
.140$:
	.Iff $INI	.Goto	150$
	.Iff PLUS	.Goto	150$
;				*****
	.IfNdf $SYL	.Goto	145$
	.Ift SAV ;                 -- SYS$LOGIN support included       : '$SYL'
	.Ift SAV	.Goto	150$
.145$:
;
; MCE can read the user''s command definitions from the current disk and
; directory or from the user''s login directory SYS$LOGIN:. MCE will
; check for the presence of Logical Name support before trying to read
; from SYS$LOGIN on your system and fall back to SY:[CurDir] if your
; system has no logical name support.
;
	.IfNdf $SYL	.Setf	$SYL
	.Iff $SYL	.Sets	X "N"
	.Ift $SYL	.Sets	X "Y"
	.Ask [$SYL] $SYL MCEBLD -- Want to read command file from SYS$LOGIN:? [D:'X']: 
;
.150$:
	.IfNdf $SYL	.Setf	$SYL
;				*****
	.IfNdf $CMP	.Goto	160$
	.Ift SAV ;                 -- Compound Command support         : '$CMP'
	.Ift SAV	.Goto	170$
.160$:
;
; MCE allows definitions of Compound Commands.
; Using the ampersand sign (&) one can define an MCE command as a 
	.Iff $CLS	; sequence of multiple MCR commands, e.g.:
	.Ift $CLS	; sequence of multiple CLI commands, e.g.:
;
;       +>STAT := TIM & PIP /LI & ACT /ALL ...etc.
;
; or directly, e.g.:
;
;       +>MAC @TASKASM & TKB @TASKTKB
;
	.IfNdf $CMP	.Setf	$CMP
	.Iff $CMP	.Sets	X "N"
	.Ift $CMP	.Sets	X "Y"
	.Ask [$CMP] $CMP MCEBLD -- Want the Compound Command support? [D:'X']: 
;
.170$:
;				*****
	.IfNdf $STA	.Goto	180$
	.Ift SAV ;                 -- Status line support              : '$STA'
	.Ift SAV	.Goto	190$
.180$:
;
; It is possible to include Status Line support.
; The Status Line shows the different settings and FIFO-parameters of MCE
; and can be switched on/off with the "MCE STATus on/off" command.
; Line 24 of the terminal is used for status information.
;
	.IfNdf $STA	.Setf	$STA
	.Iff $STA	.Sets	X "N"
	.Ift $STA	.Sets	X "Y"
	.Ask [$STA] $STA MCEBLD -- Want Status Line support? [D:'X']: 
.190$:
	.IfNdf $STADF	.Setf	$STADF
	.Iff $STADF	.Sets	X "N"
	.Ift $STADF	.Sets	X "Y"
	.Iff SAV .Ask [$STADF] $STADF MCEBLD -- Want Status Line ON by default? [D:'X']: 
	.Ift SAV .Ift $STA ;                 -- Status Line ON by default        : '$STADF'
	.Iff Sav	;
;				*****
	.IfNdf $SIL	.Goto	200$
	.Ift SAV ;                 -- Startup and Exit messages support: '$SIL'
	.Ift SAV	.Goto	210$
.200$:
;
; The Startup and Exit messages
;
	.Iff $CLS ;     "MCE -- MCR Command Line Editor 'VERS'"   and   "MCE -- Exit"
	.Ift $CLS ;     "MCE -- CLI Command Line Editor 'VERS'"   and   "MCE -- Exit"
;
; are optional.
;
	.IfNdf $SIL	.Setf	$SIL
	.Iff $SIL	.Sets	X "N"
	.Ift $SIL	.Sets	X "Y"
	.Ask [$SIL] $SIL MCEBLD -- Want Startup and Exit messages? [D:'X']: 
;
.210$:
;				*****
	.IfNdf $RTVAL	.Goto	220$
	.IfNdf $HTVAL	.Goto	220$
	.Ift SAV	.Goto	230$
.220$:
;
; When starting MCE from a remote terminal (RT: or HT:) problems may occur
; if the local system is VMS with its command line editor enabled. MCE can
; detect if it was started from a remote terminal and can take one of the
; following 3 actions:
;
;	1. Give the following message:
;
;            MCE -- ** WARNING ** Started on a Remote Terminal.
;                   If your local system is OpenVMS with its command line
;                   editor enabled please type "MCE EXIT"
;
;	2. Give the following message and exit:
;
;            MCE -- Started on a Remote Terminal, exiting...
;
;	3. Do nothing
;
	.IfNdf $RTVAL	.Setn	$RTVAL 1.
        .Askn [1:3:$RTVAL] $RTVAL MCEBLD -- Which option for RT:? [D:'$RTVAL']: 
	.IfNdf $HTVAL	.Setn	$HTVAL 1.
        .Askn [1:3:$HTVAL] $HTVAL MCEBLD -- Which option for HT:? [D:'$HTVAL']: 
;
.230$:
	.Setf	RTMESS
	.Setf	RTEXIT
	.Setf	HTMESS
	.Setf	HTEXIT
	.If $RTVAL = 1	.Sett	RTMESS
	.If $RTVAL = 2	.Sett	RTEXIT
	.If $HTVAL = 1	.Sett	HTMESS
	.If $HTVAL = 2	.Sett	HTEXIT
	.Ift SAV ;                 -- RT Messages                      : 'RTMESS'
	.Ift SAV ;                 -- RT Exit                          : 'RTEXIT'
	.Ift SAV ;                 -- HT Messages                      : 'HTMESS'
	.Ift SAV ;                 -- HT Exit                          : 'HTEXIT'
;				*****
	.IfNdf $VT2	.Goto	240$
	.Ift SAV ;                 -- VT2plus support                  : '$VT2'
	.Ift SAV	.Goto	250$
.240$:
;
; VT2plus support enables the use of VT2xx, VT3xx, VT4xx and VT5xx terminals.
; The keys <F11>, <F12> and <F13> work as <ESC>, <BS> and <LF> respectively.
; The keys <HELP>, <DO> etc. can also be used and it is possible to define the
; function keys <F6>..<F20> like this:
;
;     "F6 := TIM"
;
; Pressing <F6> then gives the time of day.
;
	.IfNdf $VT2	.Setf	$VT2
        .Iff $VT2	.Sets	X "N"
        .Ift $VT2	.Sets	X "Y"
        .Ask [$VT2] $VT2 MCEBLD -- Want the VT2plus support? [D:'X']: 
;
.250$:
;				*****
	.Iff $VT2	.Goto	270$
	.IfNdf $VT4	.Goto	260$
	.Ift SAV ;                 -- VT4plus support                  : '$VT4'
	.Ift SAV	.Goto	270$
.260$:
;
; On VT4xx and VT5xx terminals it is possible to use <F1>..<F5> as normal
; function keys. VT4plus support enables you to define those keys too if
; you answer "Y" to the following question.
;
	.IfNdf $VT4	.Setf	$VT4
        .Iff $VT4	.Sets	X "N"
        .Ift $VT4	.Sets	X "Y"
	.Ask [$VT4] $VT4 MCEBLD -- Want the VT4plus support? [D:'X']: 
;
.270$:
;				*****
	.IfNdf $TD2	.Goto	280$
	.Ift SAV ;                 -- Tandberg TDV2230 support         : '$TD2'
	.Ift SAV	.Goto	290$
.280$:
;
; The keys of a Tandberg TDV2230 terminal can also be defined.
;
	.IfNdf $TD2	.Setf	$TD2
        .Iff $TD2	.Sets	X "N"
        .Ift $TD2	.Sets	X "Y"
	.Ask [$TD2] $TD2 MCEBLD -- Want the Tandberg TDV2230 function key support? [D:'X']: 
;
.290$:
;				*****
	.IfNdf $EDT	.Goto	300$
	.Ift SAV ;                 -- EDT-Keypad support               : '$EDT'
	.Ift SAV	.Goto	310$
.300$:
;
; Some edit functions can be performed on the VTxxx keypad, similar to EDT,
; KED and K52 edit functions. The Keypad-editing can be switched on/off with
; the "MCE KEYPad on/off" command.
;
	.IfNdf $EDT	.SetF	$EDT
        .Iff $EDT	.Sets	X "N"
        .Ift $EDT	.Sets	X "Y"
	.Ask [$EDT] $EDT MCEBLD -- Want the EDT-Keypad editing support? [D:'X']: 
.310$:
	.IfNdf $EDTDF	.SetF	$EDTDF
	.Iff $EDTDF	.Sets	X "N"
	.Ift $EDTDF	.Sets	X "Y"
	.Iff SAV .Ask [$EDTDF] $EDTDF MCEBLD -- Want EDT-Keypad editing ON by default? [D:'X']: 
	.Ift SAV .Ift $EDT ;                 -- EDT-Keypad ON by default         : '$EDTDF'
	.Iff SAV ;
;				*****
	.IfNdf $EXTPR	.Goto	320$
	.Ift SAV ;                 -- Extended Prompt support          : '$EXTPR'
	.Ift SAV	.Goto	330$
.320$:
;
; The MCE prompt can show if insert or overwrite mode is active with "+"
; and "-" respectively. It can also display if EDT-Keypad editing (when
; the corresponding option is selected) is enabled with a ":" when the
; status line is off,
;
;     e.g.    "+:>"
;
; The Extended Prompt can be switched on/off with the "MCE PROMpt on/off"
; command.
;
	.IfNdf $EXTPR	.SetF	$EXTPR
        .Iff $EXTPR	.Sets	X "N"
        .Ift $EXTPR	.Sets	X "Y"
	.Ask [$EXTPR] $EXTPR MCEBLD -- Want the Extended Prompt? [D:'X']: 

.330$:
	.IfNdf $PRMDF	.SetF	$PRMDF
	.Iff $PRMDF	.Sets	X "N"
	.Ift $PRMDF	.Sets	X "Y"
	.Iff SAV .Ask [$PRMDF] $PRMDF MCEBLD -- Want Extended Prompt ON by default? [D:'X']: 
	.Ift SAV .Ift $EXTPR ;                 -- Extended Prompt ON by default    : '$PRMDF'
	.Iff SAV ;
;				*****
	.IfNdf $UPR	.Goto	340$
	.Ift SAV ;                 -- User Prompt support              : '$UPR'
	.Ift SAV	.Goto	350$
.340$:
;
; MCE can display a User Prompt. This prompt can be specified with the
; "MCE USPRompt <UserPrompt>" command. The maximum prompt size is 20
; characters.
;
	.IfNdf $UPR	.SetF	$UPR
        .Iff $UPR	.Sets	X "N"
        .Ift $UPR	.Sets	X "Y"
	.Ask [$UPR] $UPR MCEBLD -- Want the User Prompt? [D:'X']: 
;
.350$:
;				*****
	.IfNdf $PWD	.Goto	360$
	.Ift SAV ;                 -- Password Locking                 : '$PWD'
	.Ift SAV	.Goto	370$
.360$:
;
; MCE can lock a terminal with a password, almost like the LOCK command
; from Digital''s terminal servers. The maximum password-length is 8
; characters.
;
	.IfNdf $PWD	.SetF	$PWD
        .Iff $PWD	.Sets	X "N"
        .Ift $PWD	.Sets	X "Y"
	.Ask [$PWD] $PWD MCEBLD -- Want the terminal Password Locking? [D:'X']: 
;
.370$:
;				*****
	.IfNdf $INSDF	.Goto	380$
	.IfNdf $OVSDF	.Goto	380$
	.Ift SAV ;                 -- Insert mode default              : '$INSDF'
	.Ift SAV ;                 -- Overwrite mode default           : '$OVSDF'
	.Ift SAV	.Goto	390$
.380$:
;
; By default, Overwrite or Insert mode stays active until it is changed
.Iff $VT2 ; with <CTRL/A>. MCE can be built so that it sets one of the modes
.Ift $VT2 ; with <CTRL/A> or <F14>. MCE can be built so that it sets one of the modes
; active by default when it prompts for a new command. This can be
; changed with the "MCE INSErt on/off" and the "MCE OVERwrite on/off"
; commands.
;
	.IfNdf $INSDF	.SetF	$INSDF
        .Iff $INSDF	.Sets	X "N"
        .Ift $INSDF	.Sets	X "Y"
	.Ask [$INSDF] $INSDF MCEBLD -- Want MCE to set Insert mode back by default? [D:'X']: 
	.IfNdf $OVSDF	.SetF	$OVSDF
        .Iff $OVSDF	.Sets	X "N"
        .Ift $OVSDF	.Sets	X "Y"
	.Iff $INSDF .Ask [$OVSDF] $OVSDF MCEBLD -- Want MCE to set Overwrite mode back by default? [D:'X']: 
;
.390$:
;				*****
	.IfNdf $OLDDF	.Goto	400$
	.Ift SAV ;                 -- Save OLD Commands                : '$OLDDF'
	.Ift SAV	.Goto	410$
.400$:
;
; Normally, "old" commands (retrieved from the FIFO, but not edited) are
; saved again in the FIFO. MCE can be built so that it does not save these
; commands again. This mode can be changed with the "MCE SVOLd on/off"
; command.
;
	.IfNdf $OLDDF	.SetF	$OLDDF
        .Iff $OLDDF	.Sets	X "N"
        .Ift $OLDDF	.Sets	X "Y"
	.Ask [$OLDDF] $OLDDF MCEBLD -- Want MCE to save "old" commands by default? [D:'X']: 
;
.410$:
;				*****
	.IfNdf $INTDF	.Goto	420$
	.Ift SAV ;                 -- Save Internal Commands           : '$INTDF'
	.Ift SAV	.Goto	430$
.420$:
;
; Normally, Internal commands are saved in the FIFO. MCE can be built
; so that it does not save them. This mode can be changed with the
; "MCE SVINtern on/off" command.
;
	.IfNdf $INTDF	.SetF	$INTDF
        .Iff $INTDF	.Sets	X "N"
        .Ift $INTDF	.Sets	X "Y"
	.Ask [$INTDF] $INTDF MCEBLD -- Want MCE to save Internal commands by default? [D:'X']: 
;
.430$:
;				*****
	.IfNdf $TMO	.Goto	440$
	.Ift SAV ;                 -- Time-Out Support                 : '$TMO'
	.Ift SAV	.Goto	450$
.440$:
;
; When terminal Time-Out support is included, the terminal is logged out
; after a predetermined time (the exception is when MCE is run from TT0:,
; where it simply exits). You can either specify a fixed Time-Out value or
; allow the user to change it.
;
	.IfNdf $TMO	.SetF	$TMO
        .Iff $TMO	.Sets	X "N"
        .Ift $TMO	.Sets	X "Y"
	.Ask [$TMO] $TMO MCEBLD -- Want the terminal Time-Out support? [D:'X']: 
.;
.450$:
	.IfNdf $TMS	.SetF	$TMS
	.Ift SAV .Ift $TMO ;                 -- Time-Out Support change          : '$TMS'
        .Iff $TMS	.Sets	X "N"
        .Ift $TMS	.Sets	X "Y"
	.Iff SAV .Ift $TMO .Ask [$TMS] $TMS MCEBLD -- Want to allow the user to change the timeout value? [D:'X']: 
	.IfNdf $TMON	.SetN	$TMON 4
	.Ift SAV .Ift $TMO ;                 -- Time-Out value                   : '$TMON'
	.Iff $TMS 	.SetS 	T "Enter the Time-Out value in minutes"
	.Ift $TMS 	.SetS 	T "Enter the default Time-Out value in minutes"
	.Iff SAV .Ift $TMO .Askn [2:999:$TMON] $TMON MCEBLD -- 'T' [2..999 D:'$TMON']: 
	.IfNdf $TMODF 	.SetF	$TMODF
	.Ift SAV .Ift $TMO ;                 -- Time-Out default ON              : '$TMODF'
        .Iff $TMODF	.Sets	X "N"
        .Ift $TMODF	.Sets	X "Y"
	.Iff SAV .Ift $TMO .Ift $TMS .Ask [$TMODF] $TMODF MCEBLD -- Want the Time-Out to be ON by default? [D:'X']: 
;
.460$:
;				*****
	.IfNdf $MINCH	.Goto	470$
	.Ift SAV ;                 -- Minimum Command line length      : '$MINCH%D'
	.Ift SAV	.Goto	480$
.470$:
;
; Only command lines with a length greater than or equal to a given length
; are saved in the FIFO. The value can be changed with the "MCE CMSZ n"
; command. Use a value of 79 if you don''t want to save any commands at all.
;
	.IfNdf $MINCH	.SetN	$MINCH 3
        .Askn [1:79:$MINCH] $MINCH MCEBLD -- Enter minimum command line length [1..79 D:'$MINCH%D']: 
;
.480$:
;				*****
	.IfNdf $MAXFI	.Goto	490$
	.Ift SAV ;                 -- Maximum FIFO length              : '$MAXFI%D'
	.Ift SAV	.Goto	500$
.490$:
;
; Specify the default maximum number of commands which can be saved into
; the FIFO. The value can be changed with the "MCE FISZ n" command. Note
; that the actual number of commands that can be saved depends on the length
; of the commands and the available MCE pool space. Information about MCE''s
; pool space can be obtained with the "MCE FREE" command.
;
; Additional pool space can be created by installing MCE with a larger 
; increment, like this: INS $MCE/INC=xxx
; A value of 2400 will be enough in most cases.
;
	.IfNdf $MAXFI	.Iff $STA	.SetN	$MAXFI 23.
	.IfNdf $MAXFI	.Ift $STA	.SetN	$MAXFI 22.
	.Askn [1:99:'$MAXFI%D'] $MAXFI MCEBLD -- Enter maximum number of entries in FIFO [1..99 D:'$MAXFI%D']: 
;
.500$:
        .Iff PLUS	.Goto	540$
;				*****
	.IfNdf $MU	.Goto	510$
	.Ift SAV ;                 -- Multi User task                  : '$MU'
	.Ift SAV	.Goto	520$
.510$:
;
	.IfNdf $MU 	.SetF	$MU
        .Ask [$MU] $MU  MCEBLD -- Build MCE as a Multi-User task? [D:'$MU']: 
.520$:
	.IfNdf $ID 	.Goto	530$
	.Ift SAV ;                 -- I&D Space task                   : '$ID'
	.Ift SAV	.Goto	540$
.530$:
;
	.IfNdf $ID	.SetF	$ID
        .Ask [$ID] $ID  MCEBLD -- Build MCE as an I&D Space task? [D:'$ID']: 
.540$:
	.IfNdf $MU 	.SetF	$MU
	.IfNdf $ID	.SetF	$ID
        .If P1 <> "ALL"	.Goto	550$
        .Sett	$HLP
        .Sett	$COP
        .Goto	610$
.550$:
;
;				*****
;
; A custom MCE.HLP help file will be automatically created if you answer "Y"
; to the following question.
;
	.Iff $VT2 .Goto 560$
; Note that if you don''t create the help file now, you will no be able to use 
; the <NEXT-SCREEN> and <PREV-SCREEN> keys to walk through the help file.
.560$:
;
	.IfNdf $HLP 	.SetF	$HLP
        .Iff $HLP	.Sets	X "N"
        .Ift $HLP	.Sets	X "Y"
        .Ask [$HLP] $HLP MCEBLD -- Create Help file on LB:? [D:'X']: 
        .Iff $HLP	.Goto	600$
	.IfNdf $HLPDR	.Goto	570$
	.Ift SAV ;                 -- Help directory                   : '$HLPDR'
	.Ift SAV	.Goto	580$
.570$:
	.IfNdf $HLPDR	.Sets	$HLPDR "[HELP]"
	.Asks [::$HLPDR] $HLPDR MCEBLD -- What is your Help Directory? [D:'$HLPDR']: 
;
.580$:
;				*****
;
	.IfNdf $COP	.SetF	$COP
        .Iff $COP	.Sets	X "N"
        .Ift $COP	.Sets	X "Y"
        .Ask [$COP] $COP MCEBLD -- When done, copy the output MCE.TSK file to LB: device? [D:'X']: 
	.IfNdf $SYSDR	.Sets	$SYSDR <LibDir>
	.Ift $COP .Asks [::$SYSDR] $SYSDR MCEBLD -- What is your Task Directory? [D:'$SYSDR']: 
	.IfNdf $INS	.SetF	$INS
        .Iff $INS	.Sets	X "N"
        .Ift $INS	.Sets	X "Y"
        .Ift $COP .Ask [$INS] $INS MCEBLD -- Install task afterwards? [D:'X']: 
	.IfNdf $PRI	.SetF	$PRI
        .Iff $PRI	.Sets	X "N"
        .Ift $PRI	.Sets	X "Y"
        .Ask [$PRI] $PRI MCEBLD -- Print listing and map? [D:'X']: 
.610$:
;
	.;
	.IfNdf $SRC	.SetS $SRC "MCE.MAC"
	.Iff SAV .Goto 615$
	.TestFile '$SRC'
	.If <FilErr> Ne 0	.Goto	612$
        .Ask Q MCEBLD -- Use the source file '$SRC'? [D:N]: 
	.Ift Q	.Goto	615$
	.Goto	613$
.612$:
;MCEBLD -- Error: file '$SRC' not found!
;
.613$:
	.AskS [::$SRC] $SRC MCEBLD -- Enter MCE source file specification [D:'$SRC']: 
	.Goto	610$
.615$:

;MCEBLD -- No more questions.
;
	.IFF SAVOUT .GOTO 620$
;MCEBLD -- Creating saved answer file 'SAVFIL' 
        .Open 'SAVFIL'
	.Data .;
        .Data .;  MCE - Saved answer file 'SAVFIL'
        .Data .;
        .Data .; Created on '<Date>' '<Time>' by MCEBLD.CMD Version: 'VERS'
	.Data .;
	.Data .; MCE Ident code 'IDNPRE'
	.Data .;
	.Data	.Enable Global
	.Data	.Set'$CLS'	$CLS	! Multiple CLI support
	.Data	.Set'$INI'	$INI	! Init-File support
	.Data	.Set'$SYL'	$SYL	! SYS$LOGIN support
	.Data	.Set'$CMP'	$CMP	! Compound Command line support
	.Data	.Set'$STA'	$STA	! Status line command line support
	.Data	.Set'$STADF'	$STADF	! Status line ON Default
	.Data	.Set'$SIL'	$SIL	! Startup & Exit messages
	.Data	.SetN	$RTVAL	'$RTVAL'	! RT Handling value
	.Data	.SetN	$HTVAL	'$HTVAL'	! HT Handling value
	.Data	.Set'$VT2'	$VT2	! VT2Plus support
	.Data	.Set'$VT4'	$VT4	! VT4Plus support
	.Data	.Set'$TD2'	$TD2	! Tandberg TV2230 support
	.Data	.Set'$EDT'	$EDT	! Keypad editing support
	.Data	.Set'$EDTDF'	$EDTDF	! Keypad editing support default ON
	.Data	.Set'$EXTPR'	$EXTPR	! Extended Prompt support
	.Data	.Set'$PRMDF'	$PRMDF	! Extended Prompt support default ON
	.Data	.Set'$UPR'	$UPR	! User Prompt  support
	.Data	.Set'$PWD'	$PWD	! Password locking support
	.Data	.Set'$INSDF'	$INSDF	! Insert mode default
	.Data	.Set'$OVSDF'	$OVSDF	! Overwrite mode default
	.Data	.Set'$OLDDF'	$OLDDF	! Save OLD Commands default
	.Data	.Set'$INTDF'	$INTDF	! Save Internal Commands default
	.Data	.Set'$TMO'	$TMO	! Time-Out support
	.Data	.Set'$TMS'	$TMS	! Time-Out change support
	.Data	.SetN	$TMON	'$TMON%D'	! Time-Out time
	.Data	.Set'$TMODF'	$TMODF	! Time-Out default ON
	.Data	.SetN	$MINCH	'$MINCH%D'	! Minimum command length to save
	.Data	.SetN	$MAXFI	'$MAXFI%D'	! Max. FIFO length
	.Data	.Set'$MU'	$MU	! Multi-User
	.Data	.Set'$ID'	$ID	! I&D Space
	.Data	.Set'$HLP'	$HLP	! Create help file
	.Data	.SetS	$HLPDR	"'$HLPDR'"	! Help file directory
	.Data	.Set'$COP'	$COP	! Copy task image
	.Data	.SetS	$SYSDR	"'$SYSDR'"	! System Directory
	.Data	.SetS	$SRC	"'$SRC'"	! Source file
	.Data	.Set'$INS'	$INS	! Install Task
	.Data	.Set'$PRI'	$PRI	! Print Listings
	.Close
        .Ift MCR 	PIP 'SAVFIL'/PU/NM
        .Iff MCR 	PURGE 'SAVFIL'
.620$:
        .Iff $HLP .Goto 650$
;MCEBLD -- Creating help file LB:'$HLPDR'MCE.HLP
.;
        .Open LB:'$HLPDR'MCE.HLP
			.Data MCE 'VERS' - Command Line Editor options:
			.Data - Support is included for the following options:
.Ift $SIL    		.Data   * Startup and exit messages
.Ift $VT2 .Iff $VT4    	.Data   * VT2plus support
.Ift $VT4	    	.Data   * VT2plus and VT4plus support
.Ift $TD2		.Data   * TDV2230 Function keys
.Ift $INI .Iff $SYL	.Data   * MCE Init-Files
.Ift $INI .Ift $SYL	.Data   * MCE Init-Files read from LB:'$INIDR' and SYS$LOGIN:
.Ift $CLS		.Data   * Multiple CLI
.Ift $EDT .Iff $EDTDF	.Data   * EDT-Keypad editing - default OFF
.Ift $EDT .Ift $EDTDF	.Data   * EDT-Keypad editing - default ON
.Ift $STA .Ift $STADF 	.Data   * Status line - default ON
.Ift $STA .Iff $STADF 	.Data   * Status line - default OFF
.Ift $OLDDF 		.Data   * Save "Old" Commands - default ON
.Iff $OLDDF 		.Data   * Save "Old" Commands - default OFF
.Ift $INTDF 		.Data   * Save Internal Commands - default ON
.Iff $INTDF 		.Data   * Save Internal Commands - default OFF
.Ift $INSDF 		.Data   * Auto Insert mode - default ON
.Ift $OVSDF 		.Data   * Auto Overwrite mode - default ON
.Ift $CMP  		.Data   * Compound Command lines
.Ift $TMO .Iff $TMS	.Data   * Terminal Time-Out ('$TMON%D' minutes) fixed
.Ift $TMO .Ift $TMS .Iff $TMODF	.Data   * Terminal Time-Out ('$TMON%D' minutes) - default OFF
.Ift $TMO .Ift $TMS .Ift $TMODF	.Data   * Terminal Time-Out ('$TMON%D' minutes) - default ON
.Ift $EXTPR .Iff $PRMDF	.Data   * Extended Prompt - default OFF
.Ift $EXTPR .Ift $PRMDF	.Data   * Extended Prompt - default ON
.Iff $UPR .Ift $PWD	.Data   * Terminal Password Locking
.Ift $UPR .Iff $PWD	.Data   * User Prompt
.Ift $UPR .Ift $PWD	.Data   * User Prompt and Terminal Password Locking
.Ift RTMESS .Ift HTMESS .Data   * RT: and HT: detection (Message)
.Ift RTMESS .Iff HTMESS	.Data   * RT: detection (Message)
.Iff RTMESS .Ift HTMESS	.Data   * HT: detection (Message)
.Ift RTEXIT .Ift HTEXIT .Data   * RT: and HT: detection (Exit)
.Ift RTEXIT .Iff HTEXIT .Data   * RT: detection (Exit)
.Iff RTEXIT .Ift HTEXIT	.Data   * HT: detection (Exit)
.If $MINCH = 1		.Data - By default all commands are saved. (CMSZ = 1)
.If $MINCH > 1		.Data - By default only commands which contain at least '$MINCH%D' characters are saved.
			.Data   To change this use the "MCE CMSZ n" command (see INTERNAL)
			.Data - By default the savebuffer can contain up to '$MAXFI%D' command lines.
			.Data   To change this value use the "MCE FISZ n" command (see INTERNAL)
.Iff $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL RECALL
.Ift $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL RECALL KEYPAD
.; --KEYS1 ------------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|KEYS*1
.Ift $EDT 		.Data MCE 'VERS' - Command Line Editor Non-keypad editing and cursor movement commands
.Iff $EDT 		.Data MCE 'VERS' - Command Line Editor Editing and cursor movement commands:
			.Data
			.Data  <LEFT>      or <CTRL/D> - Move Cursor Left
			.Data  <RIGHT>     or <CTRL/F> - Move Cursor Right
			.Data  <BACKSPACE> or <CTRL/H> - Move Cursor to Begin of Line
			.Data  <CTRL/E>                - Move Cursor to End of Line
			.Data  <TAB>       or <CTRL/I> - Move one Word to the right or move from EOL to BOL
			.Data  <DELETE>                - Delete Character Left of Cursor
			.Data  <CTRL/V>                - Delete Character at Cursor
			.Data  <LINEFEED>  or <CTRL/J> - Delete Word Left of Cursor
			.Data  <CTRL/W>                - Delete Word Right at Cursor
			.Data  <CTRL/U>                - Delete from Cursor to Begin of Line
			.Data  <CTRL/K>                - Delete from Cursor to End of Line
			.Data  <CTRL/C>                - Delete whole line
			.Data  <CTRL/R>                - Rewrite Line
.Iff $VT2		.Data  <CTRL/A>                - Switch between Overwrite and Insert mode
.Ift $VT2		.Data  <CTRL/A>    or  <F14>   - Switch between Overwrite and Insert mode
			.Data
.Ift $EXTPR	 	.Data  If the Extended Prompt is on the current mode is shown by the first prompt
.Ift $EXTPR 		.Data  character: "+" means Insert mode, "-" Overwrite mode.
			.Data
.Iff $EDT 		.Data More help: KEYS2 COMMANDLINES TRANS FUNC START EXIT INTERNAL RECALL
.Ift $EDT 		.Data More help: KEYS2 COMMANDLINES TRANS FUNC START EXIT INTERNAL RECALL KEYPAD
.; --KEYS2 ------------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|KEYS2
.Ift $EDT 		.Data MCE 'VERS' - Command Line Editor non-keypad keys:
.Iff $EDT 		.Data MCE 'VERS' - Command Line Editor keys:
			.Data
			.Data Save and execute:
			.Data  <RETURN>    or <ENTER>    - Execute command
.Ift $VT2 		.Data  <ESC><ESC>  or <DO>       - Execute command without waiting
.Iff $VT2 		.Data  <ESC><ESC>                - Execute command without waiting
			.Data  <CTRL/X>                  - Execute cmd and leave FIFO-pointer where it is
			.Data  <CTRL/N>                  - Save command without executing
			.Data  <CTRL/T>                  - Enable output from other tasks: detach 10 sec.
			.Data  <CTRL/P>                  - Enable output from other tasks: suspend MCE
.Ift $EDT		.Data Non-keypad command buffer manipulation keys:
.Iff $EDT		.Data Command buffer manipulation keys:
			.Data  <PF4>       or <ESC>S     - Display contents of FIFO
			.Data  <UP>        or <CTRL/B>   - Retrieve previous command
			.Data  <DOWN>                    - Reverse of <UP> (next command)
.Iff $VT2 		.Data  <PF2>       or <ESC>Q     - Recall command previously requested (see RECALL)
.Ift $VT2 		.Data  <PF2> or <FIND> or <ESC>Q - Recall command previously requested (see RECALL)
.Ift $EDT		.Data Non-keypad Help keys:
.Iff $EDT		.Data Help keys:
.Ift $VT2 		.Data  <HELP> or <CTRL/?>        - Gives HELP about MCE
.Iff $VT2 		.Data  <CTRL/?>                  - Gives HELP about MCE
.Ift $VT2 		.Data  <PRV-SCREEN>/<NXT-SCREEN> - Gives Previous/Next HELP screen
.Ift $EDT		.Data Non-keypad command translation keys:
.Iff $EDT		.Data Command translation keys:
			.Data  <PF1>       or <ESC>P     - Translate command without execution
			.Data  <PF3>       or <ESC>R     - Show command translation Buffer
.Iff $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL RECALL
.Ift $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL RECALL KEYPAD
.; -- COMMANDLINES  ------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|COM*MANDLINES
			.Data MCE 'VERS' - Command Line Editor command lines:
			.Data  The maximum length of command lines is 78 characters.
			.Data  If the command line starts with "MCE", an internal command is assumed. Other
.Iff $CLS		.Data  command lines are checked for translation and the result is spawned to MCR.
.Ift $CLS		.Data  command lines are checked for translation and the result is spawned to the
.Ift $CLS		.Data  current Command Line Interpreter for the invoking terminal.
.Iff $CLS		.Data  Special action is performed with "BYE" (see INTERNAL.)
.Ift $CLS		.Data  Special action is performed with "BYE" and "LOG[out]" (see INTERNAL.)
.Ift $CMP		.Data  A command line can consist of a single command or can contain more commands
.Ift $CMP		.Data  separated by " &". (Note the space to let PIP /TD&/LI still work.)
.Ift $CMP		.Data  A command after a "&" will is processed only if the previous one succeeded.
.Ift $CMP		.Data  A compound command line may also contain internal commands mixed with
.Ift $CMP		.Data  normal CLI commands. Only the first command is checked for translation.
			.Data
.If $MINCH = 1		.Data  Normally all command lines are saved in the FIFO-buffer. This may be changed
.If $MINCH = 1		.Data  with the "MCE CMSZ n" command (see INTERNAL.)
.If $MINCH > 1		.Data  Normally all command lines which contain '$MINCH' or more characters are saved
.If $MINCH > 1		.Data  in the FIFO-buffer. This may be changed with "MCE CMSZ n" (see INTERNAL.)
			.Data  When '$MAXFI' entries are present, or when no buffer space is available to hold
			.Data  more entries, the new command is saved and the oldest disappears.
			.Data  Retrieved command lines from the FIFO are only saved again if they are
			.Data  edited or when the saving is enabled by "MCE SVOL ON" (Save "OLD" commands)
			.Data  and this command line is not the newest in the FIFO. This prevents filling 
			.Data  up your FIFO with one command by continuously repeating the last command.
			.Data  Internal commands are saved when this is enabled by "MCE SVIN ON"
			.Data  (Save "INTERNAL" commands.)
.Iff $EDT 		.Data More help: KEYS TRANS FUNC START EXIT INTERNAL RECALL
.Ift $EDT 		.Data More help: KEYS TRANS FUNC START EXIT INTERNAL RECALL KEYPAD
.; -- TRANS1  ------------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|TRANS*1
			.Data MCE 'VERS' - Command Line Editor command translations:
			.Data
			.Data  A command may be defined with:
.Iff $CMP		.Data         +>CNAM := <TRANSLATION_OF_COMMAND>
.Ift $CMP  		.Data         +>CNAM := <TRANSL_OF_COMMAND> [& <TRANSL_OF_COMMAND> [& .....]]
			.Data  where CNAM is the command name which will be substituted by the text at the
			.Data  right side of the ":=".
.Ift $CMP		.Data  A command behind " &" will only be processed when the former was successful.
			.Data  NOTE: The ":=" must be preceded and followed by at least a single space or TAB
			.Data  e.g.:  +>HE := HELP PIP        ! 
			.Data         +>HE                    ! Will request "HELP PIP"
.Iff $EDT		.Data  The <PF1> or <ESC>P key translates the command without execution.
.Ift $EDT		.Data  With Keypad disabled <PF1> or <ESC>P translates the command without execution.
			.Data
			.Data  If you enter a predefined command followed by additional text, this additional
			.Data  text will normally be appended as a whole.
			.Data  e.g.:  +>HE /RE                ! Will request "HELP PIP /RE
			.Data  or:    +>HE /RE<PF1>           ! Will be translated and displayed
			.Data  result +>HELP PIP /RE
			.Data
			.Data  You can delete a command translation entry by redefining it to null.
			.Data  e.g.:  +>HE :=                 ! Delete above definition
			.Data
.Iff $EDT 		.Data More help: KEYS COMMANDLINES TRANS2 FUNC START EXIT INTERNAL RECALL
.Ift $EDT 		.Data More help: KEYS COMMANDLINES TRANS2 FUNC START EXIT INTERNAL RECALL KEYPAD
.; -- TRANS2  ------------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|TRANS2
			.Data MCE 'VERS' - Command Line Editor command translations (continued):
			.Data
			.Data  Abbreviate commands by separating the optional part with a "*"
			.Data  e.g.:  +>HP*IP := HELP PIP
			.Data  This command may now be requested with:
			.Data         +>HP or HPI or HPIP  etc.
			.Data
			.Data  More sophisticated is the Parameter-Substitution, similar to the
			.Data  indirect command file processor.
			.Data  e.g.:  +>DIR*ECTORY := PIP ''P1''/LI
			.Data         +>COP*Y := PIP ''P2''/NV/CD=''P1''
			.Data         +>DIR                   ! Will be translated into "PIP /LI"
			.Data         +>DIREC FIL             ! Will be translated into "PIP FIL/LI"
			.Data         +>COPY A.CMD [200,200]  ! Will simulate DCL COPY command
.Ift $INI 		.Data  Note that command definitions may be performed from the command file
.Ift $INI .Iff $CLS .Iff $SYL .Data         LB:'$INIDR'MCEINI.CMD  and  SY:[CurDir]MCEINI.CMD
.Ift $INI .Ift $CLS .Iff $SYL .Data         LB:'$INIDR'MCEINI.xxx  and  SY:[CurDir]MCEINI.xxx
.Ift $INI .Iff $CLS .Ift $SYL .Data         LB:'$INIDR'MCEINI.CMD  and  SYS$LOGIN:MCEINI.CMD
.Ift $INI .Ift $CLS .Ift $SYL .Data         LB:'$INIDR'MCEINI.xxx  and  SYS$LOGIN:MCEINI.xxx
.Ift $INI 		.Data  which will always be read when MCE is started.
.Ift $INI .Iff $SYL	.Data         [CurDir] = the default directory at the startup of MCE.
.Ift $INI .Ift $CLS 	.Data         xxx      = CLI name
.Ift $INI 		.Data  Files are read in the order they are mentioned above.
			.Data
.Iff $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL RECALL
.Ift $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL RECALL KEYPAD
.; -- FUNC  ------------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|FUNC*TIONS
			.Data MCE 'VERS' - Command Line Editor function keys:
			.Data
			.Data  You may request a command with function keys <PF1>, <PF2>, <PF3> or <PF4>
			.Data  It is also possible to request commands with control keys (e.g. <CTRL/G>).
.Ift $VT2 		.Data  On a VT2xx/VT3xx/VT4xx/VT5xx-Keyboard you can also define the function keys
.Ift $VT2 .Iff $VT4	.Data  <F6>..<F20> and <FIND> <INSERT HERE> etc.
.Ift $VT2 .Ift $VT4	.Data  <F6>..<F20> and <FIND> <INSERT HERE> etc.  On a VT4xx also <F1>..<F5>
.Ift $TD2 		.Data  On a TDV2230 you can also define the Function keys <F1>..<F7>
			.Data  Define the command translation as "''key'' := Command".
			.Data  e.g.   +>PF2 := TIM            ! Redefine <PF2> function key
			.Data         <PF2>                   !  to display the current time
.Iff $VT2 .Iff $TD2 .Goto 625$
		 	.Data         +>F6 := ACT             ! Define F6 function key
		 	.Data         <F6>                    !  to display the active tasks
.625$:
.Ift $VT2 		.Data  The keys <FIND> etc. must be entered as "F$a", where "a" is the first char.
.Ift $VT2 		.Data  of the key function, e.g. "F$P" for <PREV-SCREEN>    (English text)
			.Data  Define a control key as follows:
			.Data  e.g.   +>^G := SET /DEF        ! Define <CTRL/G> key
			.Data         <CTRL/G>                !  to display the current directory
			.Data  Note that the following control-keys CANNOT be defined:
			.Data  <CTRL/M> = <RETURN>,  <CTRL/O>,  <CTRL/Q>,  <CTRL/S>,  <CTRL/[> = <ESC>
			.Data  Next to the rest of the Alphabetic characters the following keys can
			.Data  be defined: <CTRL/\>,  <CTRL/]>,  <CTRL/^>,  <CTRL/?>
.Iff $EDT 		.Data More help: KEYS COMMANDLINES TRANS START EXIT INTERNAL RECALL
.Ift $EDT 		.Data More help: KEYS COMMANDLINES TRANS START EXIT INTERNAL RECALL KEYPAD
.; -- START ------------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|STA*RT
			.Data MCE 'VERS' - Command Line Editor startup
			.Data
			.Data  Invoke MCE with command "MCE" (best done in LOGIN.CMD with ".XQT MCE").
.Iff $INI .Goto 630$
.Ift $CLS		.Data  A File of command definitions, CLI commands and/or internal commands will be 
.Iff $CLS		.Data  A File of command definitions, MCR commands and/or internal commands will be
			.Data  read at MCE startup time. The file may be specified in the form 
			.Data  "MCE <startup_file>".
			.Data  If no startup file is specified in the command line (the normal case),
			.Data  files are read in this order:
.Ift $CLS 		.Data     LB:'$INIDR'MCEINI.xxx,        followed by
.Ift $CLS .Iff $SYL	.Data     SY:[CurDir]MCEINI.xxx,
.Ift $CLS .Ift $SYL	.Data     SYS$LOGIN:MCEINI.xxx,
.Ift $CLS 		.Data  Where "xxx" is the name of the CLI defined for the terminal invoking MCE
.Ift $CLS .Iff $SYL	.Data  and [CurDir] is the default directory at the startup of MCE.
.Iff $CLS 		.Data     LB:'$INIDR'MCEINI.CMD,        followed by
.Iff $CLS .Iff $SYL	.Data     SY:[CurDir]MCEINI.CMD,
.Iff $CLS .Ift $SYL	.Data     SYS$LOGIN:MCEINI.CMD,
.Iff $CLS .Iff $SYL	.Data  [CurDir] is the default directory at the startup of MCE.
			.Data  This algorithm divides command definitions into two groups:
			.Data  A system-wide set in LB:'$INIDR', followed by a user''s private set.
			.Data  Overriding the default filename in the command line when invoking MCE offers
			.Data  the possibility to tailor an application specific command set.
.630$:
			.Data
.Iff $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC EXIT INTERNAL RECALL
.Ift $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC EXIT INTERNAL RECALL KEYPAD
.; -- EXIT ------------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|EX*IT
			.Data MCE 'VERS' - Command Line Editor exit
			.Data
			.Data   To exit MCE operations, use the "MCE EXIT" internal command.
			.Data   or <CTRL/Y>
			.Data   More about internal commands see help MCE INTERNAL.
			.Data
.Iff $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START INTERNAL RECALL
.Ift $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START INTERNAL RECALL KEYPAD
.; -- INTERNAL -----------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|INT*ERNAL
			.Data MCE 'VERS' - Command Line Editor internal commands (MCE xxxx)
		        .Data    CLEA/PURG: Delete all commands in command FIFO/Translation buffer
		        .Data    CMSZ n   : Changes minimum command line length to be saved to "n"
		        .Data    ECHO     : Command lines are echoed on TI: (useful in Init-Files)
		        .Data    EXIT     : MCE task exit (return to the normal command environment.)
		        .Data    FISZ n   : Changes FIFO-size to "n" (if enough memory)
		        .Data    FREE     : Display MCE Pool info (largest_block:Total_free_blocks:fragments)
			.Data    INSE xx  : Auto Insert mode on/off       (xx = on / off)
.Ift $EDT 		.Data    KEYP xx  : Set Keypad editing on/off     (xx = on / off)
.Ift $PWD	        .Data    LOCK     : Terminal password Locking
		        .Data    LIST/RING: FIFO is a list/ring: UP and DOWN stop/roll at the end
			.Data    OVER xx  : Auto Overwrite mode on/off    (xx = on / off)
.Ift $EXTPR		.Data    PROM xx  : Extended Prompt on/off        (xx = on / off)
.Ift $INI 		.Data    READ file: Read commands from specified file (same as CHAI file)
.Ift $INI 		.Data    REPL file: Combines PURGe and READ.
.Ift $STA 		.Data    STAT xx  : Status line on/off/show       (xx = on / off / "blank")
		        .Data    SVIN xx  : Save internal commands on/off (xx = on / off)
		        .Data    SVOL xx  : Save "old" commands on/off    (xx = on / off)
.Ift $TMO .Ift $TMS       .Data    TIMO xxx : Sets Time-Out Value (ON/OFF or 2-999 minutes)
.Ift $UPR		.Data    USPR xx  : Define User Prompt          |  INTERNAL2  gives you an 
.Ift $UPR	        .Data    VERS     : Show MCE Version            |  explanation of internal commands.
.Iff $UPR	        .Data    VERS     : Show MCE Version 
.Iff $UPR		.Data         INTERNAL2 gives you an explanation of internal commands.
.Iff $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL2 RECALL
.Ift $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL2 RECALL KEYPAD
.; -- INTERNAL2 -----------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|INTERNAL2|INT2
			.Data MCE 'VERS' - Command Line Editor internal commands (continued)
			.Data
.Iff $CLS		.Data   Two command verbs are specially handled by MCE: "BYE" and "MCE".
.Iff $CLS		.Data   "BYE" is sent to MCR and MCE exits immediately.
.Ift $CLS		.Data   Three command verbs are specially handled by MCE: "BYE", "LOG[out]" and "MCE".
.Ift $CLS		.Data   "BYE" and when in DCL "LOG[out]" result in a spawn of "BYE" to MCR and MCE
.Ift $CLS		.Data   exits immediately.
			.Data   Note that you may define any other string (e.g. GOODBYE) to result
.Iff $CLS		.Data   in BYE.
.Ift $CLS		.Data   in BYE or LOGOUT.
			.Data   Commands starting with "MCE" are treated as internal commands and not passed
.Iff $CLS		.Data   to MCR. Following the verb "MCE " a four-character action specifier
.Ift $CLS		.Data   to the CLI. Following the verb "MCE " a four-character action specifier
			.Data   defines the action wanted.
.Iff $INI .Goto 650$
			.Data   The READ and REPLACE options, if no file is specified, follow the
.Iff $CLS .Goto 640$
			.Data   the same filename convention as at startup: the files according
			.Data   to the current CLI are read in, i.e. the command "MCE REPLace"
			.Data   produces a restart of MCE (but the FIFO-buffer is kept), useful
			.Data   after a SET TERMINAL <new_cli> command.
.Ift $SYL		.Data   When a space is given as filename, only SYS$LOGIN:MCEINI.xxx is read.
.Iff $SYL		.Data   When a space is given as filename, only SY:[CurDir]MCEINI.xxx is read.
.Iff $SYL		.Data   [CurDir] is the default directory at the moment of the command.
	.Goto 650$
.640$:
			.Data   the same filename convention as at startup: the Init-Files
			.Data   are read in, i.e. the command "MCE REPLace" produces a restart of MCE
			.Data   (but the FIFO-buffer is kept).
.Ift $SYL		.Data   When a space is given as filename, only SYS$LOGIN:MCEINI.xxx is read.
.Iff $SYL		.Data   When a space is given as filename, only SY:[CurDir]MCEINI.xxx is read.
.Iff $SYL		.Data   [CurDir] is the default directory at the moment of the command.
.650$:
			.Data
.Iff $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL RECALL
.Ift $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL RECALL KEYPAD
.; -- RECALL -----------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|REC*ALL
			.Data MCE 'VERS' - Command Line Editor recall function
			.Data
			.Data  Recalling a command is done by entering the first part of a command
			.Data  which was previously executed, terminated by the function key <PF2>,
.Iff $VT2		.Data  by <UP> if UPFInd is enabled or by <ESC>Q.
.Ift $VT2		.Data  by <UP> if UPFInd is enabled, by <ESC>Q or by <FIND>.
			.Data  This will find the last command already executed starting with the
			.Data  string entered. The command may be edited or executed by pressing
			.Data  the return key. A second <PF2> keystroke searches further back in the
			.Data  FIFO. When no string is entered, the last defined string is taken.
			.Data  Note that the function key <PF2> may not have any superimposed
			.Data  translation defined (by defining "PF2 := ....."), for example:
			.Data     +>DIR X.DAT        ! Previously entered command
			.Data     +>DMP X.DAT        !     "          "      "
			.Data
			.Data     +>D<PF2>           ! Will recall the last command starting with "D":
			.Data     						+>DMP X.DAT
			.Data     +><PF2>            ! Will recall the for last command:
			.Data     						+>DIR X.DAT
			.Data     +>DI<PF2>          ! Will recall the command:
			.Data     						+>DIR X.DAT
			.Data
.Iff $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL
.Ift $EDT 		.Data More help: KEYS COMMANDLINES TRANS FUNC START EXIT INTERNAL KEYPAD
.Iff $EDT .Goto 660$
.; -- KEYPAD  -----------------------------------------------------------------------------------------------------------------
			.INC HLPNR
			.Sets MAXHLP HLPCHR[HLPNR:HLPNR]
			.Data \MCE'MAXHLP'|KEY*PAD
			.Data MCE 'VERS' - Command Line Editor EDT-Keypad and Function keys:
			.Data  +--------+-----------------+   +--------+--------+--------+--------+
			.Data  |        |                 |   |\\\\\\\\|        |        | DelLin |
			.Data  |  Help  |  Execute NoWait |   |\\Gold\\| KPHelp | Recall | ------ | <F11>
			.Data  |        |                 |   |\\\\\\\\|        |        |\UndLin |  = ESC
			.Data  +--------+--------+--------+   +--------+--------+--------+--------+
			.Data  | (Find) | Insert | Re-    |   |        | ShoCmd | ShoFif | DelWrd | <F12>
			.Data  | Recall |  Here  |  move  |   | Transl | ------ | ------ | ------ |  = BegLin
			.Data  |        |        |        |   |        |\\Vers\\|\\Free\\|\UndWrd\|
			.Data  +--------+--------+--------+   +--------+--------+--------+--------+ <F13>
			.Data  |        |  Prev  |  Next  |   | Detach |        |  Cut   | DelChr |  = DelWrd
			.Data  | Select |  Help  |  Help  |   | ------ |        | ------ | ------ |    Right
			.Data  |        |(Screen)|(Screen)|   |\Suspnd\|        |\Paste\\|\UndChr\|
			.Data  +--------+--------+--------+   +--------+--------+--------+--------+ <F14>
			.Data           |  Prev  |            |        |  EOL   |        |        |  = Ins/Ov
			.Data           | Command|            |  Word  | ------ | Do Stay| Enter  |
			.Data           |        |            |        |\DelLin\|\       |        |
			.Data  +--------+--------+--------+   +--------+--------+--------+ ------ |
			.Data  |        |  Next  |        |   |    BegLine      | Select |\\\\\\\\|
			.Data  |  <-    | Command|   ->   |   | --------------- | ------ |\\Save\\|
			.Data  |        |        |        |   |\\\\ClrLine\\\\\\|\Reset\\|\\\\\\\\|
			.Data  +--------+--------+--------+   +-----------------+--------+--------+
.660$:
	.Close
        .Ift MCR 	PIP LB:'$HLPDR'MCE.HLP/PU/NM
        .Iff MCR	PURGE LB:'$HLPDR'MCE.HLP
.670$:
;MCEBLD -- Creating assembly configuration file MCEPRE.INC
        .Open MCEPRE.INC
        .Data 	SUBTTL MCE - Configuration file MCEPRE.INC
        .Data ;
        .Data ; MCE Configuration file
        .Data ; Created on '<Date>' '<Time>' by MCEBLD.CMD Version: 'VERS'
	.Data ;
	.Data 	IFNDEF	FALSE
	.Data FALSE	equ	0
	.Data 	ENDIF
	.Data 	IFNDEF	TRUE
	.Data TRUE	equ	NOT FALSE
	.Data 	ENDIF
	.Data ;
		.Data IDNPRE	equ	'IDNPRE'	; Identcode of MCEPRE.INC
.Ift $CLS    	.Data CLISUP	equ	TRUE	; Multiple CLI support
.Iff $CLS    	.Data CLISUP	equ	FALSE	; No Multiple CLI support
.Ift $CMP    	.Data COMPND	equ	TRUE	; Compound Command (&) support
.Iff $CMP    	.Data COMPND	equ	FALSE	; No Compound Command (&) support
.Ift $EDT    	.Data EDT	equ	TRUE	; EDT-Keypad support included
.Iff $EDT    	.Data EDT	equ	FALSE	; EDT-Keypad support not included
.Ift $EDT .Iff $EDTDF	.Data EDTDEF	equ	0	; EDT-Keypad off by default
.Ift $EDTDF 	.Data EDTDEF	equ	1	; EDT-Keypad on by default
.Ift $UPR 	.Data UPR	equ	TRUE	; User Prompt
.Iff $UPR 	.Data UPR	equ	FALSE	; No User Prompt
.Ift $PWD 	.Data PWD	equ	TRUE	; Terminal password locking
.Iff $PWD 	.Data PWD	equ	FALSE	; No Terminal password locking
.Ift $EXTPR 	.Data EXTNPR	equ	TRUE	; Extended Prompt included
.Iff $EXTPR 	.Data EXTNPR	equ	FALSE	; Extended Prompt not included
.Ift $EXTPR .Iff $PRMDF .Data PRMDEF	equ	0	; Extended Prompt off by default
.Ift $EXTPR .Ift $PRMDF .Data PRMDEF	equ	1	; Extended Prompt on by default
.Ift $INI    	.Data FILE	equ	TRUE	; MCE Init-File support included
.Iff $INI    	.Data FILE	equ	FALSE	; MCE Init-File support not included
.Ift $SYL	.Data SYLOGIN	equ	TRUE	; Read MCEINI from SYS$LOGIN:
.Iff $SYL	.Data SYLOGIN	equ	FALSE	; Don''t read MCEINI from SYS$LOGIN:
.Iff $INSDF 	.Data INSDEF	equ	0	; Don''t return to insert mode by default
.Ift $INSDF 	.Data INSDEF	equ	1	; Return to insert mode by default
.Iff $INTDF 	.Data INTDEF	equ	0	; Don''t save internal commands by default
.Ift $INTDF 	.Data INTDEF	equ	1	; Save internal commands by default
.Iff $OLDDF 	.Data OLDDEF	equ	0	; Don''t save old commands by default
.Ift $OLDDF 	.Data OLDDEF	equ	1	; Save old commands by default
.Iff $OVSDF 	.Data OVSDEF	equ	0	; Don''t return to overwrite mode by default
.Ift $OVSDF 	.Data OVSDEF	equ	1	; Return to overwrite mode by default
.Ift RTMESS	.Data RTMESS	equ	TRUE	; RT: detection (Message)
.Iff RTMESS	.Data RTMESS	equ	FALSE	; No RT: detection (Message)
.Ift RTEXIT	.Data RTEXIT	equ	TRUE	; RT: detection (Exit)
.Iff RTEXIT	.Data RTEXIT	equ	FALSE	; No RT: detection (Exit)
.Ift HTMESS	.Data HTMESS	equ	TRUE	; HT: detection (Message)
.Iff HTMESS	.Data HTMESS	equ	FALSE	; No HT: detection (Message)
.Ift HTEXIT	.Data HTEXIT	equ	TRUE	; HT: detection (Exit)
.Iff HTEXIT	.Data HTEXIT	equ	FALSE	; No HT: detection (Exit)
.Iff $SIL    	.Data SILENT	equ	TRUE	; No Startup and Exit messages
.Ift $SIL    	.Data SILENT	equ	FALSE	; Display Startup and Exit messages
.Ift $STA    	.Data STATUS	equ	TRUE	; Status Line support
.Iff $STA    	.Data STATUS	equ	FALSE	; No Status Line support
.Ift $STA .Iff $STADF	.Data STADEF	equ	0	; Status line off by default
.Ift $STADF 	.Data STADEF	equ	1	; Status line on by default
.Ift $VT2    	.Data VT2XX	equ	TRUE	; VT2plus support included
.Iff $VT2    	.Data VT2XX	equ	FALSE	; VT2plus support not included
.Ift $VT4    	.Data VT4XX	equ	TRUE	; VT4plus support included
.Iff $VT4    	.Data VT4XX	equ	FALSE	; VT4plus support not included
.Ift $TD2    	.Data TDV2XX	equ	TRUE	; TDV2230 support included
.Iff $TD2    	.Data TDV2XX	equ	FALSE	; TDV2230 support not included
.Ift $TMO	.Data TIMOUT	equ	TRUE	; Time-Out support included
.Iff $TMO	.Data TIMOUT	equ	FALSE	; Time-Out support not included
.Ift $TMO   	.Data TMOVAL	equ	'$TMON'	; Time-Out in '$TMON' minutes
.Ift $TMO .Ift $TMS	.Data TMOSET	equ	TRUE	; Time-Out setable
.Ift $TMO .Iff $TMS	.Data TMOSET	equ	FALSE	; Time-Out fixed
.Ift $TMO .Ift $TMS .Iff $TMODF	.Data TMOON	equ	0	; Time-Out OFF by default
.Ift $TMO .Ift $TMS .Ift $TMODF	.Data TMOON	equ	1	; Time-Out ON by default
        .Data MAXFIF	equ	'$MAXFI%D'	; Maximum number of entries in FIFO
	.Data MAXHLP	equ	'''MAXHLP'''	; Highest available help page
	.Data MINCHR	equ	'$MINCH%D'	; Minimum cmd-line length to be saved
        .Close
        .Ift MCR 	PIP MCEPRE.INC/PU/NM
        .Iff MCR 	PURGE MCEPRE.INC
;MCEBLD -- Creating assembly command file MCEASM.CMD
        .Open MCEASM.CMD
        .Data ;
        .Data ; MCE Macro build file
        .Data ; Created on '<Date>' '<Time>' by MCEBLD.CMD Version: 'VERS'
        .Data ;
        .Data MCE,MCE='$SRC'
        .Close
        .Ift MCR 	PIP MCEASM.CMD/PU/NM
        .Iff MCR 	PURGE MCEASM.CMD
;MCEBLD -- Creating task builder command file MCETKB.CMD
        .Open MCETKB.CMD

        .Iff PLUS .Goto 680$
	.; any RSX280 specifics here
.680$:
.Enable Data
;
; MCE task build file
; Created on '<Date>' '<Time>' by MCEBLD.CMD Version: 'VERS'
;
.Disable Data
.;.Iff $MU .Iff $ID .Data MCE/CP,MCE/-SP=MCE
.;.Ift $MU .Iff $ID .Data MCE/MU/CP,MCE/-SP=MCE
.;.Iff $MU .Ift $ID .Data MCE/ID/CP,MCE/-SP=MCE
.;.Ift $MU .Ift $ID .Data MCE/ID/MU/CP,MCE/-SP=MCE
.Data MCE=MCE,LB:[SYSTEM]FCSLIB/LB,LB:[SYSTEM]SYSLIB/LB/TASK=...MCE/PRI=60/EXTTSK=2400/ASG=TI:1,SY:2
.;.Enable Data
.;/
.;Units=2
.;Asg=TI:1
.;Asg=SY:2
.;Pri=60
.;Task=...MCE
.;'LIBR'
.;Exttsk=2400  ; Can be extended to add more MCE pool space
.;/
.;.Disable Data
        .Close
    	.Ift MCR	PIP MCETKB.CMD/PU/NM
    	.Iff MCR	PURGE MCETKB.CMD
.MAC:
;MCEBLD -- Assembling MCE
	.;
    MAC @MCEASM
        .If <ExStat> Ne <Succes> .Goto 2000$
        .;
	.Ift MCR	PIP MCE.OBJ/PU/NM,MCE.LST/PU/NM
	.Iff MCR	PURGE MCE.OBJ,MCE.LST
.LINK:
;MCEBLD -- Building MCE
	.Ift MCR	TKB @MCETKB
	.Iff MCR	LINK @MCETKB
        .If <ExStat> Ne 1 .Goto 2000$
.;
    	.Ift MCR	PIP MCE.TSK/PU/NM,MCE.MAP/PU/NM,MCE.SYM/PU/NM
    	.Iff MCR	PURGE MCE.TSK,MCE.MAP,MCE.SYM
.;
	.IfNdf $COP	.SetF	$COP
        .Iff $COP .Goto 1000$
    	.Ift MCR	PIP LB:'$SYSDR'/NV/FO/CO=MCE.TSK
    	.Iff MCR	COPY/NEW/CONT MCE.TSK LB:'$SYSDR'
        .Iff $INS .Goto 1000$
;MCEBLD -- Installing MCE
        .IfAct ...MCE   ABO ...MCE
        .IfIns ...MCE   REM ...MCE
    INS LB:'$SYSDR'MCE
.;
.1000$:
	.IfNdf $PRI	.SetF	$PRI
        .Ift $PRI .Ift MCR	PRI MCE/FLA=MCE.LST,MCE.MAP
        .Ift $PRI .Iff MCR	PRINT/FLAG/JOB=MCE MCE.LST,MCE.MAP
;MCEBLD -- Done.
        .Exit   <Succes>
.2000$:
;
;MCEBLD -- Something went wrong!!!
;
        .Exit   <Warnin>
