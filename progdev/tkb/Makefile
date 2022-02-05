# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../../Tools/cpm/zsm4.com
DRLINK = ../../Tools/cpm/drlink.com
TKB    = ../../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = link.mac \
	linkcpm.mac \
	linkrsx.mac

OBJS = $(SRCS:.mac=.rel)

all: tkb.com tkb.tsk

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l/s8"

tkb.com: $(OBJS)
	$(ZXCC) $(DRLINK) -"tkb=link,linkcpm"
	cp tkb.sym tkbcpm.sym

tkb.tsk: $(OBJS) syslib.lib
	$(ZXCC) $(TKB) -"tkb,tkbrsx,tkb=link,linkrsx,syslib/lb/ofmt:tsk/task=...TKB/ext=20000/asg=TI:5,SY:1-4:6-14"

test: tkb.com
	@cp tkb.com tk.com
	$(ZXCC) tk -"tkb1=link,linkcpm"
	@rm tk.com
	comp tkb.com tkb1.com > comp_tkb_tkb1
	ls -l comp_tkb_tkb1

clean:
	rm -f tkb.com tkb.tsk *.rel *.sym *.prn *.map *.crf core *~ *.\$$\$$\$$

copy: tkb.tsk
	@echo "cd system" > cptkb.cmd
	@echo "delete tkb.tsk" >> cptkb.cmd
	@echo "import tkb.tsk tkb.tsk /c" >> cptkb.cmd
	@echo "bye" >> cptkb.cmd
	$(VOL180) /dev/fd0 < cptkb.cmd
	@rm cptkb.cmd
