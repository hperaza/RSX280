.PREFIX:
.PREFIX: .mac .rel

SRCS = main.mac \
	debug280.mac \
	dis280.mac

OBJS = $(SRCS:.mac=.rel)

all: debug.tsk

$(OBJS): %.rel: %.mac *.inc
	zxcc zsm4 -"=$</l"

debug.tsk: $(OBJS)
	zxcc tkb -"$@,,debug=main,debug280,dis280,syslib/lb/ofmt:tsk/task=...ddt/asg=TI:1:2,SY:3:4/pri=70/priv"

clean:
	rm -f *.rel *.prn *.tsk *.sym *.map core *~ *.\$$\$$\$$

copy: debug.tsk
	@echo "cd test" > cp.cmd
	@echo "delete debug.tsk" >> cp.cmd
	@echo "import debug.tsk debug.tsk /c" >> cp.cmd
	@echo "dir" >> cp.cmd
	@echo "bye" >> cp.cmd
	../../Tools/linux/vol180/vol180 ../../cf-partition.img < cp.cmd
	@rm cp.cmd

