# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../Tools/cpm/zsm4.com
TKB    = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = vdo.mac \
	vdocpm.mac \
	vdorsx.mac

OBJS = $(SRCS:.mac=.rel)

all: vdo.com vdo.tsk

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

vdo.com: $(OBJS)
	$(ZXCC) $(TKB) -"$@,vdocpm=vdo/of=com,vdocpm"

vdo.tsk: $(OBJS) syslib.lib
	$(ZXCC) $(TKB) -"$@,vdorsx=vdo/of=tsk,vdorsx,syslib/lb/task=...VDO/ext=30000/pri=65"

cpycpm: vdo.com
	cpmrm -f p112 /dev/fd0 0:vdo.com
	cpmcp -f p112 /dev/fd0 vdo.com 0:

cpyrsx: vdo.tsk
	@echo "cd system" > copy.cmd
	@echo "delete vdo.tsk" >> copy.cmd
	@echo "import vdo.tsk vdo.tsk /c" >> copy.cmd
	@echo "dir" >> copy.cmd
	@echo "bye" >> copy.cmd
	$(VOL180) /dev/fd0 < copy.cmd
	@rm copy.cmd

clean:
	rm -f vdo.com vdo.tsk *.rel *.sym *.prn *.crf *.map core *~ *.\$$\$$\$$
