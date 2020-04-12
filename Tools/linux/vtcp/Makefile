SHELL=/bin/sh

CC = cc
CFLAGS = -g -Wall
#CFLAGS = -O3
#CFLAGS = -g
LIBS =

SRCS = main.c tcp.c serial.c ttyio.c telnet.c

OBJS = $(SRCS:.c=.o)

PROG = vtcp

all: vtcp

.SUFFIX: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<

${PROG}: ${OBJS}
	$(CC) -g -o $@ ${OBJS} $(LIBS)

install: 

clean:
	rm -f ${OBJS} $(PROG) *~ core *.bak
