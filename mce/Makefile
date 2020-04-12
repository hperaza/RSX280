# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../Tools/cpm/zsm4.com
TKB    = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = mce.mac

OBJS = $(SRCS:.mac=.rel)

PROG = mce.tsk

all: $(PROG)

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

$(PROG): $(OBJS) fcslib.lib syslib.lib
	$(ZXCC) $(TKB) -"$@=mce/of=tsk,fcslib/lb,syslib/lb/task=...MCE/ext=1024/pri=60"

clean:
	rm -f $(PROG) *.rel *.prn *.sym *.map core *.bin *.tsk *~ *.\$$\$$\$$

copy: mce.tsk
	@echo "cd system" > cpmce.cmd
	@echo "delete mce.tsk" >> cpmce.cmd
	@echo "import mce.tsk mce.tsk /c" >> cpmce.cmd
	@echo "bye" >> cpmce.cmd
	$(VOL180) /dev/fd0 < cpmce.cmd
	@rm cpmce.cmd

copysrc:
	@echo "cd user" > cpmce.cmd
	@echo "delete mceini.cmd" >> cpmce.cmd
	@echo "import mceini.cmd mceini.cmd" >> cpmce.cmd
	@echo "delete mcebld.cmd" >> cpmce.cmd
	@echo "import mcebld.cmd mcebld.cmd" >> cpmce.cmd
	@echo "delete mce.mac" >> cpmce.cmd
	@echo "import mce.mac mce.mac" >> cpmce.cmd
	@echo "bye" >> cpmce.cmd
	$(VOL180) /dev/fd0 < cpmce.cmd
	@rm cpmce.cmd
