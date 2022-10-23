# Path to Linux utilities
ZXCC = zxcc

# Path to CP/M utilities
ZSM4 = ../Tools/cpm/zsm4.com
TKB  = ../Tools/cpm/tkb.com

.SUFFIXES:
.SUFFIXES: .mac .rel

SRCS = cpm.mac map.mac

OBJS = $(SRCS:.mac=.rel)
XRFS = $(SRCS:.mac=.crf)

all: cpm.tsk map.com

# *.inc files are copied from the latest RSX280 build
$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

cpm.tsk: cpm.rel odt.lib
	$(ZXCC) $(TKB) -"$@,cpm,cpm=cpm/ofmt:tsk,odt/lb/load=0/task=...CPM/asg=TI:5,SY:1-4,SY:6-8"

map.com: map.rel
	$(ZXCC) $(TKB) -"$@,,=map/ofmt:com"

clean:
	rm -f *.com *.rel *.prn *.sym *.map core *.tsk *~ *.\$$\$$\$$
