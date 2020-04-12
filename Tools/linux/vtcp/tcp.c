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
#include <errno.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#ifdef __FreeBSD__
#include <netinet/in.h>
#endif
#include <sys/socket.h>

#include <sys/time.h>
#include <sys/select.h>

#include "tcp.h"

struct sockaddr_in host;
struct hostent *hp;

static int _connect(int fd, int async);

int tcp_build_address(const char *server, int port) {
  int fd = 0;
//  char buf[1024];

  if (!server) {
    host.sin_addr.s_addr = INADDR_ANY;
    host.sin_family = AF_INET;
//    gethostname(buf, sizeof(buf));
//    server = buf;
  } else {
    if ((host.sin_addr.s_addr = inet_addr(server)) != -1) {
      host.sin_family = AF_INET;
    } else {
      if ((hp = gethostbyname(server)) != NULL) {
        bzero((char *) &host, sizeof(host));
       	bcopy(hp->h_addr, (char *) &host.sin_addr, hp->h_length);
        host.sin_family = hp->h_addrtype;
      } else {
        fd = -2;
      }
    }
  }
  
  if (!fd) {
    host.sin_port = (unsigned short) htons(port);
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      fd = -3;  // address error
    }
  }

  return fd;
}

int tcp_bind(int port) {

  int fd = tcp_build_address(0, port);

  if (fd > 0) {
    if (bind(fd, (struct sockaddr *) &host, sizeof(host)) < 0) {
      close(fd);
      fd = -4;
      return fd;
    }
    fcntl(fd, F_SETFL, FNDELAY);
    fcntl(fd, F_SETLK, F_UNLCK);
  }

  return fd;
}

int tcp_connect_s(const char *server, int port, int async) {
  int fd = tcp_build_address(server, port);
  if (fd > 0) fd = _connect(fd, async);
  return fd;
}

int tcp_connect(unsigned long server, int port, int async) {
  int fd;

  host.sin_addr.s_addr = htonl(server);
  host.sin_family = AF_INET;
  host.sin_port = (unsigned short) htons(port);

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fd = -3;  // address error
    return fd;
  }

  return _connect(fd, async);
}

static int _connect(int fd, int async) {
  int status, curState, origState;

  if (fd > 0) {

    // Set the close-on-exec flag so that the socket will not get
    // inherited by child processes.
    fcntl(fd, F_SETFD, FD_CLOEXEC);

    //if (connect(fd, (struct sockaddr *) &host, sizeof(host)) < 0) {
    //  close(fd);
    //  fd = -4;
    //}

    // Attempt to connect. The connect may fail at present with an
    // EINPROGRESS but at a later time it will complete.

    if (async) {
#ifdef USE_FIONBIO
      curState = 1;
      status = ioctl(fd, FIONBIO, &curState);
#else
      origState = fcntl(fd, F_GETFL);
      curState = origState | O_NONBLOCK;
      status = fcntl(fd, F_SETFL, curState);
#endif            
    } else {
      status = 0;
    }

    if (status > -1) {
      status = connect(fd, (struct sockaddr *) &host, sizeof(host));
      if (status < 0) {
        if (errno == EINPROGRESS) {
//          asyncConnect = 1;
          status = 0;
        } else {
          close(fd);
          fd = -4;
        }
      }
    }

  }

  if (fd > 0) {
    fcntl(fd, F_SETFL, FNDELAY);
    fcntl(fd, F_SETLK, F_UNLCK);
  }

  return fd;
}

int tcp_get_local_address(int fd, unsigned long *hostr, unsigned short *portr) {
  int length = sizeof(host);

  if (getsockname(fd, (struct sockaddr *) &host, (socklen_t *) &length))
    return 0;

  *hostr = host.sin_addr.s_addr;
  *portr = host.sin_port;

  return 1;
}

int tcp_listen(unsigned long *hostr, unsigned short *portr) {
  unsigned long th;
  unsigned short tp;
  int fd;

  fd = tcp_bind(*portr);
  if (fd < 0) return fd;

  if (!tcp_get_local_address(fd, hostr, &tp)) return -1;
  if (!tcp_get_local_address(fd, &th, portr)) return -1;
  if (listen(fd, 4) < 0) return -1;

  return fd;
}

int tcp_accept(int sockfd) {
  int fd, len;
  
  len = sizeof(struct sockaddr_in);

  fd = accept(sockfd, (struct sockaddr *) &host, (socklen_t *) &len);
  if (fd < 0) return fd;
  fcntl(fd, F_SETFD, FD_CLOEXEC);

  return fd;
}

int tcp_send(int fd, char *buf, unsigned long len) {
  return send(fd, buf, len, 0);
}

int tcp_receive(int fd, char *buf, unsigned long len) {
  return recv(fd, buf, len, 0);
}
