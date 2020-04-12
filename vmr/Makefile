# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../Tools/cpm/zsm4.com
TKB    = ../Tools/cpm/tkb.com

.SUFFIXES:
.SUFFIXES: .mac .rel

SRCS = main.mac \
	symbols.mac \
	tasks.mac \
	pars.mac \
	devices.mac \
	pool.mac \
	vmem.mac

OBJS = $(SRCS:.mac=.rel)

all: vmr.tsk

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

vmr.tsk: $(OBJS) syslib.lib
	$(ZXCC) $(TKB) -"$@,vmr,vmr=main/ofmt=tsk,symbols,devices,tasks,pars,pool,vmem,syslib/lb/task=...VMR"

copy: vmr.tsk
	@echo "cd system" > copy.cmd
	@echo "delete vmr.tsk" >> copy.cmd
	@echo "import vmr.tsk vmr.tsk /c" >> copy.cmd
	@echo "dir" >> copy.cmd
	@echo "bye" >> copy.cmd
	$(VOL180) /dev/fd0 < copy.cmd
	@rm copy.cmd

clean:
	rm -f *.com *.rel *.prn *.sym core *.map *.tsk *~ *.\$$\$$\$$
