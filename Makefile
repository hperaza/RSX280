########################################################################
#                                                                      #
#     Makefile to build the RSX280 system and to create a bootable     #
#     disk image.                                                      #
#                                                                      #
########################################################################

SHELL=/bin/sh

# Path to Linux utilities
ZXCC    = zxcc
VOL180  = ./Tools/linux/vol180/vol180
SYM2INC = ./Tools/linux/sym2inc/sym2inc

# Path to CP/M utilities
ZSM4    = ./Tools/cpm/zsm4.com
TKB     = ./Tools/cpm/tkb.com
LBR     = ./Tools/cpm/lbr.com
DRLIB   = ./Tools/cpm/drlib.com

# System modules
sysmod = startup.rel drivers.lib kernel.lib sysdat.rel

# Source of system modules (directories)
sysdirs = boot drivers kernel

# Source of utilities
utildirs = ldr filesys mcr pip icp rmd vmr utils prvutl ted vdo mce zap cpm calc

# Import user settings
include $(PWD)/Config.make

# Compile a system image, boot sector, mcr, help and utilities
all: update-incs system filesys libs cli utils progdev test games kermit

# Build the Linux tools
linux-tools:
	@(cd ./Tools/linux; ${MAKE})

# Update the system include files in all directories
update-incs:
	@if ! cmp -s inc/sysconf.inc.${platform} inc/sysconf.inc; then \
		(cd inc ; ln -sf sysconf.inc.${platform} sysconf.inc) ; \
		touch inc/sysconf.inc; \
	fi
	@for i in system.inc inc/*inc; do \
		f=`basename $$i` ; \
		d=`dirname $$i` ; \
		find . -name $$f -exec test "{}" -ot $$d/$$f \; -exec cp -v $$d/$$f {} \; ; \
	done

update-system-inc:
	@find . -name system.inc -exec test "{}" -ot ./system.inc \; -exec cp -v ./system.inc {} \;

update-libs:
	@find . -name syslib.lib -exec test "{}" -ot ./mcr/syslib.lib \; -exec cp -v ./mcr/syslib.lib {} \;
	@find . -name fcslib.lib -exec test "{}" -ot ./mcr/fcslib.lib \; -exec cp -v ./mcr/fcslib.lib {} \;

# Compile libraries
.PHONY: libs
libs:	
	@(cd libs/syslib; ${MAKE} all)
	@(cd libs/fcslib; ${MAKE} all)
	@(cd libs/bcdflt; ${MAKE} all)
	@(cd libs/odt; ${MAKE} all)

# Build the system image
system: syssrcs system.sys

# Compile the kernel modules
syssrcs:
	@for i in ${sysdirs}; do \
		echo Making all in $$i ; \
		(cd $$i; ${MAKE} all) ; \
	done
	@cp -u drivers/${platform}/drivers.lib .
	@cp -u kernel/${platform}.rel .
	@cp -u kernel/startup.rel .
	@cp -u kernel/kernel.lib .
	@cp -u kernel/sysdat.rel .

# Link the system modules into a system image file.
# Note: The data segment must be located above 4000h or else it will not be
# fully accessible to privileged tasks. If necessary, use the "d4000" linker
# option.
system.sys: $(sysmod)
	$(ZXCC) $(TKB) -"system.sys,system.sym,system.map=startup/ofmt:com/load=0/cseg=100,kernel.lib,${platform},drivers/lb,sysdat"
	@cat system.map
	$(SYM2INC) system.sym system.dat system.inc

# Compile MCR and the Indirect Command Processor
cli: libs system
	@cp -u libs/syslib/syslib.lib mcr
	@cp -u libs/fcslib/fcslib.lib mcr
	@cp -u system.inc mcr
	@(cd mcr; ${MAKE} all)
	@cp -u libs/syslib/syslib.lib icp
	@cp -u libs/fcslib/fcslib.lib icp
	@cp -u system.inc icp
	@(cd icp; ${MAKE} all)
	
# Compile the system loader
ldr: system
	@cp -u system.inc ldr
	@(cd ldr; ${MAKE} all)

# Compile the filesystem task
filesys: system
	@cp -u system.inc filesys
	@(cd filesys; ${MAKE} all)

# Compile system utilities and basic applications
utils: libs system
	@cp -u libs/syslib/syslib.lib mcr
	@cp -u libs/fcslib/fcslib.lib mcr
	@cp -u libs/syslib/syslib.lib icp
	@cp -u libs/fcslib/fcslib.lib icp
	@cp -u libs/syslib/syslib.lib utils
	@cp -u libs/fcslib/fcslib.lib utils
	@cp -u libs/syslib/syslib.lib prvutl
	@cp -u libs/syslib/syslib.lib pip
	@cp -u libs/fcslib/fcslib.lib pip
	@cp -u libs/syslib/syslib.lib vmr
	@cp -u libs/syslib/syslib.lib rmd
	@cp -u libs/syslib/syslib.lib ted
	@cp -u libs/syslib/syslib.lib vdo
	@cp -u libs/syslib/syslib.lib zap
	@cp -u libs/syslib/syslib.lib mce
	@cp -u libs/fcslib/fcslib.lib mce
	@cp -u libs/syslib/syslib.lib calc
	@cp -u libs/fcslib/fcslib.lib calc
	@cp -u libs/bcdflt/bcdflt.lib calc
	@cp -u system.inc ldr
	@cp -u system.inc filesys
	@cp -u system.inc mcr
	@cp -u system.inc icp
	@cp -u system.inc rmd
	@cp -u system.inc prvutl
	@for i in ${utildirs}; do \
		echo Making all in $$i ; \
		(cd $$i; ${MAKE} all) ; \
	done

# Compile the program-development applications
progdev: libs system
	@cp -u libs/syslib/syslib.lib progdev/zsm
	@cp -u libs/fcslib/fcslib.lib progdev/zsm
	@cp -u libs/syslib/syslib.lib progdev/tkb
	@cp -u libs/syslib/syslib.irl progdev/tkb
	@cp -u libs/syslib/syslib.lib progdev/lbr
	@cp -u libs/fcslib/fcslib.lib progdev/lbr
	@cp -u system.inc progdev/debug
	@cp -u libs/syslib/syslib.lib progdev/debug
	@cp -u libs/syslib/syslib.lib progdev/t3xz
	@(cd progdev; ${MAKE} all)

# Compile Kermit
kermit: libs system
	@cp -u libs/syslib/syslib.lib kermit
	@(cd kermit; ${MAKE} all)

# Compile a few test programs
test: libs system
	@cp -u libs/syslib/syslib.lib test
	@cp -u libs/fcslib/fcslib.lib test
	@cp -u libs/bcdflt/bcdflt.lib test
	@cp -u libs/odt/odt.lib test
	@cp -u system.inc test
	@(cd test; ${MAKE} all)

# Compile a few simple games
games: libs system
	@cp -u libs/syslib/syslib.lib games
	@cp -u libs/fcslib/fcslib.lib games
	@(cd games; ${MAKE} all)

# Do a clean in all directories, except in ./Tools/linux
clean:
	@(cd libs/syslib; ${MAKE} clean)
	@(cd libs/fcslib; ${MAKE} clean)
	@(cd libs/bcdflt; ${MAKE} clean)
	@(cd libs/odt; ${MAKE} clean)
	@for i in ${sysdirs}; do \
		echo Cleaning in $$i ; \
		(cd $$i; ${MAKE} clean) ; \
	done
	@for i in ${utildirs}; do \
		echo Cleaning in $$i ; \
		(cd $$i; ${MAKE} clean) ; \
	done
	@(cd progdev; ${MAKE} clean)
	@(cd kermit; ${MAKE} clean)
	@(cd games; ${MAKE} clean)
	@(cd test; ${MAKE} clean)
	rm -f *~ *.bak *.rel *.lib *.sub *.sym *.sys *.tsk *.map

# Create a new floppy disk image with boot sector, system image,
# system directory, help directory, and a user directory with a
# few example files.
disk-image:
	@echo "new" ${image} ${size} ${files} > mkimg.cmd
	@echo "mount " ${image} >> mkimg.cmd
	@echo "mkdir system 1,1" >> mkimg.cmd
	@echo "mkdir help 1,2" >> mkimg.cmd
	@echo "mkdir syslog 1,5" >> mkimg.cmd
	@echo "mkdir basic 20,1" >> mkimg.cmd
	@echo "mkdir user 20,2" >> mkimg.cmd
	@echo "delete system.sys" >> mkimg.cmd
	@echo "import ./system.sys system.sys /c:512" >> mkimg.cmd
	@echo "updboot boot/${platform}/${bootloader}" >> mkimg.cmd
	@echo "cd system" >> mkimg.cmd
	@echo "import ./acnt.dat acnt.dat" >> mkimg.cmd
	@echo "import ./startup.cmd.${platform} startup.cmd" >> mkimg.cmd
	@echo "import ./login.txt login.txt" >> mkimg.cmd
	@echo "import ./nologin.txt nologin.txt" >> mkimg.cmd
	@echo ";import ./syslogin.cmd syslogin.cmd" >> mkimg.cmd
	@echo "import ./extras.cmd extras.cmd" >> mkimg.cmd
	@echo "import ./syscopy.cmd syscopy.cmd" >> mkimg.cmd
	@echo "cd user" >> mkimg.cmd
	@echo "import ./welcome.txt welcome.txt" >> mkimg.cmd
	@echo "import ./login.cmd login.cmd" >> mkimg.cmd
	@echo "import ./hello.mac hello.mac" >> mkimg.cmd
	@echo "import ./hello.cmd hello.cmd" >> mkimg.cmd
	@echo "import ./hello1.mac hello1.mac" >> mkimg.cmd
	@echo "import ./progdev/t3xz/test/hello.t3x hello.t3x" >> mkimg.cmd
	@echo "import ./progdev/t3xz/test/tbuild.t3x tbuild.t3x" >> mkimg.cmd
	@echo "import ./type.cmd type.cmd" >> mkimg.cmd
	@echo "import ./mce/mceini.cmd mceini.cmd" >> mkimg.cmd
	@echo "mkdir games 20,3" >> mkimg.cmd
	@echo "mkdir cpm 20,4" >> mkimg.cmd
	@echo "mkdir test 20,5" >> mkimg.cmd
	@echo "cd master" >> mkimg.cmd
	@echo "dir master" >> mkimg.cmd
	@echo "dir system" >> mkimg.cmd
	@echo "dir help" >> mkimg.cmd
	@echo "dir user" >> mkimg.cmd
	@echo "dir games" >> mkimg.cmd
	@echo "dir cpm" >> mkimg.cmd
	@echo "dir test" >> mkimg.cmd
	@echo "quit" >> mkimg.cmd
	@if [ -f ${image} ]; then \
		mv -b ${image} ${image}.old ; \
	fi
	$(VOL180) < mkimg.cmd
	@rm mkimg.cmd

# Update a disk image with the system binary, MCR and system libraries.
# The system image normally resides in the [MASTER] directory. Utilitiles,
# libraries and system include files are placed in the [SYSTEM] directory.
copy-system: system cli
	@echo "delete system.sys" > copy.cmd
	@echo "import ./system.sys system.sys /c:512" >> copy.cmd
	@echo "updboot" >> copy.cmd
	@echo "dir" >> copy.cmd
	@echo "cd system" >> copy.cmd
	@for i in inc/*.inc; do \
		echo "delete "`basename $$i` >> copy.cmd ; \
		echo "import "$$i" "`basename $$i` >> copy.cmd ; \
	done
	@echo "delete ldr.tsk" >> copy.cmd
	@echo "import ldr/ldr.tsk ldr.tsk /c" >> copy.cmd
	@echo "delete sysfcp.tsk" >> copy.cmd
	@echo "import filesys/sysfcp.tsk sysfcp.tsk /c" >> copy.cmd
	@echo "delete system.inc" >> copy.cmd
	@echo "import system.inc system.inc" >> copy.cmd
	@echo "delete rsx280.sys" >> copy.cmd
	@echo "import system.sys rsx280.sys" >> copy.cmd
	@echo "delete rsx280.sym" >> copy.cmd
	@echo "import system.sym rsx280.sym" >> copy.cmd
	@echo "delete startup.cmd" >> copy.cmd
	@echo "import startup.cmd.${platform} startup.cmd" >> copy.cmd
	@echo "delete sysvmr.cmd" >> copy.cmd
	@echo "import sysvmr.cmd.${platform} sysvmr.cmd" >> copy.cmd
	@for i in mcr/*.tsk; do \
		echo "delete "`basename $$i` >> copy.cmd ; \
		echo "import "$$i" "`basename $$i`" /c" >> copy.cmd ; \
	done
	@echo "delete syslib.lib" >> copy.cmd
	@echo "import libs/syslib/syslib.lib syslib.lib" >> copy.cmd
	@echo "delete fcslib.lib" >> copy.cmd
	@echo "import libs/fcslib/fcslib.lib fcslib.lib" >> copy.cmd
	@echo "delete bcdflt.lib" >> copy.cmd
	@echo "import libs/bcdflt/bcdflt.lib bcdflt.lib" >> copy.cmd
	@echo "delete odt.lib" >> copy.cmd
	@echo "import libs/odt/odt.lib odt.lib" >> copy.cmd
	@echo "dir" >> copy.cmd
	@echo "quit" >> copy.cmd
	$(VOL180) $(image) < copy.cmd
	@rm copy.cmd

# Update a disk image with the privileged and non-privileged utilities.
# All utilities are placed in the [SYSTEM] directory.
copy-utils: cli utils
	@echo "cd system" > copy.cmd
	@for i in mcr/*.tsk; do \
		echo "delete "`basename $$i` >> copy.cmd ; \
		echo "import "$$i" "`basename $$i`" /c" >> copy.cmd ; \
	done
	@for i in utils/*.tsk; do \
		echo "delete "`basename $$i` >> copy.cmd ; \
		echo "import "$$i" "`basename $$i`" /c" >> copy.cmd ; \
	done
	@for i in prvutl/*.tsk; do \
		echo "delete "`basename $$i` >> copy.cmd ; \
		echo "import "$$i" "`basename $$i`" /c" >> copy.cmd ; \
	done
	@echo "delete rmd.tsk" >> copy.cmd
	@echo "import rmd/rmd.tsk rmd.tsk /c" >> copy.cmd
	@echo "delete vmr.tsk" >> copy.cmd
	@echo "import vmr/vmr.tsk vmr.tsk /c" >> copy.cmd
	@echo "delete pip.tsk" >> copy.cmd
	@echo "import pip/pip.tsk pip.tsk /c" >> copy.cmd
	@echo "delete icp.tsk" >> copy.cmd
	@echo "import icp/icp.tsk icp.tsk /c" >> copy.cmd
	@echo "delete mce.tsk" >> copy.cmd
	@echo "import mce/mce.tsk mce.tsk /c" >> copy.cmd
	@echo "delete vdo.tsk" >> copy.cmd
	@echo "import vdo/vdo.tsk vdo.tsk /c" >> copy.cmd
	@echo "delete ted.tsk" >> copy.cmd
	@echo "import ted/ted.tsk ted.tsk /c" >> copy.cmd
	@echo "delete zap.tsk" >> copy.cmd
	@echo "import zap/zap.tsk zap.tsk /c" >> copy.cmd
	@echo "delete calc.tsk" >> copy.cmd
	@echo "import calc/calc.tsk calc.tsk /c" >> copy.cmd
	@echo "delete cpm.tsk" >> copy.cmd
	@if [ "${size}" -ge "3000" ]; then \
		echo "import cpm/cpm.tsk cpm.tsk /c" >> copy.cmd ; \
	fi
	@echo "dir" >> copy.cmd
	@if [ "${size}" -ge "3000" ]; then \
		echo "cd cpm" >> copy.cmd ; \
		echo "delete map.com" >> copy.cmd ; \
		echo "import cpm/map.com map.com" >> copy.cmd ; \
		echo "dir" >> copy.cmd ; \
	fi
	@echo "quit" >> copy.cmd
	$(VOL180) $(image) < copy.cmd
	@rm copy.cmd

# Copy the help files to the [HELP] directory of the disk image.
copy-help:
	@echo "cd help" > copy.cmd
	@for i in help/*.hlp help/*.doc; do \
		echo "delete "`basename $$i` >> copy.cmd ; \
		echo "import "$$i" "`basename $$i` >> copy.cmd ; \
	done
	@echo "dir" >> copy.cmd
	@echo "quit" >> copy.cmd
	$(VOL180) $(image) < copy.cmd
	@rm copy.cmd

# Copy the program-development utilities to the disk image
copy-progdev: progdev
	@echo "cd system" > copy.cmd
	@for i in progdev/*/*.tsk; do \
		echo "delete "`basename $$i` >> copy.cmd ; \
		echo "import "$$i" "`basename $$i`" /c" >> copy.cmd ; \
	done
	@echo "delete t3xz.lib" >> copy.cmd
	@echo "import progdev/t3xz/t3xz.lib t3xz.lib" >> copy.cmd
	@echo "dir" >> copy.cmd
	@echo "quit" >> copy.cmd
	$(VOL180) $(image) < copy.cmd
	@rm copy.cmd

# Copy the BASIC-11 example programs to the [BASIC] directory of the disk
# image. For small images (e.g. floppies), only a reduced set of programs
# is copied.
reduced-set = acey.bas blackjack.bas buzzwd.bas civilwar.bas cycles.bas \
	hamurs.bas hangman.bas lunar.bas mandel.bas maze.bas ship.bas \
	trader.bas trek100.bas weekday.bas

copy-basic: progdev
	@echo "cd basic" > copy.cmd
	@if [ "${size}" -ge "3000" ]; then \
		for i in progdev/basic11/programs/*.bas ; do \
			echo "delete "`basename $$i` >> copy.cmd ; \
			echo "import "$$i" "`basename $$i` >> copy.cmd ; \
		done ; \
		for i in progdev/basic11/programs/*.rlc ; do \
			echo "delete "`basename $$i` >> copy.cmd ; \
			echo "import "$$i" "`basename $$i` >> copy.cmd ; \
		done ; \
	else \
		for i in ${reduced-set} ; do \
			echo "delete "`basename $$i` >> copy.cmd ; \
			echo "import progdev/basic11/programs/"$$i" "`basename $$i` >> copy.cmd ; \
		done ; \
	fi
	@echo "dir" >> copy.cmd
	@echo "quit" >> copy.cmd
	$(VOL180) $(image) < copy.cmd
	@rm copy.cmd

# Copy some CP/M files for the CP/M emulator, the files are placed
# in the [CPM] directory.
copy-cpm:
	@if [ "${size}" -ge "3000" ]; then \
		echo "cd cpm" > copy.cmd ; \
		for i in cpm/test/* ; do \
			echo "delete "`basename $$i` >> copy.cmd ; \
			echo "import "$$i" "`basename $$i` >> copy.cmd ; \
		done ; \
		echo "dir" >> copy.cmd ; \
		echo "quit" >> copy.cmd ; \
		$(VOL180) $(image) < copy.cmd ; \
		rm copy.cmd ; \
	fi

# Copy some test files to the disk image.
# The files are placed in a separate [TEST] directory.
copy-test: test
	@echo "cd test" > copy.cmd
	@for i in test/*.tsk; do \
		echo "delete "`basename $$i` >> copy.cmd ; \
		echo "import "$$i" "`basename $$i`" /c" >> copy.cmd ; \
	done
	@for i in test/extras/*.*; do \
		echo "delete "`basename $$i` >> copy.cmd ; \
		if [[ `basename $$i | cut -d . -s -f 2-` == 'tsk' ]]; then \
			echo "import "$$i" "`basename $$i`" /c" >> copy.cmd ; \
		else \
			echo "import "$$i" "`basename $$i` >> copy.cmd ; \
		fi ; \
	done
	@echo "delete maze.t3x" >> copy.cmd
	@echo "import progdev/t3xz/test/maze.t3x maze.t3x" >> copy.cmd
	@echo "delete rsxio.mac" >> copy.cmd
	@echo "import progdev/t3xz/test/rsxio.mac rsxio.mac" >> copy.cmd
	@echo "delete mkmaze.cmd" >> copy.cmd
	@echo "import progdev/t3xz/test/mkmaze.cmd mkmaze.cmd" >> copy.cmd
	@echo "delete test.cal" >> copy.cmd
	@echo "import calc/test/test.cal test.cal" >> copy.cmd
	@echo "delete iftest.cal" >> copy.cmd
	@echo "import calc/test/iftest.cal iftest.cal" >> copy.cmd
	@echo "dir" >> copy.cmd
	@echo "exit" >> copy.cmd
	$(VOL180) $(image) < copy.cmd
	@rm copy.cmd

# Copy a few simple games to the disk image.
# The files are placed in a separate [GAMES] directory.
copy-games: games
	@if [ "${size}" -ge "3000" ]; then \
		echo "cd games" > copy.cmd ; \
		for i in games/*.tsk; do \
			echo "delete "`basename $$i` >> copy.cmd ; \
			echo "import "$$i" "`basename $$i`" /c" >> copy.cmd ; \
		done ; \
		echo "dir" >> copy.cmd ; \
		echo "quit" >> copy.cmd ; \
		$(VOL180) $(image) < copy.cmd ; \
		rm copy.cmd ; \
	fi

# Copy Kermit to the disk image.
copy-kermit: kermit
	@if [ "${size}" -ge "3000" ]; then \
		echo "cd system" > copy.cmd ; \
		echo "delete kermit.tsk" >> copy.cmd ; \
		echo "import kermit/kermit.tsk kermit.tsk /c" >> copy.cmd ; \
		echo "dir" >> copy.cmd ; \
		echo "cd test" >> copy.cmd ; \
		echo "delete kermit.ini" >> copy.cmd ; \
		echo "import kermit/kermit.ini kermit.ini" >> copy.cmd ; \
		echo "dir" >> copy.cmd ; \
		echo "quit" >> copy.cmd ; \
		$(VOL180) $(image) < copy.cmd ; \
		rm copy.cmd ; \
	fi

# Copy everything to the disk image.
copy-all: copy-system copy-utils copy-help \
          copy-progdev copy-basic copy-test \
          copy-kermit copy-games copy-cpm

# Configure system
sysvmr-old:
	@echo "cd system" > vmr.cmd
	@echo "vmr @sysvmr" >> vmr.cmd
	@echo "delete [master]system.sys" >> vmr.cmd
	@echo "copy rsx280.sys [master]system.sys /c" >> vmr.cmd
	@echo "updboot" >> vmr.cmd
	@echo "bye" >> vmr.cmd
	$(VOL180) $(image) < vmr.cmd
	@rm vmr.cmd

sysvmr:
	@echo "cd system" > vmr.cmd
	@echo "delete [master]system.sys" >> vmr.cmd
	@echo "copy rsx280.sys [master]system.sys /c:512" >> vmr.cmd
	@echo "copy rsx280.sym [master]system.sym" >> vmr.cmd
	@echo "vmr @sysvmr" >> vmr.cmd
	@echo "delete [master]system.sym" >> vmr.cmd
	@echo "updboot" >> vmr.cmd
	@echo "bye" >> vmr.cmd
	$(VOL180) $(image) < vmr.cmd
	@rm vmr.cmd

# Copy disk image to physical device
dev-copy:
	@if [ "${backup}" != "no" ]; then \
		if [ -f device.backup ]; then \
			mv -f device.backup device.backup.old ; \
		fi ; \
		echo dd if=$(outdev) of=device.backup bs=512 skip=$(offset) count=$(size) ; \
		dd if=$(outdev) of=device.backup bs=512 skip=$(offset) count=$(size) ; \
	fi
	@if [ "${conv}" = "swab" ]; then \
		echo dd if=$(image) of=$(outdev) conv=swab bs=512 seek=$(offset) ; \
		dd if=$(image) of=$(outdev) conv=swab bs=512 seek=$(offset) ; \
	else \
		echo dd if=$(image) of=$(outdev) bs=512 seek=$(offset) ; \
		dd if=$(image) of=$(outdev) bs=512 seek=$(offset) ; \
	fi
