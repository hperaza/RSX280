# Path to Linux utilities
ZXCC    = zxcc
VOL180  = ../../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4    = ../../Tools/cpm/zsm4.com
LBR     = ../../Tools/cpm/lbr.com
DRLIB   = ../../Tools/cpm/drlib.com

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
	$(ZXCC) $(LBR) -"$@=atn/e+fabs/e+sincos/e+fneg/e+fsgn/e+fint/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+fpow/e+exp/e+log/e+sqr/e+xform/e+plyevl/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+fadd/e+fmul/e+fdiv/e+fpmisc/e+afp/e+fpa/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+ifp/e+fpi/e+denorm/e+fnorm/e+pshpop/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+ftrunc/e+rolfr/e+isint/e+fpclr/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+fpload/e+fpmove/e+fpdat/e"
	@rm src.lib
	$(ZXCC) $(LBR) -",bcdflt=bcdflt.lib"
	$(ZXCC) $(LBR) -",tty:/m=bcdflt.lib"
	$(ZXCC) $(DRLIB) BCDFLT.IRL=BCDFLT.LIB[I]

clean:
	rm -f $(OBJS) $(PROG) *.sym *.prn *.irl *.com *.tsk core *~ *.\$$\$$\$$
