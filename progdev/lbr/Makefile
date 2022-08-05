# Path to Linux utilities
ZXCC = zxcc

# Path to CP/M utilities
ZSM4 = ../../Tools/cpm/zsm4.com
TKB  = ../../Tools/cpm/tkb.com

.SUFFIXES:
.SUFFIXES: .mac .rel

SRCS = lbr.mac \
	cpmio.mac \
	rsxio.mac

OBJS = $(SRCS:.mac=.rel)
XRFS = $(SRCS:.mac=.crf)

all: lbr.com lbr.tsk

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

lbr.com: $(OBJS)
	$(ZXCC) $(TKB) -"$@,lbrcpm=lbr,cpmio/of:c"

lbr.tsk: $(OBJS) syslib.lib fcslib.lib
	$(ZXCC) $(TKB) -"$@,,lbr=lbr/of:t,rsxio,fcslib/lb,syslib/lb/task=...LBR/ext=2000"

clean:
	rm -f *.com *.rel *.prn *.sym *.map core *.tsk *~ *.\$$\$$\$$
