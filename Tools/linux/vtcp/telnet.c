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

/* Adapted from SIMH */

#include "telnet.h"
#include "tcp.h"

/* Telnet protocol constants - negatives are for init'ing signed char data */

#define TN_IAC          -1                              /* protocol delim */
#define TN_DONT         -2                              /* dont */
#define TN_DO           -3                              /* do */
#define TN_WONT         -4                              /* wont */
#define TN_WILL         -5                              /* will */
#define TN_BRK          -13                             /* break */
#define TN_BIN          0                               /* bin */
#define TN_ECHO         1                               /* echo */
#define TN_SGA          3                               /* sga */
#define TN_LINE         34                              /* line mode */
#define TN_CR           015                             /* carriage return */

/* Telnet line states */

#define TNS_NORM        000                             /* normal */
#define TNS_IAC         001                             /* IAC seen */
#define TNS_WILL        002                             /* WILL seen */
#define TNS_WONT        003                             /* WONT seen */
#define TNS_SKIP        004                             /* skip next */

static char mantra[] = {
  TN_IAC, TN_WILL, TN_LINE,
  TN_IAC, TN_WILL, TN_SGA,
  TN_IAC, TN_WILL, TN_ECHO,
  TN_IAC, TN_WILL, TN_BIN,
  TN_IAC, TN_DO, TN_BIN
};


//----------------------------------------------------------------------

int telnet_init(int fd) {
  tcp_send(fd, (char *) mantra, 15);
  return 1;
}

int telnet_filter(char c) {
  static int tsta = 0;  // Telnet state
  static int dstb = 0;  // disable Telnet bin (default bin mode)

  // Examine new data, remove TELNET cruft before making input available

  switch (tsta) {         // case telnet state
    case TNS_NORM:                     // normal
      if (c == TN_IAC) {               // IAC?
        tsta = TNS_IAC;                // change state
        break;
      }
      if ((c == TN_CR) && dstb)        // CR, no bin
        tsta = TNS_SKIP;               // skip next
      else
        return 1;                      // OK to output this char
      break;

    case TNS_IAC:                      // IAC prev
      if ((c == TN_IAC) & !dstb) {     // IAC + IAC, bin?
        tsta = TNS_NORM;               // treat as normal
        break;                         // keep IAC
      }
      if (c == TN_BRK) {               // IAC + BRK?
        tsta = TNS_NORM;               // treat as normal
        c = 0;                         // char is null
        // rbr = 1;                    // flag break
        break;
      }
      if (c == TN_WILL)                // IAC + WILL?
        tsta = TNS_WILL;
      else if (c == TN_WONT)           // IAC + WONT?
        tsta = TNS_WONT;
      else
        tsta = TNS_SKIP;               // IAC + other
      break;

    case TNS_WILL:
    case TNS_WONT:                     // IAC+WILL/WONT prev
      if (c == TN_BIN) {               // BIN?
        if (tsta == TNS_WILL)
          dstb = 0;
        else
          dstb = 1;
      }

    case TNS_SKIP:
    default:                           // skip char
      tsta = TNS_NORM;                 // next normal
      break;
  }

  return 0;
}
