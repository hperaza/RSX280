SHELL=/bin/sh

CC = cc
CFLAGS = -g -Wall
#CFLAGS = -O3
#CFLAGS = -g
LIBS =

SRCS = main.c \
	mount.c \
	dirio.c \
	indexf.c \
	fileio.c \
	bitmap.c \
	buffer.c \
	blockio.c \
	mkdisk.c \
	check.c \
	vmr.c \
	misc.c

OBJS = $(SRCS:.c=.o)

OBJS_VOL180 = main.o \
	mount.o \
	dirio.o \
	indexf.o \
	fileio.o \
	bitmap.o \
	buffer.o \
	blockio.o \
	mkdisk.o \
	vmr.o \
	misc.o

OBJS_FSCK180 = check.o \
	buffer.o \
	blockio.o \
	indexf.o \
	fileio.o \
	bitmap.o \
	dirio.o \
	misc.o

PROGS = vol180 fsck180

.SUFFIX: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<

all: ${PROGS}

vol180: ${OBJS_VOL180}
	$(CC) -g -o $@ ${OBJS_VOL180} $(LIBS)

fsck180: ${OBJS_FSCK180}
	$(CC) -g -o $@ ${OBJS_FSCK180} $(LIBS)

install: 

clean:
	rm -f ${OBJS} $(PROGS) *~ core *.bak
