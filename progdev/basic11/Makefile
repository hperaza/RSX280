# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../../Tools/cpm/zsm4.com
TKB    = ../../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = basmain.mac \
	basinit.mac \
	bascli.mac \
	basic1.mac \
	basic2.mac \
	basic3.mac \
	basic4.mac \
	basic5.mac \
	baserr.mac \
	basdat.mac \
	basfp.mac \
	basrsx.mac \
	bascpm.mac

OBJS = $(SRCS:.mac=.rel)

PROGRSX = basic.tsk
PROGCPM = basic.com

all: $(PROGRSX) $(PROGCPM)

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l/s7

$(PROGRSX): $(OBJS)
	@echo -e $@",basicrsx.sym,basicrsx.map=basmain/ofmt:tsk,&\r" > build.cmd
	@echo -e "basfp,bascli,basic1,basic2,basic3,basic4,basic5,&\r" >> build.cmd
	@echo -e "baserr,basrsx,basinit,basdat/task=...BAS/asg=TI:1,SY:2-13\r" >> build.cmd
	$(ZXCC) $(TKB) -"@build.cmd"
	@rm build.cmd

$(PROGCPM): $(OBJS)
	@echo -e $@",basiccpm.sym=basmain/ofmt:com,&\r" > build.cmd
	@echo -e "basfp,bascli,basic1,basic2,basic3,basic4,basic5,&\r" >> build.cmd
	@echo -e "baserr,bascpm,basinit,basdat\r" >> build.cmd
	$(ZXCC) $(TKB) -"@build.cmd"
	@rm build.cmd

copy: $(PROGRSX)
	@echo "cd system" > cp.cmd
	@echo "delete basic.tsk" >> cp.cmd
	@echo "import basic.tsk basic.tsk /c" >> cp.cmd
	@echo "dir" >> cp.cmd
	@echo "quit" >> cp.cmd
	$(VOL180) /dev/fd0 < cp.cmd
	@rm cp.cmd

clean:
	rm -f *.rel *.prn *.sym *.map core *.bin $(PROGRSX) $(PROGCPM) *~ *.\$$\$$\$$
