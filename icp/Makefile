# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../Tools/cpm/zsm4.com
TKB    = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = main.mac \
	commands.mac \
	vars.mac \
	spec.mac \
	eval.mac \
	subst.mac \
	utils.mac

OBJS = $(SRCS:.mac=.rel)

PROG = icp.tsk

all: $(PROG)

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

$(PROG): $(OBJS) fcslib.lib syslib.lib
	$(ZXCC) $(TKB) -"$@=main/of=t,commands,subst,eval,vars,spec,utils,syslib/lb/task=...AT./priv/pri=64/ext=1024"

clean:
	rm -f $(PROG) *.rel *.prn *.map *.sym core *.bin *.tsk *~ *.\$$\$$\$$

copy: $(PROG)
	@echo "cd system" > cpicp.cmd
	@echo "delete icp.tsk" >> cpicp.cmd
	@echo "import icp.tsk icp.tsk /c" >> cpicp.cmd
	@echo "bye" >> cpicp.cmd
	$(VOL180) /dev/fd0 < cpicp.cmd
	@rm cpicp.cmd
