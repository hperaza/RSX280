This directory contains the sources for the (secondary) hardware bootstrap loaders.

The Z280RC board normally boots from the CompactFlash disk. The RSX280 boot sequence is as follows:

 - The primary boot loader resides in block 0 of the CF and is used to load the ROM monitor.
 - The B monitor command loads the secondary bootstrap loader (see the [cfboot.mac](cfboot.mac) file) at address 0000h and executes it. The Z280 CPU is assumed to be in System Mode, with either the MMU disabled or enabled with virtual addresses matching physical addresses (Z80-compatible mode.)
 - The secondary bootstrap loader enables the MMU, maps the top page (virtual address F000h) to physical address 080000h (above 512K) relocates itself to virtual address F000h and continues execution from there.
 - After validating the RSX280 partition, the loader loads the system into contiguous memory pages starting from physical address 000000h using the first page (virtual address 0000h) as a sliding-window. This allows loading system images larger than 64K (but smaller than 512K!).
 - The loader maps the first part of the loaded system (which contains the kernel code) to the first 15 pages, and jumps to the kernel entry point.
 - The kernel reinitializes the MMU and starts the system.

