ZXCC   = zxcc
VOL180 = ../Tools/linux/vol180/vol180
ZSM4   = ../Tools/cpm/zsm4.com
TKB    = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRC = kmit.mac \
	kcom.mac \
	kpkt.mac \
	krem.mac \
	kser.mac \
	ktt.mac \
	kcmd.mac \
	kutl.mac \
	kdat.mac \
	ksys.mac

OBJ = $(SRC:.mac=.rel)

PROG = kermit.tsk

all: $(PROG)

$(OBJ): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

$(PROG): $(OBJ) syslib.lib
	$(ZXCC) $(TKB) -"kermit,,kermit=kmit,kcom,kpkt,krem,kser,ktt,kcmd,kutl,kdat,ksys,syslib/lb/of=tsk/task=...ker/ext=8200/asg=TI:1-2,SY:3-8"
#	cat kermit.map

copy: $(PROG)
	@echo "cd test" > cpker.cmd
	@echo "delete kermit.tsk" >> cpker.cmd
	@echo "import kermit.tsk kermit.tsk /c" >> cpker.cmd
	@echo "delete kermit.ini" >> cpker.cmd
	@echo "import kermit.ini kermit.ini" >> cpker.cmd
	@echo "dir" >> cpker.cmd
	@echo "bye" >> cpker.cmd
	$(VOL180) ../floppy.img < cpker.cmd
	@rm cpker.cmd

clean:
	rm -f *.bak *~ $(OBJ) $(PROG) *.prn *.sym *.map
