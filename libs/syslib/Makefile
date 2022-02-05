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
	$(ZXCC) $(LBR) -"$@=gcml/e+attach/e+detach/e+malloc/e+ovlmgr/e+cvtld/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+adec/e+adec2/e+hldec/e+ldec/e+hlhex/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+parsefn/e+ucase/e+perror/e+errstr/e+crlf/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+bin2bcd/e+bcd2bin/e+clrmem/e+cphlde/e+exit/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+rctrlo/e+getln/e+getch/e+gccl/e+putstr/e+putch/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+sleep/e+crc16/e+md5/e+crypt/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+cpy32m/e+cpl32m/e+add32m/e+and32m/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+or32m/e+xor32m/e+shl32m/e+shr32m/e+rlc32m/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+cvtbd/e+cvtbd2/e+cvtwd/e+cvtwh/e+cvtwo/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+qioblk/e+savrg/e+savreg/e+saval/e+savall/e"
	@rm src.lib
	$(ZXCC) $(LBR) -",syslib=syslib.lib"
	$(ZXCC) $(LBR) -",tty:/m=syslib.lib"
	$(ZXCC) $(DRLIB) SYSLIB.IRL=SYSLIB.LIB[I]

clean:
	rm -f $(OBJS) $(PROG) *.sym *.prn *.irl core *~ *.\$$\$$\$$
