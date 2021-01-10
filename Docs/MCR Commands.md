## MCR Commands

Like in RSX-11M, when entering a command name only the first three letters are meaningful to MCR, the rest is ignored; the only exception being the command HELP. If the command is not recognized as internal, MCR will try to find an installed task with the same name and execute it.

**ABO** [_taskname_]

> Terminates (aborts) execution of the specified task. If no task name is specified, the task started by a RUN command (TT_nn_) is aborted.
> 
> Privileged users can abort any task. Non-privileged users can only abort tasks started from their own terminal.

**ACS** _ddn_**:/BLKS=**_n_

> Privileged command. Allocates a checkpoint file on the specified device if _n_ is greater that zero. If _n_ is zero, the use of the checkpoint file is discontinued.

**ACT** [**/ALL**][**/TERM=**_ttn_:]

> Show active tasks started from the current terminal.
> 
> If the command is issued with the /ALL switch, all active tasks in the system are displayed. To display the task names of all the active tasks for a specific terminal, use the /TERM switch.

**ALT** _taskname_**/PRI=**_nnn_  
**ALT** _taskname_**/RPRI=**_nnn_

> Privileged command, alters task priority. /PRI sets the task's default priority, while /RPRI sets the task's running priority.

**ASN** _physical_=_logical_[**/GBL**][**/LOGIN**][**/TERM=**_ttn:_]  
**ASN** =[_logical_][**/GBL**][**/LOGIN**][**/TERM=**_ttn:_]  
**ASN** [**/GBL**][**/LOGIN**][**/TERM=**_ttn:_]

> Defines, deletes or displays logical device assignments.
> 
> The options are:
> 
> **/GBL**
> 
> Defines, deletes or displays global logical-device assignments
> 
> **/LOGIN**
> 
> Defines, deletes or displays login logical-device assignments
> 
> **/TERM=**_ttn:_
> 
> Defines, deletes or displays logical-device assignments for the specified terminal
> 
> All option switches are privileged. Non-privileged users can define, delete and display only local logical-device assignments.

**ATL** [_taskname_]

> Display the name and the status of all active tasks in the system, or the status of a particular task.

**BRO** _ttn_:_message_  
**BRO** **LOG**:_message_  
**BRO** **ALL**:_message_

> Send (broadcast) _message_ to one terminal (_ttn_), to logged-in terminals (LOG), or to all terminals (ALL).
> 
> ALL and LOG are privileged keywords.

**BYE**

> Terminates the current session and logs the user out of the system.

**CAN** [_taskname_]

> Privileged command, cancels time-based initiation requests for a task (see the RUN command). If any time-based schedule requests for the task exist, they are removed. If the task is executing, the execution is not affected.

**CLQ**

> Displays information about tasks currently in the clock queue. Any pending time-based schedule requests will be displayed.

**DEV**  
**DEV** _dd:_  
**DEV** _ddn:_  
**DEV /LOG**  
**DEV /MOU**

> With no additional options, the DEV command displays the names and status of all devices in the system.
> 
> If dd: is specified, only all units of the specified device will be listed.
> 
> If ddn: is specified only the status of the specified device and unit will be listed.
> 
> Use the /LOG switch to list all logged-in terminals.
> 
> Use the /MOU switch to list all mounted devices.

**DMO** _ddn:_

> Dismounts the specified device. Privileged unless the device is mounted public or private to the terminal.

**FIX** _taskname_

> Privileged command, loads and locks a task in memory. The task specified in the argument must be installed and inactive.
> 
> Fixed tasks remain physically in memory even after they exit. They do not have to be loaded when a request is made to run them.

**HEL** [_username_]  
**HELLO** [_username_]

> Logs onto this system. If _username_ is not specified, the user will be prompted for one.

**HELP** [_topic_ [_subtopic_]]

> Displays help about the specified _topic_.

**INS** [**$**]_filename_[**/INC=**_n_][**PRI=**_n_][**/TASK=**_name_][**/RUN**[**=REM**][**/CKP=NO**][**/ACP=YES**]

> Enters (installs) a task into the System Task Directory (STD), making it known to the system.
> 
> The options are:
> 
> **/PRI=**_n_
> 
> Set task running priority to _n_
> 
> **/RUN**[**=REM**]
> 
> Start task immediately. If REM is specified, the task will be removed on exit.
> 
> **/INC=**_nnn_
> 
> Specify a memory increment
> 
> **/TASK=**_name_
> 
> Specify an alternate task name
> 
> **/CKP=NO**
> 
> Disables checkpointing of the installed task
> 
> **/ACP=YES**
> 
> Indicates that the task is an Ancillary Control Processor
> 
> Privileged users can specify any options. Non-privileged users are allowed to use the INS command only when the option /RUN=REM is specified.

**INI** _ddn_**:**[[**"**]_label_[**"**]][**/**_options_...]

> Initializes a file storage device for use with RSX180.
> 
> The _options_ can be:
> 
> **/CF**
> 
> Ask (**/-CF** do not ask) for confirmation before initializing the main directory and related files. Default is to ask.
> 
> **/MF:**_nnn_ or **/MF=**_nnn_
> 
> sets the maximum number of files (entries in the index file) that can be stored in the volume.
> 
> **/PR:**_hhhh_ or **/PR=**_hhhh_ or **/PR=[**_rwed,rwed,rwed,rwed_**]**
> 
> where _hhhh_ is an hexadecimal value and _rwed_ *r*ead-*w*rite-*e*xecute-*d*elete protection bits specification. Sets the default file protection for newly created files. Default is /PR=FFFF or /PR=[rwed,rwed,rwed,rwed] (all actions granted to everyone.)
> 
> **/WB**
> 
> write boot loader and update system file information in boot blocks.

**LUN** _taskname_

> Displays static LUN assignments of a task.

**MOU** _ddn_**:**[**/ACP=**_acpname_]

> Connects a device to the filesystem processor so files can be accessed.
> 
> The /ACP option can be used to specify an alternate Ancillary Control Processor.
> 
> The command is privileged unless the device is public.

**PAR**

> Display existing memory partitions.

**RED** _new_=_old_

> Privileged command, redirects I/O requests from _old_ device to _new_.

**REM** _taskname_

> Privileged command, deletes a task from the System Task Directory (STD) and thereby removes the task from the system. This is the complement of the MCR INS command.

**RES** [_taskname_]

> Resumes the execution of a stopped task. Non-privileged users can resume only tasks started from their own terminal.

**RUN** [**$**]_filename_[**/PRI=**_n_][**/TERM=**_ttn:_][**/INC=**_nnn_][**/TASK=**_name_][**/CKP=NO**][**/CMD=**_command_]

> Run a task with the specified options:
> 
> **/PRI=**_n_
> 
> Set task running priority to _n_
> 
> **/TERM=**_ttn:_
> 
> Start task on the specified terminal
> 
> **/INC=**_nnn_
> 
> Specify a memory increment
> 
> **/TASK=**_name_
> 
> Specify an alternate task name
> 
> **/CKP=NO**
> 
> Disable task checkpointing
> 
> **/CMD=**_command_
> 
> Specify a command line argument, must be the last option in the line

**RUN** _taskname_ _dtime_[**/RSI=**_rsi_]

> Time-scheduled task request: run the specified task at a time increment from now, where _taskname_ is the full 6-character name of the installed task to run, and _dtime_ is the time interval and has the form _nnnu_, where _nnn_ is a numeric value and _u_ the units (H = hours, M = minutes, S = seconds, T = ticks). Privileged command.
> 
> If specified, the RSI option determines the reschedule interval (i.e. how often the task will be re-run.) The format of the _rsi_ parameter is the same as for _dtime_.

**SET** **/**_option_

> where _option_ can be any of the following:
> 
> **/ECHO=**_ttn:_
> 
> Enable local echo
> 
> **/NOECHO=**_ttn:_
> 
> Disable local echo
> 
> **/LOWER=**_ttn:_
> 
> Enable lowercase input
> 
> **/NOLOWER=**_ttn:_
> 
> Convert lowercase input to uppercase
> 
> **/CRT=**_ttn:_
> 
> Enables backwards character delete option
> 
> **/NOCRT=**_ttn:_
> 
> Disables backwards character delete option
> 
> **/SLAVE=**_ttn:_
> 
> Slave terminal (reject unsolicited input)
> 
> **/NOSLAVE=**_ttn:_
> 
> Remove slave status from terminal
> 
> **/BRO=**_ttn:_
> 
> Enable receiving of broadcast messages
> 
> **/NOBRO=**_ttn:_
> 
> Disable receiving of broadcast messages
> 
> **/LOGON**
> 
> Enable user login
> 
> **/NOLOGON**
> 
> Disable user login
> 
> **/PRIV=**_ttn:_
> 
> Make terminal privileged
> 
> **/NOPRIV=**_ttn:_
> 
> Make terminal non-privileged
> 
> **/DIR=[**_dir_**]**
> 
> Set or display current directory
> 
> **/PUB=**_ddn:_
> 
> Establish the specified device as public device
> 
> **/HOST=**_hostname_
> 
> Displays or sets the host name
> 
> **/COLOG**
> 
> Displays the current console terminal and logfile assignments
> 
> **/COLOG=ON**
> 
> Starts console logging
> 
> **/COLOG=OFF**
> 
> Stops console logging
> 
> **/COLOG/NOCOTERM**
> 
> Disables console terminal
> 
> **/COLOG/COTERM**[**=**_ttn:_]
> 
> Changes the console terminal assignment. If _ttn:_ is not specified, the most recent assignment is restored (or TT0: after boot.)
> 
> **/COLOG/NOLOGFILE**
> 
> Disables the log file
> 
> **/COLOG/LOGFILE**[**=**[_filespec_]]
> 
> Changes the log file assignment. If _=filespec_ is not specified, the default log file is restored. If only = is specified, a new version of the log file will be created.
> 
> **/RNDC=**_nnn_
> 
> Displays or sets the duration of the scheduler's round-robin interval in ticks
> 
> **/RNDL=**_nnn_
> 
> Displays or sets the lowest priority to be considered for round-robin scheduling
> 
> **/RNDH=**_nnn_
> 
> Displays or sets the highest priority to be considered for round-robin scheduling
> 
> **/SWPC=**_nnn_
> 
> Displays or sets the duration of the swapping interval in ticks
> 
> **/SWPR=**_nnn_
> 
> Displays or sets the priority range for swapping
> 
> LOGON, NOLOGON, PRIV, NOPRIV, PUB, HOST, COLOG, RNDC, RNDH, RNDL, SWPC and SWPR are privileged options. Non-privileged users are only allowed to change the characteristics of their own terminal.

**STP** _taskname_

> Stops the execution of the specified task. Non-privileged users can only stop tasks started from their own terminal.

**SYN**

> Flushes the filesystem buffers.

**TAL** [_taskname_]

> If _taskname_ is not specified, information is displayed for all tasks installed in the system. The display format is the same as that of the MCR ATL command.

**TAS** [_taskname_][**/DEV=**_ttn:_]

> Displays the list of installed tasks (System Task Directory).

**TIM** [_hh_:_mm_[:_ss_]] [_MM_/_DD_/_YYYY_]  
**TIM** [_hh_:_mm_[:_ss_]] [_DD_-_MMM_-_YYYY_]

> Displays or sets the system date and time. If no argument is specified, the command displays the current time and date. Only privileged users can set the date and time.

**UFD** _ddn_:**[**_dirname_**]**_/options..._

> Create user directory. Note that the square brackets around _dirname_ are part of the syntax and therefore mandatory. The command accepts the following options:
> 
> **/ALLOC=**_n_
> 
> Pre-allocate the specified number of blocks (currently ignored)
> 
> **/PROT=[**...**]**
> 
> Set the access protection bits (currently ignored)
> 
> **/OWNER=[**_grp,usr_**]**
> 
> Set the directory ownership

**UNF** _taskname_

> Privileged command, frees a fixed task in memory. This is the complement of the MCR FIX command.

**Catch-all task support**

Any command line that is not recognized by MCR is passed entirely to a task named **`...CA.`** if such task is installed. The mechanism allows using an external task as a CLI extension.


