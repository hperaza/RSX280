# Path to Linux utilities
ZXCC    = zxcc

# Path to CP/M utilities
ZSM4    = ../../../Tools/cpm/zsm4.com
LBR     = ../../../Tools/cpm/lbr.com

SRCS = comp.mac \
	div.mac \
	mult.mac \
	strip.mac \
	start.mac \
	startcpm.mac \
	t.mac \
	tcpm.mac

OBJS = $(SRCS:.mac=.rel)

PROGS = t3xz.lib t3xzcpm.lib

all: $(PROGS)

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .mac .rel

$(OBJS): %.rel: %.mac
	$(ZXCC) $(ZSM4) -"="$</l/s8

t3xz.lib: $(OBJS)
	$(ZXCC) $(LBR) -"$@=t,start,comp,div,mult,strip"
	$(ZXCC) $(LBR) -",t3xz/li=t3xz.lib"
	$(ZXCC) $(LBR) -",tty:/br=t3xz.lib"
#	$(ZXCC) $(LBR) -"t3xz/ix=t3xz.lib"

t3xzcpm.lib: $(OBJS)
	$(ZXCC) $(LBR) -"$@=tcpm,startcpm,comp,div,mult,strip"
	$(ZXCC) $(LBR) -",t3xzcpm/li=t3xzcpm.lib"
	$(ZXCC) $(LBR) -",tty:/br=t3xzcpm.lib"
#	$(ZXCC) $(LBR) -"t3xzcpm/ix=t3xzcpm.lib"

clean:
	rm -f $(OBJS) $(PROGS) *.sym *.prn *.irl core *~ *.\$$\$$\$$
