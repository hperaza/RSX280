This directory contains the sources for the RSX280 device drivers for Bill
Shen's (Plasmo) Z280RC board:

* `TT:` Terminal device driver supporting the Z280 internal serial port as
  the system console (TT0:) and Plasmo's OX16C954 quad-serial board
  (TT1:..TT4:). Both are fully interrupt-driven.
* `DU:` "Universal" IDE driver for Compact Flash or Hitachi (IBM) Microdrive,
  supporting up to 8 partitions. The CF distributed with the Z280RC uses
  fixed-size partitions, and therefore the limits are hardcoded inside the
  driver code.
* `NL:` Null pseudo-device, returns EOF on input and ignores output.
* The standard pseudo-devices:
  * `LB:` System boot device
  * `SY:` User's default device
  * `TI:` User's current terminal
  * `CO:` System console, normally redirected to a terminal (e.g TT0:)
  or to a file by the Console Logger Task (COT...)
  * `CL:` Console Listing, normally redirected to TT0:

