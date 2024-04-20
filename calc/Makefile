# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../Tools/cpm/zsm4.com
TKB    = ../Tools/cpm/tkb.com

.SUFFIXES:	# delete the default suffixes
.SUFFIXES: .mac .rel

SRCS = calc.mac \
	format.mac \
	termdef.mac \
	cpmio.mac \
	rsxio.mac

OBJS = $(SRCS:.mac=.rel)

all:	calc.com calc.tsk

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

calc.com: $(OBJS) *.lib # using RSX180 SYSLIB on CP/M for ALLOC/FREE
	$(ZXCC) $(TKB) -"calc/of:c,calccpm=termdef,calc,format,cpmio,syslib/lb,bcdflt/lb"

calc.tsk: $(OBJS) *.lib
	$(ZXCC) $(TKB) -"calc/of:t,,calc=termdef,calc,format,rsxio,bcdflt/s,fcslib/s,syslib/s/task=...cal/ext=12000"

clean:
	rm -f $(OBJS) calc.com calc.tsk *.sym *.prn *.map core *~ *.\$$\$$\$$

copy: calc.tsk
	@echo "cd test" > cpy.cmd
	@echo "delete calc.tsk" >> cpy.cmd
	@echo "import calc.tsk calc.tsk /c" >> cpy.cmd
	@echo "delete calc.hlp" >> cpy.cmd
	@echo "import calc.hlp calc.hlp" >> cpy.cmd
	@echo "delete test.cal" >> cpy.cmd
	@echo "import test/test.cal test.cal" >> cpy.cmd
	@echo "dir" >> cpy.cmd
	@echo "bye" >> cpy.cmd
	$(VOL180) ../cf-partition.img < cpy.cmd
	@rm cpy.cmd
