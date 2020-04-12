This directory contains the sources for the Monitor Console Routine (MCR),
which is the default CLI of RSX180.

MCR consists of two tasks:

 * A resident task that receives commands from all terminals, processes the
   simplest ones (mostly task control which do not ptoduce output) and
   dispatches the rest to the secondary task.
 * A secondary task that is started on the terminal where the command
   originated, and is used to process more complex commands or commands
   that produce output.

