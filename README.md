# RSX180

RSX180 is an Operating System for the Zilog Z180 CPU that is similar in look
and feel to the old DEC's RSX-11M.

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

More details [here](http://p112.sourceforge.net/index.php?rsx180).

## Hardware supported

P112 CPU board (Z182) with:

 * Kernel: Dallas DS1202/DS1302 (RTC and NVRAM)
 * Terminal driver: two serial ports (Z182 and FDC37C655)
 * Floppy disk driver (FDC37C655)
 * Hard disk driver (GIDE)
 * Parallel (printer) port (FDC37C655)

## Compiling the system

Follow the instructions listed in the Docs/Compiling.txt file.

## Bugs and limitations

Please note that this still is a work in progress. The kernel is rather
complete and stable, the system can be booted and used, but there are many
unfinished details, some basic utilities are still missing features and
bugs are very likely hiding somewhere in the code.

