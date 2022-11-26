# RSX280

RSX280 is an Operating System for the Zilog Z280 CPU that is similar in look and feel to the old DEC's RSX-11M.

Features:

 * Multi-tasking.
 * Multi-user.
 * Multi-terminal.
 * Priority-based micro-kernel with round-robin scheduling of tasks of the same priority.
 * [QIO](http://www.wikipedia.com/wiki/QIO) mechanism.
 * [AST](http://www.wikipedia.org/wiki/Asynchronous_System_Trap) support.
 * [Event Flags](https://en.wikipedia.org/wiki/Event_flag).
 * Send-Receive inter-task communication mechanism.
 * Kernel functionality extended by *privileged* tasks.
 * Task Directory for fast task activation.
 * Tasks can be fixed in memory for even faster activation.
 * Device drivers.
 * Dynamic allocation of system resources.
 * Fork processes.
 * Clock queue and time-scheduled task execution.
 * Privileged and non-privileged users.
 * 2-level filesystem structure.
 * Indirect Command Processor.

RSX280 is a direct port of RSX180, and therefore compatible at both command
and system call level. More details [here](http://p112.sourceforge.net/index.php?rsx180).

## Hardware supported

Bill Shen's Z280RC board with:

 * Kernel: Dallas DS1202/DS1302 (RTC and NVRAM)
 * Terminal driver: five serial ports (Z280's internal UART and Bill Shen's OX16C950 quad-serial port board)
 * Compact Flash disk driver

Tilmann Reh's CPU280 board with:

 * Kernel: Dallas DS1287A (RTC and NVRAM)
 * Terminal driver: two serial ports (Z280's internal UART and 81C17 on-board UART)
 * Floppy disk driver
 
## Bugs and limitations

Please note that this still is a work in progress. The kernel is rather complete and stable, the system can be booted and used, but there are many unfinished details, some basic utilities are still missing features and bugs are very likely hiding somewhere in the code.

