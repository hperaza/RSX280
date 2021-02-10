# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../Tools/cpm/zsm4.com
TKB    = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = main.mac \
	dir.mac \
	copy.mac \
	attr.mac \
	delete.mac \
	unlock.mac \
	rename.mac \
	search.mac \
	match.mac

OBJS = $(SRCS:.mac=.rel)

PROG = pip.tsk

all: $(PROG)

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

$(PROG): $(OBJS) fcslib.lib syslib.lib
	$(ZXCC) $(TKB) -"$@=main/ofmt=tsk,dir,copy,attr,delete,unlock,rename,search,match,fcslib/lb,syslib/lb/task=...PIP/asg=SY:1-4:6-7,TI:5"

clean:
	rm -f $(PROG) *.rel *.prn *.sym *.map core *.bin *.tsk *~ *.\$$\$$\$$

copy: pip.tsk
	@echo "cd system" > cppip.cmd
	@echo "delete pip.tsk" >> cppip.cmd
	@echo "import pip.tsk pip.tsk /c" >> cppip.cmd
	@echo "bye" >> cppip.cmd
	$(VOL180) /dev/fd0 < cppip.cmd
	@rm cppip.cmd
