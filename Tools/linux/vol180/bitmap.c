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
#include <time.h>

#include "fileio.h"
#include "buffer.h"
#include "bitmap.h"
#include "misc.h"

extern unsigned long nblocks;
extern unsigned long bmblock;
extern unsigned char clfactor;

/*-----------------------------------------------------------------------*/

/* Allocate single disk block: find free block in bitmap file, mark it
   as allocated and return its number. Returns zero if disk full */

unsigned long alloc_cluster(void) {
  struct BUFFER *buf;
  unsigned char bmp, mask;
  unsigned long i, cnt, bmblk;

  /* nblocks is obtained from volume id (second block of BOOT.SYS) */
  cnt = BMHDRSZ;  /* skip header */
  bmblk = bmblock;
  buf = get_block(bmblk);
  if (!buf) return 0;
  mask = 0x80;
  bmp = buf->data[cnt];
  for (i = 0; i < nblocks; ++i) {
    if ((bmp & mask) == 0) {
      /* mark this block as used */
      bmp |= mask;
      buf->data[cnt] = bmp;
      buf->modified = 1;
      release_block(buf);
#ifdef DEBUG
  printf("allocated cluster %lu\n", i);
#endif
      return i << clfactor;
    }
    mask >>= 1;
    if (mask == 0) {
      if (++cnt == 512) {
        release_block(buf);
        buf = get_block(++bmblk);
        if (!buf) return 0;
        cnt = 0;
      }
      bmp = buf->data[cnt];
      mask = 0x80;
    }
  }

  return 0;
}

/* Allocate contiguous disk space: find free contiguos blocks in bitmap
   file, mark them as allocated and return the number of the starting
   block. Returns zero on failure */

unsigned long alloc_clusters(unsigned long num) {
  struct BUFFER *buf;
  unsigned char bmp, mask;
  unsigned long i, j, cstart, cnt, bmblk;
  
  /* nblocks is obtained from volume id (second block of BOOT.SYS) */
  cstart = 0;     /* start of contiguous segment */
  j = 0;          /* contiguous block count */
  cnt = BMHDRSZ;  /* skip header */
  bmblk = bmblock;
  buf = get_block(bmblk);
  if (!buf) return 0;
  mask = 0x80;
  bmp = buf->data[cnt];
  for (i = 0; i < nblocks; ++i) {
    if ((bmp & mask) == 0) {
      /* free block, keep looking to see if we have enough in a row */
      if (j == 0) cstart = i;
      if (++j == num) break; /* found enough contiguous space */
    } else {
      /* block in use, reset search */
      j = 0;
    }
    mask >>= 1;
    if (mask == 0) {
      if (++cnt == 512) {
        release_block(buf);
        buf = get_block(++bmblk);
        if (!buf) return 0;
        cnt = 0;
      }
      bmp = buf->data[cnt];
      mask = 0x80;
    }
  }
  release_block(buf);

  if (j < num) return 0;
  
  /* now mark blocks as allocated */

  mask = (0x80 >> (cstart & 7));
  cnt = ((cstart >> 3) + BMHDRSZ) & 0x1FF;
  bmblk = bmblock + ((((cstart >> 3) + BMHDRSZ) >> 9) & 0xFFF);

  buf = get_block(bmblk);
  if (!buf) return 0;
  bmp = buf->data[cnt];
  /* TODO: check this!!! */
  for (i = 0; i < num; ++i) {
    /* mark this block as used */
    bmp |= mask;
    mask >>= 1;
    if (mask == 0) {
      buf->data[cnt] = bmp;
      buf->modified = 1;
      if (++cnt == 512) {
        release_block(buf);
        buf = get_block(++bmblk);
        if (!buf) return 0;
        cnt = 0;
      }
      bmp = buf->data[cnt];
      mask = 0x80;
    }
  }
  if (mask != 0x80) {
    buf->data[cnt] = bmp;
    buf->modified = 1;
  }
  release_block(buf);

#ifdef DEBUG
  printf("allocated cluster chain %lu (%ld)\n", cstart, num);
#endif
  return cstart << clfactor;
}

int free_cluster(unsigned long clno) {
  struct BUFFER *buf;
  unsigned char mask;
  unsigned long cnt, bmblk;

  mask = (0x80 >> (clno & 7));
  cnt = ((clno >> 3) + BMHDRSZ) & 0x1FF;
  bmblk = bmblock + ((cnt >> 9) & 0xFFF);
  
  buf = get_block(bmblk);
  if (!buf) return 0;
  
  buf->data[cnt] &= ~mask;
  buf->modified = 1;
  
  release_block(buf);

#ifdef DEBUG
  printf("freeing cluster %lu\n", clno);
#endif

  return 1;
}

/* Allocate inode: find free index file entry in bitmap file, mark it
   as allocated and return its number. Returns zero if no entries are free */

unsigned short alloc_inode(void) {
  struct BUFFER *buf;
  unsigned char bmp, mask;
  unsigned long i, cnt, bmblk, nfiles;

  bmblk = bmblock;
  buf = get_block(bmblk);
  if (!buf) return 0;
  
  bmblk += GET_INT24(buf->data, 8);  /* get VBN of index file bitmap */
  release_block(buf);
  buf = get_block(bmblk);
  if (!buf) return 0;

  nfiles = GET_INT16(buf->data, 0);
  cnt = BMHDRSZ;  /* skip header */
  mask = 0x80;
  bmp = buf->data[cnt];
  for (i = 0; i < nfiles; ++i) {
    if ((bmp & mask) == 0) {
      /* mark this entry as used */
      bmp |= mask;
      buf->data[cnt] = bmp;
      buf->modified = 1;
      release_block(buf);
#ifdef DEBUG
  printf("allocated inode %lu\n", i);
#endif
      return i + 1;  /* make it 1-based */
    }
    mask >>= 1;
    if (mask == 0) {
      if (++cnt == 512) {
        release_block(buf);
        buf = get_block(++bmblk);
        if (!buf) return 0;
        cnt = 0;
      }
      bmp = buf->data[cnt];
      mask = 0x80;
    }
  }

  return 0;
}

int free_inode(unsigned short ino) {
  struct BUFFER *buf;
  unsigned char mask;
  unsigned long cnt, bmblk, nfiles;
  
  --ino;  /* make it zero-based */

  bmblk = bmblock;
  buf = get_block(bmblk);
  if (!buf) return 0;
  
  bmblk += GET_INT24(buf->data, 8);  /* get VBN of index file bitmap */
  release_block(buf);
  buf = get_block(bmblk);
  if (!buf) return 0;

  nfiles = GET_INT16(buf->data, 0);
  release_block(buf);
  
  if (ino >= nfiles) return 0;

  mask = (0x80 >> (ino & 7));
  cnt = ((ino >> 3) + BMHDRSZ) & 0x1FF;
  bmblk += (cnt >> 9) & 0x1F;
  
  buf = get_block(bmblk);
  if (!buf) return 0;
  
  buf->data[cnt] &= ~mask;
  buf->modified = 1;
  
  release_block(buf);

#ifdef DEBUG
  printf("freeing inode %lu\n", clno);
#endif

  return 1;
}
