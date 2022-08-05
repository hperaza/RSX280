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
	freload.mac \
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
	fdecbk.mac \
	finc24.mac \
	fdec24.mac \
	fcmp24.mac

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
	$(ZXCC) $(LBR) -"$@=fgetln,fgetch,fputstr,fputln,fputch,fopen,fclose"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,fread,fwrite,freload,fseek,fpos,fsize,fdel"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,fren,mkdir,fflush,fsync,frdblk,fwrblk"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,frattr,fwattr,fincbk,fdecbk,parsefn,qioblk"
	@cp $@ src.lib
	$(ZXCC) $(LBR) -"$@=src.lib,finc24,fdec24,fcmp24"
	@rm src.lib
	$(ZXCC) $(LBR) -",fcslib/li/cr/un=fcslib.lib"
	$(ZXCC) $(LBR) -",tty:/br=fcslib.lib"
	$(ZXCC) $(DRLIB) FCSLIB.IRL=FCSLIB.LIB[I]

clean:
	rm -f $(OBJS) $(PROG) *.sym *.prn *.irl core *~ *.\$$\$$\$$
