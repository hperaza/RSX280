CPM - A CP/M Emulator for RSX280
================================

This repository contains a CP/M emulator for the RSX280 operating system.

It is written in Zilog Z280 assembly language and is based on an earlier
version for UZI-280 (a Unix edition 7 operating system for the Z280)
by Stefan Nitschke and released into the Public Domain in 1996.

RSX280 is being developed by Hector Peraza and is available from
https://github.com/hperaza/RSX280

****  NOTE: THIS IS NOW A TENTATIVE WORKING VERSION  ****

CPM is a program to allow "well-behaved" Z80 CP/M programs to run
under RSX280.  The implementation only supports basic CP/M 2.2
BDOS functionality and console-only BIOS routines.  None of the
usual CP/M 2.2 BIOS routines for accessing other disk drives,
or character devces like a printer, punch and reader are
available.

Further documentation will be added at a later time.

Revision history is now in reverse chronological order.


Update: 28-Dec-2021
-------------------

Hector Peraza has corrected and enhanced the emulator considerably.
I've just pulled his changes back into this master repository.  Most
of his updates are documented in the new Changelog.txt file (and I
will move this update section of this README into this file shortly).
Much of the emulation is now accurately running most well behaved CP/M
2.2 programs.


Update: 14-Jun-2020
-------------------

Added Compute File Size (BDOS function 35) and tried to squeeze
a few more bytes of code-space.


Update: 12-Jun-2020
-------------------

Reorganise source-code and improve comments.  Only minor functional
changes to the emulation of the CP/M file control block contents
have been made, and an attempt at building the disk allocation
vector upon starting the emulator.

There are some outstanding issues -

* Console input sometimes gets out-of-sync when the terminal input
AST routine adds a character to the input ring buffer (we have no
control over disabling/enabling RSX280 system interrupts).

* Still to-do - fine-tune the CP/M allocation vector emulation so
that directory accessing programs can see and report on disk usage
in blocks correctly.

* Files opened for updating from CP/M do not seem to be writing
file sectors for existing files (e.g. running a configuration
program to update a terminal-type does not change the first sector
in a .COM file where these things are stored).


Update: 26-May-2020
-------------------

Search First/Search Next (BDOS functions 17 & 18) are mostly working.
Contiguous files from RSX280 (mainly task image *.TSK files) are
not being returned to the emulation.

The Digital Research SDIR program is generally working and able to
list out the CP/M directory with file sizes.  The size of CP/M extents
is not yet consistent though - and I'm investigating whether an
emulation of the allocation vector bitmap is needed.  The disk
parameter block values in-use emulate an 8Mb hard disk with up to
1024 directory entries.  STAT *.* also reports correct file
information.

I've changed command input routine so that it may no longer accept
program filenames from another user directory.  Instead I'll be
considering adding a search-path instead - so if a CP/M program is
not in the user's home directory then another directory will be
searched (most likely SY0:[CPM]).

Also I've used the CP/M assembler ASM on DUMP.ASM and loaded the
resultant LOAD.HEX file to produce a running DUMP.COM and tried
out a CP/M visual editor (I use PMATE) to edit test files.
Infocom games like ZORK I and Hitchhiker's Guide to the Galaxy
also work fine.


Update: 18-May-2020
-------------------

I've been through a few iterations trying to implement the BDOS search_first
and search_next functions (17 and 18).  At first I tried using RSX280
native directory searching like the PIP program uses - but this ended
up being too complex and large (and it doesn't scale if you have a
large number of files in your home directory).

As a consequence, I've approached this from the CP/M BDOS source
and made some assumptions.  Firstly, all files visible from CP/M
*MUST* be only up to eight-character filenames (not nine), and only
files with version number 1 (e.g. FILE.DAT;1) are seen by CP/M
(as FILE.DAT).

I'm using the RSX280 FCS routines to walk through the user's
directory and computing file sizes (in CP/M records of 128-bytes)
by looking up the SY:[MASTER]INDEXF.SYS;1 index file.  I'm able to
do a Search First and Search Next to retrieve the correct results
the first time through, however, trying a second time fails.  I'm
suspicious of problems with the system's FSEEK routine and will
just post a "where I'm at" snapshot and look at it further soon.
Debugging messages are everywhere at present.


Update: 07-May-2020
-------------------

BDOS Console I/O and sequential File functions are working.

Still to-do Search First/Next functions and verify Random File
functions.

A log file follows -

```
>run cpm
CP/M 2.2 emulator 59K V0.99 [DEBUG]
Use ^Z followed by Return/Enter to exit.

X>[CPM]CAL 2020 /D
Calendar,  Version 1.3
Name of Disk Output File? cal.tmp
Calendar Output File/Device is cal.tmp

X>[CPM]PIP CON:=CAL.TMP
                       Calendar of Year 2020


Calendar for JANUARY   Calendar for FEBRUARY  Calendar for MARCH
Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa
          1  2  3  4                      1    1  2  3  4  5  6  7
 5  6  7  8  9 10 11    2  3  4  5  6  7  8    8  9 10 11 12 13 14
12 13 14 15 16 17 18    9 10 11 12 13 14 15   15 16 17 18 19 20 21
19 20 21 22 23 24 25   16 17 18 19 20 21 22   22 23 24 25 26 27 28
26 27 28 29 30 31      23 24 25 26 27 28 29   29 30 31


Calendar for APRIL     Calendar for MAY       Calendar for JUNE
Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa
          1  2  3  4                   1  2       1  2  3  4  5  6
 5  6  7  8  9 10 11    3  4  5  6  7  8  9    7  8  9 10 11 12 13
12 13 14 15 16 17 18   10 11 12 13 14 15 16   14 15 16 17 18 19 20
19 20 21 22 23 24 25   17 18 19 20 21 22 23   21 22 23 24 25 26 27
26 27 28 29 30         24 25 26 27 28 29 30   28 29 30
                       31

Calendar for JULY      Calendar for AUGUST    Calendar for SEPTEMBER
Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa
          1  2  3  4                      1          1  2  3  4  5
 5  6  7  8  9 10 11    2  3  4  5  6  7  8    6  7  8  9 10 11 12
12 13 14 15 16 17 18    9 10 11 12 13 14 15   13 14 15 16 17 18 19
19 20 21 22 23 24 25   16 17 18 19 20 21 22   20 21 22 23 24 25 26
26 27 28 29 30 31      23 24 25 26 27 28 29   27 28 29 30
                       30 31

Calendar for OCTOBER   Calendar for NOVEMBER  Calendar for DECEMBER
Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa
             1  2  3    1  2  3  4  5  6  7          1  2  3  4  5
 4  5  6  7  8  9 10    8  9 10 11 12 13 14    6  7  8  9 10 11 12
11 12 13 14 15 16 17   15 16 17 18 19 20 21   13 14 15 16 17 18 19
18 19 20 21 22 23 24   22 23 24 25 26 27 28   20 21 22 23 24 25 26
25 26 27 28 29 30 31   29 30                  27 28 29 30 31


X>^Z
>
```

Update: 01-May-2020
-------------------

The original UZI-280 version was missing some CP/M BDOS functions.
These have been added.  Console I/O and Disk file Open/Create/Read/Write
appear to be working.   Still to-do file Remove, Random access Read/Write,
Directory Search and some other BDOS functions.

