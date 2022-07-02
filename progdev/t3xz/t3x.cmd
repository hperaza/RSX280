.; Pre-requisites:
.; * t3x.tsk	bootstrap compiler
.; * mklib.t3x	utility sources
.; * lib.mac	run-time library sources
.; * t.src	new compiler sources
.enable substitution
.ifnins ...t3x ins t3x/task=...t3x/inc=25000
.ifnins ...mac ins $mac
.ifnins ...tkb ins $tkb/inc=30000
; Compile the run-time library
mac =lib/i$
.if <exstat> ne <succes> .stop
tkb lib.bin/of:com=lib
.if <exstat> ne <succes> .stop
; Compile mklib
t3x mklib
.if <exstat> ne <succes> .stop
pip mklib.tsk/co/nv=mklib.tsk
pip mklib.tsk/pu
; Merge run-time object and compiler sources
run mklib
.if <exstat> ne <succes> .stop
; Compile the compiler
t3x t /v
.if <exstat> ne <succes> .stop
pip t.tsk/co/nv=t.tsk
pip t.tsk/pu
; Try compiling and running a simple application
run t/inc=25000/cmd=fib
.if <exstat> ne <succes> .stop
pip fib.tsk/co/nv=fib.tsk
pip fib.tsk/pu
run fib
; Success!
