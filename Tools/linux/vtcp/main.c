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
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include <sys/time.h>
#include <sys/select.h>

#include <arpa/inet.h>	// for ntohl

#include <termios.h>

#include "main.h"
#include "tcp.h"
#include "serial.h"
#include "ttyio.h"
#include "telnet.h"

int listen_fd, serial_fd, vt_fd[8];
int blen = 0, telnet = 0, debug = 0, mxmode = 1;

int usage(char *p) {
  printf("usage: %s tty [-b speed] [-p port] [-t] [-d]\n", p);
  return 1;
}

char *date_string(time_t t) {
  static char s[100];
  
  strcpy(s, ctime(&t));
  if (s[strlen(s)-1] == '\n') s[strlen(s)-1] = '\0';

  return s;
}

int baud(int speed) {
  switch (speed) {
    case   B300: return   300;
    case   B600: return   600;
    case  B1200: return  1200;
    case  B2400: return  2400;
    case  B4800: return  4800;
    case  B9600: return  9600;
    case B19200: return 19200;
    case B38400: return 38400;
    default:     return     0;
  }
}

int main(int argc, char *argv[]) {
  int i, fw, nf, speed = B9600;
  unsigned long h;
  unsigned short p = 8000, pl;
  fd_set rset, wset;
  char *ttydev = NULL;
  //struct timeval tmout;
  
  for (i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-h") == 0) {
      return usage(argv[0]);
    } else if (strcmp(argv[i], "-p") == 0) {
      if (i < argc-1) ++i; else return usage(argv[0]);
      p = atoi(argv[i]);
    } else if (strcmp(argv[i], "-b") == 0) {
      if (i < argc-1) ++i; else return usage(argv[0]);
      speed = atoi(argv[i]);
    } else if (strcmp(argv[i], "-t") == 0) {
      telnet = 1;
    } else if (strcmp(argv[i], "-d") == 0) {
      debug = 1;
    } else {
      if (!ttydev) {
        ttydev = argv[i];
      } else {
        return usage(argv[0]);
      }
    }
  }

  if (!ttydev) {
    fprintf(stderr, "%s: no serial device specified\n", argv[0]);
    return 1;
  }
  
  serial_fd = -1;
  listen_fd = -1;
  for (i = 0; i < 8; ++i) vt_fd[i] = -1;
  
  switch (speed) {
    case   300: speed =   B300; break;
    case   600: speed =   B600; break;
    case  1200: speed =  B1200; break;
    case  2400: speed =  B2400; break;
    case  4800: speed =  B4800; break;
    case  9600: speed =  B9600; break;
    case 19200: speed = B19200; break;
    case 38400: speed = B38400; break;
    default:
      fprintf(stderr, "%s: unsupported speed %d, defaulting to 9600\n",
                      argv[0], speed);
      speed = B9600;
      break;
  }
  
  serial_fd = serial_open(ttydev, speed);
  if (serial_fd < 0) {
    fprintf(stderr, "%s: could not open %s: %s\n", argv[0], ttydev,
                    strerror(errno));
    return 1;
  } else {
    printf("Connected to %s, %d baud\n", ttydev, baud(speed));
  }
  
  pl = p;
  listen_fd = tcp_listen(&h, &pl);
  if (listen_fd < 0) {
    fprintf(stderr, "%s: could not listen on port %d: %s\n",
                    argv[0], p, strerror(errno));
    return 1;
  } else {
    printf("Listening on port %d", p);
    if (telnet) printf(", telnet mode");
    printf("\n");
  }
  
  printf("Press Q to quit, D to toggle debug mode\n");

  init_term();

  for (;;) {
    // wait for events
    FD_ZERO(&rset);
    FD_ZERO(&wset);

    fw = fileno(stdin) + 1;
    FD_SET(fileno(stdin), &rset);

    if (fw <= listen_fd) fw = listen_fd + 1;
    FD_SET(listen_fd, &rset);
    
    if (fw <= serial_fd) fw = serial_fd + 1;
    FD_SET(serial_fd, &rset);
  
    for (i = 0; i < 8; ++i) {
      if (vt_fd[i] >= 0) {
        if (fw <= vt_fd[i]) fw = vt_fd[i] + 1;
        FD_SET(vt_fd[i], &rset);
        //FD_SET(vt_fd[i], &wset);
      }
    }

    nf = select(fw, &rset, &wset, NULL, NULL);
    if (nf > 0) {
      if (FD_ISSET(fileno(stdin), &rset)) {
        // stdin event
        int c = get_char();
        if (toupper(c) == 'Q') {
          break;
        } else if (toupper(c) == 'X') {
          int active = 0;
          for (i = 0; i < 8; ++i) {
            if (vt_fd[i] >= 0) {
              active = 1;
              break;
            }
          }
          if (active) {
            printf("Connections active, can't switch mode\r\n");
          } else {
            if (mxmode) {
              printf("Multiplexed mode OFF\r\n");
              mxmode = 0;
            } else {
              printf("Multiplexed mode ON\r\n");
              mxmode = 1;
            }
          }
        } else if (toupper(c) == 'D') {
          debug = !debug;
          printf("Debug mode %s\r\n", debug ? "on" : "off");
        }
      }
      
      if (FD_ISSET(listen_fd, &rset)) {
        // new connection
        unsigned long h;
        unsigned short p;
        time_t now;
        now = time(NULL);
        int fd = tcp_accept(listen_fd);
        tcp_get_local_address(fd, &h, &p);
        h = ntohl(h);
        printf("%s - New connection from %lu.%lu.%lu.%lu, ",
               date_string(now),
               (h >> 24) & 0xff, (h >> 16) & 0xff, (h >> 8) & 0xff, h & 0xff);
        if (fd >= 0) {
          int found = 0;
          for (i = 0; i < 8; ++i) {
            if (vt_fd[i] < 0) {
              vt_fd[i] = fd;
              found = 1;
              break;
            }
          }
          if (!found) {
            printf("no free lines: closing connection\r\n");
            close(fd);
          } else {
            printf("using line %d\r\n", i);
            // try to put telnet client in char mode
            if (telnet) telnet_init(fd);
          }
        }
      }
      if (FD_ISSET(listen_fd, &wset)) {
        printf("listen_fd wset\r\n");
      }

      if (FD_ISSET(serial_fd, &rset)) {
        // serial port char
        char c;
        serial_getc(serial_fd, &c);
        process_serial_char(c);
      }
      if (FD_ISSET(serial_fd, &wset)) {
        printf("serial_fd wset\r\n");
      }
      
      for (i = 0; i < 8; ++i) {
        if (vt_fd[i] >= 0) {
          if (FD_ISSET(vt_fd[i], &rset)) {
            // TCP/IP char
            int len;
            char buf[16];
            len = tcp_receive(vt_fd[i], buf, 16);
            if (len == 0) {
              unsigned long h;
              unsigned short p;
              time_t now;
              now = time(NULL);
              tcp_get_local_address(vt_fd[i], &h, &p);
              h = ntohl(h);
              printf("%s - %lu.%lu.%lu.%lu closed connection on line %d\r\n",
               date_string(now),
               (h >> 24) & 0xff, (h >> 16) & 0xff, (h >> 8) & 0xff, h & 0xff,
               i);
              close(vt_fd[i]);
              vt_fd[i] = -1;
            } else {
              int j, len2 = 0;
              if (telnet) {
                for (j = 0; j < len; ++j) {
                  if (telnet_filter(buf[j])) buf[len2++] = buf[j];
                }
              } else {
                len2 = len;
              }
              process_tcp_chars(buf, len2, i);
            }
          }
        }
      }
      for (i = 0; i < 8; ++i) {
        if (vt_fd[i] >= 0) {
          if (FD_ISSET(vt_fd[i], &wset)) {
            printf("vt_fd[%i] wset\r\n", i);
          }
        }
      }
    
    } else {
      if (errno == EINTR) break;
    }
    
    // how to detect closed connection? (in send/recv?)
  }
  
  reset_term();

  for (i = 0; i < 8; ++i) if (vt_fd[i] >= 0) close(vt_fd[i]);
  close(listen_fd);

  return 0;
}

int process_serial_char(char c) {
  static int len = 0;
  static int line = 0;

  if (debug) {
    if (c & 0x80) {
      printf("%02X: packet to line %d, len %d\r\n",
             c & 0xff, (c >> 4) & 0x7, c & 0xf);
    } else {
      printf(" -> %02X %c\r\n",
             c & 0xff, (c >= 0x20) && (c < 0x7F) ? c : ' ');
    }
  }
  if (mxmode) {
    if (c & 0x80) {
      // switch to another line
      line = (c >> 4) & 0x7;
      len = c & 0xf;
    } else {
      // normal char
      if (len > 0) {
        --len;
        send_tcp_char(c, line);
      } else {
        // ignore any spurious chars
        return 0;
      }
    }
  } else {
    send_tcp_char(c, 0);
    len = 0;
  }
  return 1;
}

int process_tcp_chars(char *c, int n, int line) {
  int i, len;

  if (mxmode) {
    while (n >= 0) {
      if (n > 15) len = 15; else len = n;
      // send header with line number length
      send_serial_char(0x80 | (line & 7) << 4 | len);
      // send data
      for (i = 0; i < len; ++i) send_serial_char(*c++ & 0x7f);
      n -= 15;
    }
  } else {
    for (i = 0; i < n; ++i) send_serial_char(*c++ & 0x7f);
  }
  return 1;
}

int send_serial_char(char c) {
  if (debug) {
    if (c & 0x80) {
      printf("%02X: packet from line %d, len %d\r\n",
             c & 0xff, (c >> 4) & 0x7, c & 0xf);
    } else {
      printf(" <- %02X %c\r\n",
             c & 0xff, (c >= 0x20) && (c < 0x7F) ? c : ' ');
    }
  }
  return serial_putc(serial_fd, c);
}

int send_tcp_char(char c, int line) {
  if ((line < 0) || (line > 7)) return 0;
  if (vt_fd[line] >= 0) tcp_send(vt_fd[line], &c, 1);

  return 1;
}
