# Path to Linux utilities
ZXCC = zxcc
ECHO = /bin/echo	# builtin 'echo' does not like the -e option in Debian Buster

# Path to CP/M utilities
ZSM4 = ../Tools/cpm/zsm4.com
TKB  = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = acnt.mac \
	acs.mac \
	bye.mac \
	cot.mac \
	dmo.mac \
	hel.mac \
	ini.mac \
	ins.mac \
	mcr.mac \
	mou.mac \
	sav.mac \
	shutup.mac \
	tkn.mac \
	ufd.mac

SYSSRCS = sys.mac \
	sysov1.mac \
	sysov2.mac \
	sysov3.mac

LIBSRCS = fdboot.mac \
	hdboot.mac \
	ttputch.mac

SYSOBJS = $(SYSSRCS:.mac=.rel)
LIBOBJS = $(LIBSRCS:.mac=.rel)

OBJS  = $(SRCS:.mac=.rel)

PROGS = $(OBJS:.rel=.tsk)

all: $(PROGS) sys.tsk

$(OBJS) $(SYSOBJS) $(LIBOBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

acnt.tsk: acnt.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=acnt/of:tsk,syslib/lb/task=ACNT/ext=512/priv"

acs.tsk: acs.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=acs/of:tsk,syslib/lb/task=...ACS/pri=70/priv"

bye.tsk: bye.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=bye/of:tsk,syslib/lb/task=...BYE/pri=50/priv"

cot.tsk: cot.rel syslib.lib fcslib.lib
	$(ZXCC) $(TKB) -"$@=cot/of:tsk,fcslib/lb,syslib/lb/task=COT.../pri=150/priv"

dmo.tsk: dmo.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=dmo/of:tsk,syslib/lb/task=...DMO/pri=160/priv"

hel.tsk: hel.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=hel/of:tsk,syslib/lb/task=...HEL/pri=50/priv"

ini.tsk: ini.rel fdboot.rel hdboot.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=ini/of:tsk,fdboot,hdboot,syslib/lb/task=...INI/pri=60/priv"

ins.tsk: ins.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=ins/of:tsk,syslib/lb/task=...INS/pri=100/priv"

mcr.tsk: mcr.rel ttputch.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=mcr/of:tsk,ttputch,syslib/lb/task=MCR.../pri=160/priv"

mou.tsk: mou.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=mou/of:tsk,syslib/lb/task=...MOU/pri=160/priv"

sav.tsk: sav.rel syslib.lib
	$(ZXCC) $(TKB) -"sav/of:tsk,sav,sav=sav,syslib/lb/task=...SAV/pri=100/priv"

shutup.tsk: shutup.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=shutup/of:tsk,syslib/lb/task=SHUTUP/pri=100/priv"

sys.tsk: $(SYSOBJS) syslib.lib
	@$(ECHO) -e "$@,sys,sys/q=sys/of:tsk,syslib/lb&\r" > build.cmd
	@$(ECHO) -e "(sysov1)(sysov2)(sysov3)/task=...MCR/pri=160/priv" >> build.cmd
	$(ZXCC) $(TKB) -"@build"
	@rm -f build.cmd

tkn.tsk: tkn.rel ttputch.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=tkn/of:tsk,ttputch,syslib/lb/task=TKTN/pri=250/priv"

ufd.tsk: ufd.rel syslib.lib
	$(ZXCC) $(TKB) -"$@=ufd/of:tsk,syslib/lb/task=...UFD/pri=100/priv"

clean:
	rm -f *.rel *.prn *.map *.sym core *.bin *.tsk *~ *.\$$\$$\$$
