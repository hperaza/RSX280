# Path to Linux utilities
ZXCC = zxcc

# Path to CP/M utilities
ZSM4 = ../Tools/cpm/zsm4.com
LBR  = ../Tools/cpm/lbr.com

SRCS = startup.mac \
	init.mac

INC = tcb.inc \
	sysfn.inc \
	z180.inc

OBJS = $(SRCS:.mac=.rel)

all: init.lib

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .mac .rel

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

init.lib: $(OBJS)	# note startup.rel not included
	$(ZXCC) $(LBR) -"$@=init"

clean:
	rm -f $(OBJS) init.lib *.sym *.prn core *~ *.\$$\$$\$$
