# Path to Linux utilities
ZXCC    = zxcc
VOL180  = ../../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4    = ../../Tools/cpm/zsm4.com
LBR     = ../../Tools/cpm/lbr.com

SRCS = afp.mac \
	atn.mac \
	denorm.mac \
	exp.mac \
	fabs.mac \
	fadd.mac \
	fdiv.mac \
	fint.mac \
	fmul.mac \
	fneg.mac \
	fnorm.mac \
	fpa.mac \
	fpdat.mac \
	fpclr.mac \
	fpi.mac \
	fpload.mac \
	fpmisc.mac \
	fpmove.mac \
	fpow.mac \
	fsgn.mac \
	ftrunc.mac \
	ifp.mac \
	isint.mac \
	log.mac \
	plyevl.mac \
	pshpop.mac \
	rolfr.mac \
	sincos.mac \
	sqr.mac \
	xform.mac

OBJS = $(SRCS:.mac=.rel)

PROG = bcdflt.lib

all: $(PROG)

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .mac .rel

$(OBJS): %.rel: %.mac
	$(ZXCC) $(ZSM4) -"="$</l

bcdflt.lib: $(OBJS)
	$(ZXCC) $(LBR) -"$@=atn,fabs,sincos,fneg,fsgn,fint,fpow,exp,log,sqr"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,xform,plyevl,fadd,fmul,fdiv,fpmisc"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,afp,fpa,ifp,fpi,denorm,fnorm,pshpop"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,ftrunc,rolfr,isint,fpclr,fpload,fpmove,fpdat"
	@rm src.lib
	$(ZXCC) $(LBR) -",bcdflt/li/cr/un=bcdflt.lib"
	$(ZXCC) $(LBR) -",tty:/br=bcdflt.lib"
	$(ZXCC) $(LBR) -"bcdflt/ix=bcdflt.lib"

clean:
	rm -f $(OBJS) $(PROG) *.sym *.prn *.irl *.com *.tsk core *~ *.\$$\$$\$$
