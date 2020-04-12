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

#ifndef __MAIN_H
#define __MAIN_H

int process_serial_char(char c);
int process_tcp_chars(char *c, int n, int line);
int send_serial_char(char c);
int send_tcp_char(char c, int line);

#endif  // __MAIN_H
