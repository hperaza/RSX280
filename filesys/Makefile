# Path to Linux utilities
ZXCC = zxcc

# Path to CP/M utilities
ZSM4 = ../Tools/cpm/zsm4.com
TKB  = ../Tools/cpm/tkb.com

SRCS = bitmap.mac \
	buffer.mac \
	blockio.mac \
	mount.mac \
	fileio.mac \
	indexf.mac \
	main.mac

INC = tcb.inc \
	dcb.inc \
	fcb.inc \
	vcb.inc \
	sysfn.inc \
	errors.inc \
	z180.inc

OBJS = $(SRCS:.mac=.rel)

PROG = sysfcp.tsk

all: $(PROG)

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .mac .rel

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

sysfcp.tsk: $(OBJS)
	$(ZXCC) $(TKB) -"sysfcp,sysfcp,sysfcp=main,fileio,mount,indexf,bitmap,buffer,blockio/of=tsk/task=SYSFCP/par=FCPPAR/pri=200/acp/priv"

clean:
	rm -f $(OBJS) $(PROG) sysfcp.bin *.sym *.prn *.map core *~ *.\$$\$$\$$
