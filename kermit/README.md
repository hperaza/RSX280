# KERMIT for RSX180 and RSX280
Here you can find the CP/M Kermit-80 ported to the RSX180/280 OS.

Kermit-180 has been tested on a P112 (Z180) machine running RSX180 and on a Z280RC (Z280) machine running RSX280.
## Kermit-180 Capabilities at a glance
|Capability|Implemented|
|--|--|
|Local operation|Yes|
|Remote operation|Yes|
|Login scripts|Yes, limited|
|Transfer text files|Yes|
|Transfer binary files|Yes|
|Wildcard send|Yes|
|File transfer interruption|Yes|
|Filename collision avoidance|Yes|
|Can time out|Yes|
|8th-bit prefixing|Yes|
|Repeat count prefixing|No|
|Alternate block checks|Yes|
|Terminal emulation|Yes, limited|
|Communication settings|If supported by the terminal driver|
|Support for dial-out modems|No|
|Transmit BREAK|If supported by the terminal driver|
|IBM communication|Yes|
|Transaction logging|No|
|Debug logging|No|
|Session logging|Yes|
|Raw file transmit|Yes|
|Act as server|Yes|
|Talk to server|Yes|
|Advanced commands for servers|Yes|
|Command/init files|Yes|
|Command macros|No|
|Local file management|Yes|
|Handle file attributes|No|
|Long packets|No|
|International Character Sets|No|
|Sliding Windows|No|
|Printer control|No|

## Quick start guide

1. Start Kermit at the PC side and setup it accordingly, the following settings were used with C-Kermit 9.0.302 under Linux:  
`set line /dev/ttyUSB1`   (or whatever line your machine is connected to)  
`set speed 19200` (this is the default for the Z280RC quad-serial board under RSX280)  
`set attributes off`  
`set parity space` (or `set parity none`)  
`set file type binary`  
`set carrier-watch off`  
`set flow-control none`  
`set control-character prefixed all`  
`set file patterns off`  
`set transfer mode manual`  
To avoid having to type the above commands every time, you can save them into a text file and then use the Kermit's `take` command to execute them.

2. Start Kermit-180 on the RSX180/280 side:  
`run $kermit` (if Kermit-180 is not installed)  
`ker` (if Kermit-180 is installed as ...KER)  
Once it is running, setup the communication parameters. The following settings were used on a Z280RC running RSX280:  
`set line tt3:` (or whatever line the Z280RC is connected to the PC)  
`set parity space` (or `set parity none`, but must match the PC settings above)  
`set file-mode binary`  
As with the PC Kermit, you can save the above command sequence into a text file for later execution via the `take` command. If you name the file `kermit.ini` and place it into your user's directory, Kermit-180 will execute it automatically on startup.

4. You can now use the `send` command on the PC side and the `get` command on the RSX280 side to transfer a file from the PC to the Z280RC. Do the opposite to send a file from the Z280RC to the PC.

5. Best is just to put the PC Kermit in `server` mode. Then, you can send and receive files by only entering commands at RSX180/280 terminal. E.g.  
`send examp.cmd` to send a file to the PC  
`get kermit.tsk` to fetch a file from the PC  
`remote dir` to browse the PC directory  
`dir` to browse the local RSX180/280 directory  
etc.

## Bugs and Limitations
Kermit-180 is built by default as a non-privileged task, meaning that in *local* mode it cannot change the characteristics of the terminal used for communications unless the terminal has been set to *public* by the system administrator. Specifically, Kermit-180 needs to set the terminal mode to *slave* with *echo disabled,* and those are privileged operations for terminals other than the user's current one. *Remote* mode does not suffer from that limitation, as it uses the user's current terminal for communications.

The filename completion (e.g. for DIR or ERA commands) is not working fully correctly yet: pressing ESC or TAB a second time appends part of the file extension again to the completed file name, etc.

The current version of Kermit-180 is not able to process file attribute information, and therefore the correct file type (text or binary) has to be set manually at both ends before starting the transfer.

Some modern Kermit programs (e.g. C-Kermit 9.0 on Linux) don't prefix all control characters by default as the protocol specifies, and therefore file transfers the PC to the Z280RC may fail randomly. The solution is to use the `set control-character prefixed all` command at the PC side. I'd recommend using `set file patterns off` and `set transfer mode manual` as well, otherwise C-Kermit will try to switch automatically to ascii mode when transferring text files in binary mode without Kermit-180 being aware of the change. The result is a text file with duplicated carriage return characters being received at the RSX180/280 side if the file at the PC side already had CR/LF line endings.
