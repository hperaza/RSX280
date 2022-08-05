# Path to Linux utilities
ZXCC    = zxcc
VOL180  = ../../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4    = ../../Tools/cpm/zsm4.com
LBR     = ../../Tools/cpm/lbr.com
DRLIB   = ../../Tools/cpm/drlib.com

SRCS = odt280.mac dis280.mac

INC = sysfn.inc

OBJS = $(SRCS:.mac=.rel)

PROG = odt.lib

all: $(PROG)

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .mac .rel

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

$(PROG): $(OBJS)
	$(ZXCC) $(LBR) -"$@=odt280,dis280"
	$(ZXCC) $(LBR) -",odt/li/cr/un=odt.lib"
	$(ZXCC) $(LBR) -",tty:/br=odt.lib"
	$(ZXCC) $(DRLIB) ODT.IRL=ODT.LIB[I]

clean:
	rm -f $(OBJS) $(PROG) *.sym *.prn *.irl core *~ *.\$$\$$\$$
