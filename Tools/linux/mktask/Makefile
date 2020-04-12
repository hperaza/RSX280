SHELL=/bin/sh

CC = cc
CFLAGS = -g -Wall
#CFLAGS = -O3
#CFLAGS = -g
LIBS =

SRCS = mktask.c

OBJS = $(SRCS:.c=.o)

PROG = mktask

all: $(PROG)

.SUFFIX: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<

${PROG}: ${OBJS}
	$(CC) -g -o $@ ${OBJS} $(LIBS)

install: 

clean:
	rm -f ${OBJS} $(PROG) *~ core *.bak
