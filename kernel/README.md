This directory contains the sources of the RSX180 kernel.

Some modules access Z180-specific features (mostly MMU, interrupt vectors
and timer), while at least one other accesses P112-specific hardware
components (RTC and NVRAM).

Access to any other hardware components is done via the device drivers.

