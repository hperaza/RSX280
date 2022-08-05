# Path to Linux utilities
ZXCC    = zxcc
VOL180  = ../../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4    = ../../Tools/cpm/zsm4.com
LBR     = ../../Tools/cpm/lbr.com
DRLIB   = ../../Tools/cpm/drlib.com

SRCS = bin2bcd.mac \
	bcd2bin.mac \
	qioblk.mac \
	sleep.mac \
	clrmem.mac \
	adec.mac \
	adec2.mac \
	hldec.mac \
	ldec.mac \
	hlhex.mac \
	ucase.mac \
	crlf.mac \
	attach.mac \
	detach.mac \
	putstr.mac \
	getch.mac \
	putch.mac \
	rctrlo.mac \
	errstr.mac \
	perror.mac \
	getln.mac \
	parsefn.mac \
	gcml.mac \
	gccl.mac \
	exit.mac \
	cphlde.mac \
	cpy32m.mac \
	add32m.mac \
	cpl32m.mac \
	and32m.mac \
	or32m.mac \
	xor32m.mac \
	shl32m.mac \
	shr32m.mac \
	rlc32m.mac \
	crypt.mac \
	md5.mac \
	crc16.mac \
	cvtbd.mac \
	cvtbd2.mac \
	cvtwd.mac \
	cvtwh.mac \
	cvtwo.mac \
	cvtld.mac \
	savrg.mac \
	savreg.mac \
	saval.mac \
	savall.mac \
	malloc.mac \
	ovlmgr.mac

INC = sysfn.inc \
	qio.inc

OBJS = $(SRCS:.mac=.rel)

PROG = syslib.lib

all: $(PROG)

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .mac .rel

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

syslib.lib: $(OBJS)
	$(ZXCC) $(LBR) -"$@=gcml,attach,detach,malloc,ovlmgr,cvtld,adec,adec2"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,hldec,ldec,hlhex,parsefn,ucase,perror,errstr"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,crlf,bin2bcd,bcd2bin,clrmem,cphlde,exit,rctrlo"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,getln,getch,gccl,putstr,putch,sleep,crc16,md5"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,crypt,cpy32m,cpl32m,add32m,and32m,or32m,xor32m"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,shl32m,shr32m,rlc32m,cvtbd,cvtbd2,cvtwd,cvtwh"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,cvtwo,qioblk,savrg,savreg,saval,savall"
	@rm src.lib
	$(ZXCC) $(LBR) -",syslib/li/cr/un=syslib.lib"
	$(ZXCC) $(LBR) -",tty:/br=syslib.lib"
	$(ZXCC) $(DRLIB) SYSLIB.IRL=SYSLIB.LIB[I]

clean:
	rm -f $(OBJS) $(PROG) *.sym *.prn *.irl core *~ *.\$$\$$\$$
