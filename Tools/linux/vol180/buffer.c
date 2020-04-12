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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "blockio.h"
#include "buffer.h"

//#define DEBUG

extern unsigned long nblocks;

/*-----------------------------------------------------------------------*/

#define NBUFFERS 30

static struct BUFFER buffer[NBUFFERS];

static int last_alloc = 0;

void dump_buf(struct BUFFER *buf) {  /* for debug */
  int i, c;
  unsigned addr;
  
  if (!buf) return;
          
  printf("blkno %lu, access count %d valid %d modified %d\n",
         buf->blkno, buf->access_cnt, buf->valid, buf->modified);
  for (addr = 0; addr < 512; addr += 16) {
    printf("%08lX: ", addr + buf->blkno * 512L);
    for (i = 0; i < 16; ++i)
      printf("%02X ", buf->data[addr+i]);
    printf("   ");
    for (i = 0; i < 16; ++i)
      c = buf->data[addr+i], fputc(((c >= 32) && (c < 127)) ? c : '.', stdout);
    printf("\n");
  }
}

void init_bufs(void) {
  memset(buffer, 0, sizeof(buffer));
}

void release_block(struct BUFFER *buf) {
#ifdef DEBUG
  if (buf && buf->access_cnt) printf("releasing block %lu\n", buf->blkno);
#endif
  if (buf) --buf->access_cnt;
}

void flush_block(struct BUFFER *buf) {
#ifdef DEBUG
  if (buf && buf->modified) printf("flushing block %lu\n", buf->blkno);
#endif
  if (buf && buf->modified) {
    write_block(buf->blkno, buf->data);
    buf->modified = 0;
  }
}

void flush_buffers(void) {
  int i;
  
  for (i = 0; i < NBUFFERS; ++i) flush_block(&buffer[i]);
}

struct BUFFER *get_block(unsigned long blkno) {
  int i, j;

#ifdef DEBUG
  printf("requesting block %lu, ", blkno);
#endif

  if (blkno >= nblocks) {
    printf("Block out of range: %ld (limit = %ld)\n", blkno, nblocks);
    return NULL;
  }

  for (i = 0; i < NBUFFERS; ++i) {
    if (buffer[i].valid && (buffer[i].blkno == blkno)) {
      ++buffer[i].access_cnt;
#ifdef DEBUG
      printf("returning existing:\n");
      dump_buf(&buffer[i]);
#endif
      return &buffer[i];
    }
  }
  j = last_alloc;
  for (i = 0; i < NBUFFERS; ++i) {
    if (++j == NBUFFERS) j = 0;
    if (buffer[j].access_cnt == 0) {
      flush_block(&buffer[j]);
      buffer[j].blkno = blkno;
      read_block(buffer[j].blkno, buffer[j].data);
      ++buffer[j].access_cnt;
      buffer[j].valid = 1;
      buffer[j].modified = 0;
#ifdef DEBUG
      printf("returning new:\n");
      dump_buf(&buffer[j]);
#endif
      last_alloc = j;
      return &buffer[j];
    }
  }

  return NULL;
}

struct BUFFER *new_block(unsigned long blkno) {
  int i, j;
  
#ifdef DEBUG
  printf("new block %lu\n", blkno);
#endif

  if (blkno >= nblocks) {
    printf("Block out of range: %ld (limit = %ld)\n", blkno, nblocks);
    return NULL;
  }

  for (i = 0; i < NBUFFERS; ++i) {
    if (buffer[i].valid && (buffer[i].blkno == blkno)) {
      memset(buffer[i].data, 0, 512);
      ++buffer[i].access_cnt;
      buffer[i].modified = 1;
      return &buffer[i];
    }
  }
  j = last_alloc;
  for (i = 0; i < NBUFFERS; ++i) {
    if (++j == NBUFFERS) j = 0;
    if (buffer[j].access_cnt == 0) {
      flush_block(&buffer[j]);
      buffer[j].blkno = blkno;
      memset(buffer[j].data, 0, 512);
      ++buffer[j].access_cnt;
      buffer[j].valid = 1;
      buffer[j].modified = 1;
      last_alloc = j;
      return &buffer[j];
    }
  }

  return NULL;
}
