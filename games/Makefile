# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../Tools/cpm/zsm4.com
TKB    = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = xonix.mac \
	snake.mac \
	pacman.mac \
	tetris.mac \
	volcano.mac \
	spcinv.mac \
	termdef.mac \
	cpmio.mac \
	rsxio.mac

OBJS = $(SRCS:.mac=.rel)
XRFS = $(SRCS:.mac=.crf)

all: xonix.com xonix.tsk \
	snake.com snake.tsk \
	pacman.com pacman.tsk \
	tetris.com tetris.tsk \
	volcano.com volcano.tsk \
	spcinv.com spcinv.tsk

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$<"


xonix.com: xonix.rel termdef.rel cpmio.rel
	$(ZXCC) $(TKB) -"xonix=termdef,xonix,cpmio"

xonix.tsk: xonix.rel termdef.rel rsxio.rel syslib.lib
	$(ZXCC) $(TKB) -"xonix=termdef,xonix,rsxio,syslib/lb/ofmt=tsk/task=...XON"


snake.com: snake.rel termdef.rel cpmio.rel
	$(ZXCC) $(TKB) -"snake=termdef,snake,cpmio"

snake.tsk: snake.rel termdef.rel rsxio.rel syslib.lib
	$(ZXCC) $(TKB) -"snake=termdef,snake,rsxio,syslib/lb/ofmt=tsk/task=...SNA"


pacman.com: pacman.rel termdef.rel cpmio.rel
	$(ZXCC) $(TKB) -"pacman=termdef,pacman,cpmio"

pacman.tsk: pacman.rel termdef.rel rsxio.rel syslib.lib
	$(ZXCC) $(TKB) -"pacman=termdef,pacman,rsxio,syslib/lb/ofmt=tsk/task=...PAC"


tetris.com: tetris.rel termdef.rel cpmio.rel
	$(ZXCC) $(TKB) -"tetris=termdef,tetris,cpmio"

tetris.tsk: tetris.rel termdef.rel rsxio.rel syslib.lib
	$(ZXCC) $(TKB) -"tetris=termdef,tetris,rsxio,syslib/lb/ofmt=tsk/task=...TET"


volcano.com: volcano.rel termdef.rel cpmio.rel
	$(ZXCC) $(TKB) -"volcano=termdef,volcano,cpmio"

volcano.tsk: volcano.rel termdef.rel rsxio.rel syslib.lib
	$(ZXCC) $(TKB) -"volcano=termdef,volcano,rsxio,syslib/lb/ofmt=tsk/task=...VOL"


spcinv.com: spcinv.rel termdef.rel cpmio.rel
	$(ZXCC) $(TKB) -"spcinv=termdef,spcinv,cpmio"

spcinv.tsk: spcinv.rel termdef.rel rsxio.rel syslib.lib
	$(ZXCC) $(TKB) -"spcinv=termdef,spcinv,rsxio,syslib/lb/ofmt=tsk/task=...INV"


cpcpm: xonix.com snake.com pacman.com tetris.com volcano.com spcinv.com
	cpmrm -f p112 /dev/fd0 0:xonix.com
	cpmrm -f p112 /dev/fd0 0:snake.com
	cpmrm -f p112 /dev/fd0 0:pacman.com
	cpmrm -f p112 /dev/fd0 0:tetris.com
	cpmrm -f p112 /dev/fd0 0:volcano.com
	cpmrm -f p112 /dev/fd0 0:spcinv.com
	cpmcp -f p112 /dev/fd0 xonix.com 0:
	cpmcp -f p112 /dev/fd0 snake.com 0:
	cpmcp -f p112 /dev/fd0 pacman.com 0:
	cpmcp -f p112 /dev/fd0 tetris.com 0:
	cpmcp -f p112 /dev/fd0 volcano.com 0:
	cpmcp -f p112 /dev/fd0 spcinv.com 0:

cprsx: xonix.tsk snake.tsk pacman.tsk tetris.tsk volcano.tsk spcinv.tsk
	@echo "cd games" > copy.cmd
	@echo "delete xonix.tsk" >> copy.cmd
	@echo "delete snake.tsk" >> copy.cmd
	@echo "delete pacman.tsk" >> copy.cmd
	@echo "delete tetris.tsk" >> copy.cmd
	@echo "delete volcano.tsk" >> copy.cmd
	@echo "delete spcinv.tsk" >> copy.cmd
	@echo "import xonix.tsk xonix.tsk /c" >> copy.cmd
	@echo "import snake.tsk snake.tsk /c" >> copy.cmd
	@echo "import pacman.tsk pacman.tsk /c" >> copy.cmd
	@echo "import tetris.tsk tetris.tsk /c" >> copy.cmd
	@echo "import volcano.tsk volcano.tsk /c" >> copy.cmd
	@echo "import spcinv.tsk spcinv.tsk /c" >> copy.cmd
	@echo "dir" >> copy.cmd
	@echo "bye" >> copy.cmd
	$(VOL180) /dev/fd0 < copy.cmd
	@rm copy.cmd

clean:
	rm -f *.com *.bin *.tsk *.rel *.sym *.prn *.crf *.map core *~ *.\$$\$$\$$
