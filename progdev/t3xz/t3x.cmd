.; Pre-requisites:
.;   t3x.tsk	bootstrap compiler
.;   *.mac	run-time library sources
.;   t.t	new compiler sources
.enable substitution
.ifnins ...t3x ins t3x/task=...t3x/inc=25000
.ifnins ...mac ins $mac
.ifnins ...lbr ins $lbr/inc=20000
.ifnins ...tkb ins $tkb/inc=30000
; Compile the run-time library
.open #1 compile.cmd
.enable data #1
=comp/i$/s8
=div/i$/s8
=mult/i$/s8
=strip/i$/s8
=start/i$/s8
=t/i$/s8
.close
mac @compile
.if <exstat> ne <succes> .stop
; Create the run-time library
lbr t3xz.lib=t/e+start/e+comp/e+div/e+mult/e+strip/e
.if <exstat> ne <succes> .stop
pip compile.cmd;*,*.obj;*/nv/de
; Compile the compiler
t3x t /v
.if <exstat> ne <succes> .stop
; Build the compiler
tkb t,t,t=t,t3xz/lb/q/xm/ext=25000/task=...t3x/ident=t3xz16/asg=ti:1,sy:2-6
.if <exstat> ne <succes> .stop
; Try compiling and running a simple application
run t/cmd=fib
.if <exstat> ne <succes> .stop
tkb fib=fib,t3xz/lb/q/ext=2000/task=...fib/ident=t3xz16/asg=ti:1,sy:2-6
.if <exstat> ne <succes> .stop
run fib
; Success!
