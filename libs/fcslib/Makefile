# Path to Linux utilities
ZXCC    = zxcc
VOL180  = ../../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4    = ../../Tools/cpm/zsm4.com
LBR     = ../../Tools/cpm/lbr.com
DRLIB   = ../../Tools/cpm/drlib.com

SRCS = fopen.mac \
	fclose.mac \
	fread.mac \
	fwrite.mac \
	frdblk.mac \
	fwrblk.mac \
	fputln.mac \
	fseek.mac \
	fpos.mac \
	fsize.mac \
	frattr.mac \
	fwattr.mac \
	fdel.mac \
	fren.mac \
	mkdir.mac \
	fsync.mac \
	qioblk.mac \
	parsefn.mac \
	fputstr.mac \
	fgetch.mac \
	fputch.mac \
	fgetln.mac \
	fflush.mac \
	fincbk.mac \
	fdecbk.mac

INC = sysfn.inc \
	fcslib.inc \
	qio.inc

OBJS = $(SRCS:.mac=.rel)

PROG = fcslib.lib

all: $(PROG)

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .mac .rel

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"="$</l

fcslib.lib: $(OBJS)
	$(ZXCC) $(LBR) -"$@=fgetln/e+fgetch/e+fputstr/e+fputln/e+fputch/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+fopen/e+fclose/e+fread/e+fwrite/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+fseek/e+fpos/e+fsize/e+frattr/e+fwattr/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+fdel/e+fren/e+mkdir/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+frdblk/e+fwrblk/e+fflush/e+fsync/e"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib/e+fincbk/e+fdecbk/e+parsefn/e+qioblk/e"
	@rm src.lib
	$(ZXCC) $(LBR) -",fcslib=fcslib.lib"
	$(ZXCC) $(LBR) -",tty:/m=fcslib.lib"
	$(ZXCC) $(DRLIB) FCSLIB.IRL=FCSLIB.LIB[I]

clean:
	rm -f $(OBJS) $(PROG) *.sym *.prn *.irl core *~ *.\$$\$$\$$
