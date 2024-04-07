# Dynamic Debugging Tool for RSX280

DDT is a debugger that makes use of the RSX280 kernel support for the breakpoint-on-halt and single-step capabilities of the Z280 CPU. DDT runs as an external task that connects to the task being debugged and takes control of it. It is intended as a more user-friendly replacement for ODT.

Advantages compared to ODT:

 * Does not need to be linked to the application, and thus takes no extra code or data space from the task being debugged.
 * Avoids having to build two versions of the same task for debugging and release.
 * Can connect to an already-running task.
 * Debugging is "transparent" to the task being debugged.

Disadvantages:

 * Slower than ODT due to the inherent back-and-forth task switching and message-passing between the debugger and debugged task.
 * Runs as privileged task (but nevertheless ensures that the user has the right privileges to start a debugging session and to access task memory).
 * Requires a separate terminal for the debugging session (unlike ODT, which can share terminal I/O with the task being debugged).

## Command line syntax

The debugger recognizes the following commands:

 * CONNECT *tsknam* -- Connects to the active task *tsknam* (the name must be fully qualified); if the connection is successful, the task execution is interrupted and the debugging session is started.
 * RUN *tsknam* -- Requests the task *tsknam* and starts a debugging session. The task does not need to be installed, in which case *tsknam* refers to the task image filename.
 * TERMINAL *ttn:* -- Use terminal *ttn:* as the debugging console.

The command names can be shortened to the minimum unambiguous sequence of characters, in this case down to a single letter.

## Debugger commands

The following commands are available during the debugging session ("_" prompt):

 * D *addr1,addr2* -- Dump memory from *addr1* to *addr2* included. If *addr1* is omitted, the dump will continue from where the last dump operation stopped, or from the current contents of the user HL register after a breakpoint hit or a single-step operation. If *addr2* is omitted, a full 256-byte page will be output.
 * L *addr,n* -- List (disassemble) *n* instructions starting from *addr*. If *addr* is not specified, the disassembly will continue from there the last L command ended, or from the current user PC after a breakpoint or single-step operation. If *n* is omitted, the next 16 instructions are displayed. Note that *n* is in hexadecimal.
 * E *addr* -- Examine/modify memory starting from *addr*. The address of the memory location is displayed, followed by the old contents. Entering a new value modifies the contents, pressing enter on an empty line leaves the contents unchanged. End with Ctrl-C or with a dot.
 * F *addr1,addr2,byte* -- Fill memory region from *addr1* to *addr2* inclusive with *byte*. Any omitted parameter is assumed to be zero.
 * S *addr1,addr2,byte* -- Search memory region from *addr1* to *addr2* inclusive for *byte*. Any omitted parameter is assumed to be zero.
 * B *addr* -- Place a breakpoint (HALT instruction) at address *addr*. Up to eight breakpoints can be specified. Without the *addr* argument the command lists the current breakpoints.
 * C *addr* -- Clear breakpoint at *addr*.
 * G *addr* -- Go to address *addr*. Starts executing the user code until a breakpoint is hit. If *addr* is not specified, the execution will continue from the current user PC value. You can interrupt the execution at any moment, before the next breakpoint is hit, by pressing Control-C at the debugger console; this is especially useful in such cases as when the task enters an infinite loop to determine the cause of misbehavior.
 * T *n* -- Trace the next *n* instructions (one if *n* is not specified) starting from the current user PC value. Note that *n* is in hexadecimal.
 * I -- Displays status information about the debugged task, using a format similar to the MCR ATL command.
 * W *addr* -- Display 16-bit word contents at the memory location *addr*.
 * X -- Display the contents of the CPU registers.
 * X *rp* -- Examine/modify CPU register pair *rp*.
 * Q -- Ends the debugging session and disconnects from the debugged task, which will then resume normal execution.
 * QA -- Ends the debugging session and Aborts the debugged task.

## Limitations

Debugging certain privileged tasks can lead to system dead-locks, e.g. when single-stepping into code that disables task dispatching prior to accessing the kernel database (this applies to ODT too). If the privileged task maps the kernel database, the debugger D command will fail to display the mapped kernel data (this will be fixed in a future version).

Debugging SST service routines is not faultless either and can lead to recursive invocation of the SST handler, in particular for tasks that trap breakpoint-on-halt and/or single-step exceptions. Debugging of AST routines is OK.

