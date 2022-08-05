# T3X/Z compiler for RSX180 and RSX280 - REL version

This a further development of the port of Nils M Holm's [T3X/Z](http://t3x.org/t3x/index.html) compiler to [RSX180](http://p112.sourceforge.net/index.php?rsx180) and [RSX280](https://github.com/hperaza/RSX280).

This version outputs object files in Microsoft REL format, which are then linked using a standard linker to the run-time library (and possibly to user-written assembly language rotines too) to produce the final task image file. The same REL file can be linked to the CP/M version of the run-time library to produce a CP/M executable without having to recompile the sources again with the CP/M version of the compiler.

## Building the compiler

The compiler can be built under CP/M or RSX180/280, or cross-compiled under Linux or Windows:

### Building on CP/M

Required files:

* The compiler source file.
* The run-time library sources.
* The correct system include files for the target system (the ones in this repository are for RSX280).
* The [ZSM4](https://github.com/hperaza/ZSM4) macro-assembler.
* A librarian program for REL files (Microsoft LIB80, Digital Research's LIB or the CP/M version of RSX180/280 LBR)
* Digital Research's LINK or the CP/M version of RSX180/280 linker TKB.
* The runnable CP/M version of the compiler (there is a copy in the [bin](bin/) directory.)

Steps:

* Compile the run-time library modules (note the /s8 option to generate 8-character symbol names):
  
  ```
  zsm4 =comp/s8
  zsm4 =div/s8
  zsm4 =mult/s8
  zsm4 =strip/s8
  zsm4 =start/s8
  zsm4 =t/s8
  ```

* Combine the run-time modules into a library:
  
  ```
  lbr t3xz.lib=t/e+start/e+comp/e+div/e+mult/e+strip/e
  ```

* Build the native RSX180/280 compiler. For this step you can use the original CP/M compiler that creates a COM file directly, and then use the resulting compiler to compile the compiler to a REL module:
  
  ```
  t3xz16 t
  pip t.t3x=t.t
  t t
  ```

  Alternalively, you can use the pre-compiled version that produces REL files: 
  
  ```
  pip t.t3x=t.t
  t3x t
  ```
  
* Link the resulting module to produce the native RSX180/280 compiler:
  
  ```
  tkb t=t.obj,t3xz/lb/of:t/task=...t3x/ext=25000/asg=ti:1,sy:2-6
  ```

* Optionally, you can build a CP/M version of the compiler:

  ```
  tkb t=t.obj,t3xzcpm/lb/of:c
  ```

  Or, using DR's link:
  
  ```
  link t.obj,t3xzcpm.lib[s]
  ```

  
### Building on RSX180/280

Required files:

* The compiler sources, without the runtime library.
* The runtime library sources.
* The correct system include files for the target system (the ones in this repository are for RSX280).
* The [ZSM4](https://github.com/hperaza/ZSM4) macro-assembler, normally included in the RSX180/280 distribution and installed as `...MAC`.
* The LBR librarian, also included in the RSX180/280 distribution and installed as `...LBR`.
* The TKB linker, included in the RSX180/280 distribution and installed as `...TKB`.
* The runnable RSX180 or RSX280 version of the compiler (e.g. the one generated under CP/M as described in the previous section, or the copy provided in the [bin](bin/) directory.)

Steps:

* Make sure MAC, LBR and TKB are installed.

* Install the bootstrap compiler:
  
  ```
  ins t3x/task=...t3x/inc=25000
  ```

* Build the run-time library modules:
  
  ```
  mac =comp/i$/s8
  mac =div/i$/s8
  mac =mult/i$/s8
  mac =strip/i$/s8
  mac =start/i$/s8
  mac =t/i$/s8
  ```

* Combine the run-time modules into a library:
  
  ```
  lbr t3xz.lib=t/e+start/e+comp/e+div/e+mult/e+strip/e
  ```

* Build the compiler:
  
  ```
  t3x t /v
  tkb t=t,t3xz/lb/of:t/task=...t3x/ext=25000/asg=ti:1,sy:2-6
  ```

You can try now compiling and running some example programs, e.g.:

```
run t/inc=25000/cmd=fib
pip fib.tsk/co/nv=fib.tsk
run fib
```

Note that we used the `run` command to run the new compiler directly. Alternatively, you can use the `rem` command to uninstall the old compiler then `ins` to install the new one as explained above.

The provided `t3x.cmd` command file automates all the above steps.

The additionally supplied `tbuild.cmd` command file can be used to build T3X applications.

### Cross-building on Linux or Windows:

Required files:

* The compiler sources, without the runtime library.
* The runtime library sources.
* The correct system include files for the target system (the ones in this repository are for RSX280).
* The [ZSM4](https://github.com/hperaza/ZSM4) macro-assembler.
* The CP/M version of RSX180/280 librarian LBR.
* The CP/M version of RSX180/280 linker TKB.
* The runnable CP/M version of the compiler (there is a copy in the [bin](bin/) directory.)
* A CP/M emulator such as John Elliot's [ZXCC](https://www.seasip.info/Unix/Zxcc/)

Steps:

* Build the run-time library:
  
  ```
  zxcc zsm4 =comp/s8
  zxcc zsm4 =div/s8
  zxcc zsm4 =mult/s8
  zxcc zsm4 =strip/s8
  zxcc zsm4 =start/s8
  zxcc zsm4 =t/s8
  ```

* Combine the run-time modules into a library:
  
  ```
  zxcc lbr t3xz.lib=t/e+start/e+comp/e+div/e+mult/e+strip/e
  ```

* Build the native RSX180/280 compiler. For this step you can use the original CP/M compiler that creates a COM file directly, and then use the resulting compiler to compile the compiler to a REL module:
  
  ```
  zxcc t3xz16 t
  cp t.t t.t3x
  zxcc t t
  ```

  Alternalively, you can use the pre-compiled version that produces REL files: 
  
  ```
  cp t.t t.t3x
  zxcc t3x t
  ```
  
* Link the resulting module to produce the native RSX180/280 compiler:
  
  ```
  zxcc tkb t=t.obj,t3xz/lb/of:t/task=...t3x/ext=25000/asg=ti:1,sy:2-6
  ```

* Optionally, you can build a CP/M version of the compiler:

  ```
  zxcc tkb t=t.obj,t3xzcpm/lb/of:c
  ```

  Or, using DR's link:
  
  ```
  zxcc drlink t.obj,t3xzcpm.lib[s]
  ```

The above steps produce both a `t.com` CP/M cross-compiler and a `t.tsk` native RSX180/280 compiler.

The supplied `Makefile` can be used to build the compiler on Linux.

## Usage

Command line syntax is the same as for the CP/M version, for example:

```
t3x source [/v]
```

The above command assumes that the compiler is installed as `...T3X`. Note that the optional `/v` (verbose) option is separated from the file name by a space.

## Differences with the CP/M version

* The argument to the HALT statement is used as the program exit code, which RSX180/280 then passes to the parent task.
* The default file extension is `.t3x` and not `.t`.
* By default, no summary line is output to the terminal.
* The starndard `t` object is no longer created by default, applications must use the `object` statement to instantiate it from the `t3x` class.
* A new `external` keyword was introduced to allow linking to external user-written assembly routines; the syntax is similar to that of the `decl` statement. Parameters are passed to the assembly routine on the stack, as with the native T3X routines.

## Limitations

The original [README](docs/orig/README) file mentions the limitations of the T3X/Z compiler. In addition, the current RSX180/280 port has the following ones:

* The `t.bdos` and `t.bdoshl` functions are not implemented, as they are CP/M-specific. They can be added if desired (they are present in the CP/M version of the run-time library) or called using the `external` statement.

## TODO

* Implement a `SYS` class for system-call access.
* Modify the `t.open` function to support additional arguments for file attributes, permissions, etc.
* Make the command line syntax consistent with RSX conventions, e.g. `output=input/options`

