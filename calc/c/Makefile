SHELL=/bin/sh

.SUFFIXES: .c .o .obj

SRCS = calc.c \
	vt100.c \
	ttyio.c \
	bcdconv.c

OBJS = $(SRCS:.c=.o)
COBJS = $(SRCS:.c=.obj)

LIBS = -lm

all: calc #calc.com

.c.o:
	cc -g -Wall -c $<

$(COBJS): %.obj: %.c
	zxc -o -c -DCPM $<

calc: calc.o vt100.o ttyio.o bcdconv.o
	cc -g -o $@ calc.o vt100.o ttyio.o bcdconv.o $(LIBS)

calc.com: calc.obj vt100.obj ttyio.obj bcdconv.obj
	@echo "-Z -W3 -Ptext=0,data,bss -C100h -o"$@ "\\" > link.cmd
	@echo "crtcpm.obj calc.obj vt100.obj ttyio.obj bcdconv.obj \\" >> link.cmd
	@echo "cpmlibc.lib libf.lib" >> link.cmd
	zxcc link -"<" +link.cmd
	@rm -f link.cmd

copy: calc.com
	cpmrm -f p112 /dev/fd0 0:calc.com
	cpmcp -f p112 /dev/fd0 calc.com 0:

install: 

clean:
	rm -f ${OBJS} ${COBJS} calc calc.com *~ core *.bak
