# Path to Linux utilities
ZXCC = zxcc

# Path to CP/M utilities
ZSM4 = ../Tools/cpm/zsm4.com
TKB  = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = bro.mac \
	ccl.mac \
	cpu.mac \
	dcu.mac \
	shf.mac \
	uptime.mac \
	who.mac

OBJS = $(SRCS:.mac=.rel)

PROGS = $(OBJS:.rel=.tsk)

all: $(PROGS)

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

#$(PROGS): %.tsk: %.rel syslib.lib
#	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb"

bro.tsk: bro.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb/task=...BRO/priv"

ccl.tsk: ccl.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb/task=...CCL/pri=90/priv"

# CPU is not really privileged, but it accesses directly the common area
cpu.tsk: cpu.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb/task=...CPU"

dcu.tsk: dcu.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb/task=...DCU/priv"

shf.tsk: shf.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb/task=SHF.../pri=105/priv"

uptime.tsk: uptime.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb/priv/task=...UPT"

who.tsk: who.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb/pri=100/priv/task=...WHO"

clean:
	rm -f *.rel *.prn *.sym *.map core *.tsk *~ *.\$$\$$\$$
