# Path to Linux utilities
ZXCC = zxcc

# Path to CP/M utilities
ZSM4 = ../Tools/cpm/zsm4.com
LBR  = ../Tools/cpm/lbr.com
TKB  = ../Tools/cpm/tkb.com

SRCS = main.mac

OBJS = $(SRCS:.mac=.rel)

PROG = ldr.tsk

all: $(PROG)

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .mac .rel

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

ldr.tsk: $(OBJS)
	$(ZXCC) $(TKB) -"$@=main/ofmt=tsk/task=LDR.../par=LDRPAR/pri=250/priv"

clean:
	rm -f $(OBJS) $(PROG) ldr.bin *.sym *.prn *.map core *~ *.\$$\$$\$$
