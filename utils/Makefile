# Path to Linux utilities
ZXCC = zxcc

# Path to CP/M utilities
ZSM4 = ../Tools/cpm/zsm4.com
TKB  = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = cal.mac \
	clock.mac \
	cookie.mac \
	dmp.mac \
	md5.mac \
	murphy.mac \
	quote.mac \
	view.mac \
	fdisk.mac \
	fmt.mac \
	bad.mac \
	vfy.mac

LIBSRCS = msboot.mac \
	mbboot.mac

OBJS = $(SRCS:.mac=.rel)
LIBOBJS = $(LIBSRCS:.mac=.rel)
PROGS = $(OBJS:.rel=.tsk)

all: $(PROGS)

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

$(LIBOBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

cal.tsk: cal.rel
	$(ZXCC) $(TKB) -"$@=$</ofmt:tsk/task=...CAL"

clock.tsk: clock.rel fcslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt:tsk,syslib/lb/task=...CLK"

dmp.tsk: dmp.rel fcslib.lib syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt:tsk,fcslib/lb,syslib/lb/task=...DMP"

md5.tsk: md5.rel fcslib.lib syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt:tsk,fcslib/lb,syslib/lb/task=...MD5"

view.tsk: view.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt:tsk,syslib/lb/ext=10000/task=...VIE"

fdisk.tsk: fdisk.rel msboot.rel mbboot.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt:tsk,msboot,mbboot,syslib/lb/task=...FDI/pri=65/priv"

fmt.tsk: fmt.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt:tsk,syslib/lb/task=...FMT/pri=65"

bad.tsk: bad.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt:tsk,syslib/lb/task=...BAD/pri=100/priv"

vfy.tsk: vfy.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt:tsk,syslib/lb/task=...VFY"

quote.tsk cookie.tsk murphy.tsk: %.tsk: %.rel syslib.lib fcslib.lib
	$(ZXCC) $(TKB) -"$@=$</ofmt:tsk,fcslib/lb,syslib/lb"

clean:
	rm -f *.rel *.prn *.sym core *.map *.tsk *~ *.\$$\$$\$$
