# Path to Linux utilities
ZXCC = zxcc

# Path to CP/M utilities
ZSM4 = ../Tools/cpm/zsm4.com
LBR  = ../Tools/cpm/lbr.com

SRCS = args.mac \
	ast.mac \
	clkq.mac \
	connect.mac \
	device.mac \
	flags.mac \
	fork.mac \
	ints.mac \
	kalloc.mac \
	pars.mac \
	mmu.mac \
	qio.mac \
	qmcr.mac \
	qldr.mac \
	qtktn.mac \
	session.mac \
	sndrcv.mac \
	syscom.mac \
	sysdat.mac \
	sysfn.mac \
	tskmgr.mac \
	z280rc.mac

INC = tcb.inc \
	z180.inc

OBJS = $(SRCS:.mac=.rel)

PROG = kernel.lib

all: $(PROG)

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .mac .rel

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

# Note: sysdat.rel is not included (will be linked last)
kernel.lib: $(OBJS)
	$(ZXCC) $(LBR) -"$@=tskmgr/e+flags/e+sndrcv/e+device/e+session/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+connect/e+ints/e+kalloc/e+mmu/e+syscom/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+qio/e+qldr/e+qmcr/e+qtktn/e+ast/e+fork/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+pars/e+clkq/e+sysfn/e+args/e+z280rc/e"
	@rm src.lib

clean:
	rm -f $(OBJS) $(PROG) *.sym *.prn core *~ *.\$$\$$\$$
