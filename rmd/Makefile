# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../Tools/cpm/zsm4.com
TKB    = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = main.mac \
	mem.mac \
	atd.mac \
	tskhdr.mac \
	command.mac \
	misc.mac \
	help.mac \
	date.mac \
	disk.mac \
	uptime.mac \
	pool.mac \
	vbuf.mac

OBJS = $(SRCS:.mac=.rel)

PROG = rmd.tsk

all: $(PROG)

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

$(PROG): $(OBJS) syslib.lib
	@echo -e "$@/ofmt:tsk,rmd,rmd/q=main,date,uptime,help,command,misc,vbuf,syslib/lb&\r" > link.cmd
	@echo -e "(atd)(mem,disk,pool)(tskhdr)/task=...RMD/pri=225/priv\r" >> link.cmd
	$(ZXCC) $(TKB) -"@link"
	@rm -f link.cmd
#	$(ZXCC) $(TKB) -"$@,rmd,rmd=main/ofmt=tsk,date,uptime,atd,mem,tskhdr,disk,pool,help,command,misc,vbuf,syslib/lb/task=...RMD/pri=225/priv"

copy: $(PROG)
	@echo "cd system" > cprmd.cmd
	@echo "delete rmd.tsk" >> cprmd.cmd
	@echo "import rmd.tsk rmd.tsk /c" >> cprmd.cmd
	@echo "dir" >> cprmd.cmd
	@echo "bye" >> cprmd.cmd
	$(VOL180) /dev/fd0 < cprmd.cmd
	@rm cprmd.cmd

clean:
	rm -f *.rel *.prn *.sym core *.map $(PROG) *~ *.\$$\$$\$$
