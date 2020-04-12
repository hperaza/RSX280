/***********************************************************************

   This file is part of vtcp, a virtual terminal daemon for connecting
   to a P112 machine running RSX180 with the multiplexed terminal driver.

   Copyright (C) 2016-2020, Hector Peraza.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>

#include "serial.h"

// open serial port, return file descriptor on success or -1 on error;
// errno contains the error number.

int serial_open(const char *dev, int speed) {
  struct termios ts;
  int fd, modemlines, off = 0;

  fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK, 0);
  if (fd < 0) return -1;

  if (!isatty(fd)) {
    close(fd);
    return -1;
  }
  if (tcgetattr(fd, &ts) == -1) {
    close(fd);
    return -1;
  }
  cfmakeraw(&ts);
  ts.c_iflag = IGNBRK;	/* | IGNCR; */
  ts.c_oflag = 0;
  ts.c_cflag = CS8 | CREAD | CLOCAL;
  ts.c_lflag = 0;
  ts.c_cc[VMIN] = 1;
  ts.c_cc[VTIME] = 0;
  if (cfsetospeed(&ts, speed) == -1) {
    close(fd);
    return -1;
  }
  if (cfsetispeed(&ts, speed) == -1) {
    close(fd);
    return -1;
  }
  if (tcsetattr(fd, TCSAFLUSH, &ts) == -1) {
    close(fd);
    return -1;
  }

  // set the line back to blocking mode after setting CLOCAL.
  if (ioctl(fd, FIONBIO, &off) < 0) {
    close(fd);
    return -1;
  }
  modemlines = TIOCM_RTS;
  if (ioctl(fd, TIOCMBIC, &modemlines)) {
    // failed to clear RTS line
    close(fd);
    return -1;
  }

  return fd;
}

int serial_close(int fd) {
  close(fd);
  return 0;
}

int serial_getc(int fd, char *c) {
  return read(fd, c, 1);
}

int serial_putc(int fd, char c) {
  return write(fd, &c, 1);
}
