# Path to Linux utilities
ZXCC = zxcc

# Path to CP/M utilities
ZSM4 = ../Tools/cpm/zsm4.com
TKB  = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = hello.mac \
	ckp.mac \
	cmdprint.mac \
	dmpres.mac \
	sendtest.mac \
	recvtest.mac \
	test.mac \
	test1.mac \
	test2.mac \
	bintest.mac \
	iitest.mac \
	fcstest.mac \
	bcdtest.mac \
	filernd.mac \
	exttest.mac \
	block0.mac \
	gcmltest.mac \
	rnetest.mac \
	exstat.mac \
	ds1820.mac \
	tecken.mac \
	qtest.mac

OBJS = $(SRCS:.mac=.rel)
TSKS = $(SRCS:.mac=.tsk)

all: $(TSKS)

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

$(TSKS): %.tsk: %.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb"

fcstest.tsk: fcstest.rel syslib.lib fcslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,fcslib/lb,syslib/lb"

bcdtest.tsk: bcdtest.rel syslib.lib bcdflt.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,bcdflt/lb,syslib/lb"

block0.tsk: block0.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb/priv"

dmpres.tsk: dmpres.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb/priv"

ckp.tsk: ckp.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt=tsk,syslib/lb/priv/task=...CKP"

clean:
	rm -f *.rel *.prn *.sym core *.map *.tsk *~ *.\$$\$$\$$
