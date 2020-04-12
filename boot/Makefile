# Path to Linux utilities
ZXCC    = zxcc
CKSBOOT = ../Tools/linux/cksboot/cksboot

# Path to CP/M utilities
ZSM4    = ../Tools/cpm/zsm4.com
TKB     = ../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = fdboot.mac hdboot.mac cfboot.mac

OBJS = $(SRCS:.mac=.rel)

all: fdboot.bin hdboot.bin cfboot.bin

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l"

fdboot.bin: fdboot.rel
	$(ZXCC) $(TKB) -"$@=fdboot/ofmt=com/load=0"
	$(CKSBOOT) $@

hdboot.bin: hdboot.rel
	$(ZXCC) $(TKB) -"$@,$(basename $@)=hdboot/ofmt=com/load=0"
	$(CKSBOOT) $@

cfboot.bin: cfboot.rel
	$(ZXCC) $(TKB) -"$@,$(basename $@)=cfboot/ofmt=com/load=0"
	$(CKSBOOT) $@

cpboot: fdboot.bin
	dd if=fdboot.bin of=/dev/fd0

clean:
	rm -f *.bin *.rel *.prn *.sym *.map core *~ *.\$$\$$\$$
