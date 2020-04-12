# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../Tools/cpm/zsm4.com
TKB    = ../Tools/cpm/tkb.com

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .mac .rel

SRCS = main.mac \
	termdef.mac \
	cpmio.mac \
	rsxio.mac

OBJS = $(SRCS:.mac=.rel)

all:	zap.com zap.tsk

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

zap.com: $(OBJS)
	$(ZXCC) $(TKB) -"zap=termdef,main,cpmio/ofmt=com"

zap.tsk: $(OBJS) syslib.lib
	$(ZXCC) $(TKB) -"zap=termdef,main,rsxio,syslib/lb/ofmt=tsk/task=...ZAP"

copy: cprsx

cpcpm: zap.com
	cpmrm -f p112 /dev/fd0 0:zap.com
	cpmcp -f p112 /dev/fd0 zap.com 0:

cprsx: zap.tsk
	@echo "cd system" > cpzap.cmd
	@echo "delete zap.tsk" >> cpzap.cmd
	@echo "import zap.tsk zap.tsk /c" >> cpzap.cmd
	@echo "bye" >> cpzap.cmd
	$(VOL180) /dev/fd0 < cpzap.cmd
	@rm cpzap.cmd

clean:
	rm -f $(OBJS) zap.com zap.tsk *.sym *.prn *.map core *~ *.\$$\$$\$$
