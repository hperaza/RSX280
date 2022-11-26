# Path to Linux utilities
ZXCC    = zxcc
CKSBOOT = ../../Tools/linux/cksboot/cksboot

# Path to CP/M utilities
ZSM4    = ../../Tools/cpm/zsm4.com
TKB     = ../../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = fdboot.mac

OBJS = $(SRCS:.mac=.rel)

all: fdboot.bin

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

fdboot.bin: fdboot.rel
	$(ZXCC) $(TKB) -"$@,$(basename $@)=fdboot/ofmt:com/load:0"
	$(CKSBOOT) $@

clean:
	rm -f *.bin *.rel *.prn *.sym *.map core *~ *.\$$\$$\$$
