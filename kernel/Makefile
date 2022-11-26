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
	cpu280.mac \
	z280rc.mac \
	startup.mac

OBJS = $(SRCS:.mac=.rel)

PROG = kernel.lib

all: startup.rel sysdat.rel cpu280.rel z280rc.rel $(PROG)

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .mac .rel

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

# Note: startup.rel, sysdat.rel, cpu280.rel and z280rc.rel are not included
# in the library:
# - startup.rel will be linked first
# - sysdat.rel will be linked last
# - only one of the machine-dependent modules (cpu280.rel, z280rc.rel) will
#   be included in the system image.
kernel.lib: $(OBJS)
	$(ZXCC) $(LBR) -"$@=tskmgr,flags,sndrcv,device,session,connect,ints"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,kalloc,mmu,syscom,qio,qldr,qmcr,qtktn,ast"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,fork,pars,clkq,sysfn,args"
	@rm src.lib

clean:
	rm -f $(OBJS) $(PROG) *.sym *.prn core *~ *.\$$\$$\$$
