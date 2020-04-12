/***********************************************************************

   This file is part of vol180, an utility to handle RSX180 volumes.
   Copyright (C) 2008-2020, Hector Peraza.

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

#ifndef __BUFFER_H
#define __BUFFER_H

struct BUFFER {
  unsigned long blkno;
  unsigned char access_cnt, modified, valid;
  unsigned char data[512];
};

void init_bufs(void);
void release_block(struct BUFFER *buf);
void flush_block(struct BUFFER *buf);
void flush_buffers(void);
struct BUFFER *get_block(unsigned long blkno);
struct BUFFER *new_block(unsigned long blkno);

#endif
