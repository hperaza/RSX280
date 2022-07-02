# T3X/Z compiler for RSX180 and RSX280

This a port of Nils M Holm's [T3X/Z](http://t3x.org/t3x/index.html) compiler to the [RSX180](http://p112.sourceforge.net/index.php?rsx180) and [RSX280](https://github.com/hperaza/RSX280) OSes.

T3X is a small, portable, procedural, block-structured, recursive, almost typeless, and to some degree object-oriented programming language with a Pascal-like syntax.

The original T3X/Z compiler implements a large subset of the T3X language and was written for Z80-based computers running CP/M.

This RSX180/280 port required mainly a rewrite of the run-time library; the compiler code itself required little modification.

Like the original T3X/Z compiler, the RSX180/280 port creates executable files directly with no linking step involved. While that choice is adequate for a small compiler like this, it nevertheless reduces flexibility by not being able to link modules compiled separately, or to machine-code routines, user or system libraries, etc.

## Building the compiler

The compiler can be built under CP/M or RSX180/280, or cross-compiled under Linux or Windows:

### Building on CP/M

Required files:

* The compiler sources, without the run-time library.
* The run-time library sources.
* The correct system include files for the target system (the ones in this repository are for RSX280).
* The [ZSM4](https://github.com/hperaza/ZSM4) macro-assembler.
* Digital Research's LINK or the CP/M version of RSX180/280 linker TKB.
* The runnable CP/M version of the compiler (there is a copy in the [bin](bin/) directory.)

Steps:

* Build the run-time library:
  
  `zsm4 =lib`
  `link lib.bin=lib`

* Integrate the run-time library into the compiler sources:
  
  `mklib`

* Build the CP/M version of the RSX180/280 compiler:
  
  `t3xz16 t`

* Build the native RSX180/280 compiler:
  
  `pip t.t3x=t.t`
  `t t`

The above steps produce a `t.com` CP/M cross-compiler that can be used to generate RSX180/280 task image files under CP/M, and a `t.tsk` native RSX180/280 compiler.

### Building on RSX180/280

Required files:

* The compiler sources, without the runtime library.
* The runtime library sources.
* The correct system include files for the target system (the ones in this repository are for RSX280).
* The [ZSM4](https://github.com/hperaza/ZSM4) macro-assembler, normally included in the RSX180/280 distribution and installed as `...MAC`.
* The TKB linker, also included in the RSX180/280 distribution and installed as `...TKB`.
* The runnable RSX180 or RSX280 version of the compiler (e.g. the one generated under CP/M as described in the previous section, or the copy provided in the [bin](bin/) directory.)

Steps:

* Make sure MAC and TKB are installed.

* Install the bootstrap compiler:
  
  `ins t3x/task=...t3x/inc=25000`

* Build the run-time library:
  
  `mac =lib/i$`
  `tkb lib.bin/of:com=lib`

* Compile the RSX180/280 `mklib` utility, and use PIP to make the output file contiguous (see the "Limitations" section below):
  
  `t3x mklib`
  `pip mklib.tsk/co/nv=mklib.tsk`

* Run the `mklib` utility to merge the run-time library with the compiler sources:
  
  `run mklib`

* Compile the compiler:
  
  `t3x t /v`
  `pip t.tsk/co/nv=t.tsk`

You can try now compiling and running some example programs, e.g.:

  `run t/inc=25000/cmd=fib`
  `pip fib.tsk/co/nv=fib.tsk`
  `run fib`

Note that we used the `run` command to run the new compiler directly. Alternatively, you can use the `rem` command to uninstall the old compiler then `ins` to install the new one as explained above. Don't forget to add enough stack space with the `/inc` option.

The provided `t3x.cmd` command file automates all the above step.

The additionally supplied `tbuild.cmd` command file can be used to build T3X applications.

### Cross-building on Linux or Windows:

Required files:

* The compiler sources, without the runtime library.
* The runtime library sources.
* The correct system include files for the target system (the ones in this repository are for RSX280).
* The [ZSM4](https://github.com/hperaza/ZSM4) macro-assembler.
* Digital Research's LINK or the CP/M version of RSX180/280 linker TKB.
* The runnable CP/M version of the compiler (there is a copy in the [bin](bin/) directory.)
* A CP/M emulator such as John Elliot's [ZXCC](https://www.seasip.info/Unix/Zxcc/)

Steps:

* Build the run-time library:
  
  `zxcc zsm4 =lib`
  `zxcc drlink lib.bin=lib`

* Integrate the run-time library into the compiler sources:
  
  `zxcc ./bin/mklib`

* Build the CP/M version of the RSX180/280 compiler:
  
  `zxcc ./bin/t3xz16 t`

* Build the native RSX180/280 compiler:
  
  `cp t.t t.t3x`
  `zxcc t t`

The above steps produce both a `t.com` CP/M cross-compiler and a `t.tsk` native RSX180/280 compiler.

The supplied `Makefile` can be used to build the compiler on Linux.

## Usage

Command line syntax is the same as the CP/M version, for example:

 `t3x source [/v]`

The above command assumes that the compiler is installed as `...T3X`. Note that the single `/v` (verbose) option is separated by a space.

## Differences with the CP/M version

* The argument to the HALT statement is used as the program exit code, which RSX180/280 then passes to the parent task.
* The default file extension is `.t3x` and not `.t`.
* By default, no summary line is output to the terminal.
* The `t.bdos` and `t.bdoshl` functions were replaced by a single `t.system`, but the implementation is still missing.

## Limitations

The original [README](docs/orig/README) file mentions the limitations of the T3X/Z compiler. In addition, the current RSX180/280 port has the following ones:

* The output task image file is created non-contiguous, due to a limitation of the `t.open` library function. This can be fixed with the following PIP command:
  
  `pip output.tsk/co/nv=output.tsk`
  
  where `output.tsk` is the name of the file generated by T3X.

* There is no way to specify task image file attributes (e.g. task name, identification version, additional memory, priority, etc.)

* The `t.system` function is recognized, but not implemented.

## TODO

* Implement the `t.system` call; may be tricky because of the variery of RSX180/280 system calls, register usage, etc. A second option would be to implement separate `t.*` functions for each system call, but that will break compatibility with the standard `T3X` core class. The correct T3X way, according to the [T3X Language Reference](http://t3x.org/t3x/t3x.html), would be to implement a `SYS` class.
* Modify the `t.open` function to support additional arguments for file attributes, permissions, etc.
* More RSX-like command line syntax, e.g. `output=input/options`
* REL output file generation, to be able to link the generated code with external routines; may make the `t.system` call obsolete, and may require the introduction of new keywords such as 'external' and 'public'.
