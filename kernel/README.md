This directory contains the sources of the RSX280 kernel.

Some modules access Z280-specific features (mostly MMU, interrupt vectors
and timer), while at least one other accesses machine-specific hardware
components (RTC and NVRAM).

Access to all other hardware components is done via the device drivers.

