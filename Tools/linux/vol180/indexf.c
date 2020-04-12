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

#include "buffer.h"
#include "bitmap.h"
#include "indexf.h"
#include "misc.h"

extern unsigned long ixblock;
extern unsigned char clfactor;

/*-----------------------------------------------------------------------*/

void set_inode(unsigned char *entry, unsigned short lnkcnt,
               char attrib, char group, char user,
               unsigned long nalloc, unsigned long nused,
               unsigned short lbcount, unsigned short perm) {
  unsigned short seqno;
  
  SET_INT16(entry, 0, lnkcnt);
  entry[2] = attrib;
  entry[3] = clfactor;

  seqno = GET_INT16(entry, 4);
  ++seqno;
  SET_INT16(entry, 4, seqno);

  entry[6] = user;
  entry[7] = group;

  SET_INT24(entry,  8, nalloc);
  SET_INT24(entry, 11, nused);
  SET_INT16(entry, 14, lbcount);
  SET_INT16(entry, 30, perm);
}

int read_inode(unsigned short num, unsigned char *entry) {
  unsigned long  blkno;
  unsigned short offset;
  struct BUFFER  *buf;

  --num;  /* inodes are 1-based */
  blkno = ixblock + (num / 8);
  offset = (num % 8) * 64;
  buf = get_block(blkno);
  if (!buf) return 0;
  
  memcpy(entry, &buf->data[offset], 64);
  release_block(buf);
  
  return 1;
}

int write_inode(unsigned short num, unsigned char *entry) {
  unsigned long  blkno;
  unsigned short offset;
  struct BUFFER  *buf;

  --num;  /* inodes are 1-based */
  blkno = ixblock + (num / 8);
  offset = (num % 8) * 64;
  buf = get_block(blkno);
  if (!buf) return 0;
  
  memcpy(&buf->data[offset], entry, 64);
  buf->modified = 1;
  release_block(buf);
  /* TODO: update modified timestamp of INDEXF.SYS file (inode 0)? */
  
  return 1;
}

int new_inode(void) {
  unsigned char  inode[64];
  unsigned short i;

  for (;;) {
    i = alloc_inode();
    if (i == 0) return 0;
    if (read_inode(i, inode) == 0) return 0;
    if (GET_INT16(inode, 0) == 0) return i;  /* paranoia check */
  }
  return 0;  /* index file full */
}

void set_date(unsigned char *buf, time_t t) {
  struct tm *tms;
  char temp[16];
  int  i;

  tms = localtime(&t);
  
  snprintf(temp, 15, "%04d%02d%02d%02d%02d%02d", tms->tm_year + 1900,
                    tms->tm_mon + 1, tms->tm_mday,
                    tms->tm_hour, tms->tm_min, tms->tm_sec);

  for (i = 0; i < 7; ++i) {
    buf[i] = ((temp[2*i] - '0') << 4) + (temp[2*i+1] - '0');
  }
}

void set_cdate(unsigned char *entry, time_t t) {
  set_date(&entry[16], t);
}

void set_mdate(unsigned char *entry, time_t t) {
  set_date(&entry[23], t);
}

void set_name(unsigned char *entry, char *fname, char *ext, unsigned short vers) {
  int i;

  for (i = 0; i < 9; ++i) entry[i+50] = *fname ? *fname++ : ' ';
  for (i = 0; i < 3; ++i) entry[i+59] = *ext   ? *ext++   : ' ';
  entry[62] = vers & 0xFF;
  entry[63] = (vers >> 8) & 0xFF;
}

/* Return a string representing the original file name stored in the node */
char *get_name(unsigned char *entry) {
  static char str[30];
  unsigned short vers;
  int i;
  char *p;
  
  p = str;
  for (i = 0; i < 9; ++i) if (entry[50+i] && (entry[50+i] != ' ')) *p++ = entry[50+i];
  *p++ = '.';
  for (i = 0; i < 3; ++i) if (entry[59+i] && (entry[59+i] != ' ')) *p++ = entry[59+i];
  *p++ = ';';
  vers = GET_INT16(entry, 62);
  snprintf(p, 5, "%d", vers);

  return str;
}

void dump_inode(unsigned short num) {
  unsigned long  blkno;
  unsigned short offset;
  unsigned char  *entry;
  struct BUFFER  *buf;

  --num;  /* inodes are 1-based */
  blkno = ixblock + (num / 8);
  offset = (num % 8) * 64;

  printf("Index File entry %d (VBN %06lXh, offset %04Xh):\n",
         num + 1, blkno - ixblock, offset);

  buf = get_block(blkno);
  if (!buf) {
    printf("Unable to read block\n");
    return;
  }

  entry = &buf->data[offset];
  
  printf("\n");
  
  printf("  Link count             %u\n", GET_INT16(entry, 0));
  printf("  Attributes             %02Xh\n", entry[2]);
  printf("  Seq number             %u\n", GET_INT16(entry, 4));
  printf("  User ID                %u\n", entry[6]);
  printf("  Group ID               %u\n", entry[7]);
  printf("  Cluster factor         %d\n", entry[3]);
  printf("  Allocated blocks       %u\n", GET_INT24(entry, 8));
  printf("  Used blocks            %u\n", GET_INT24(entry, 11));
  printf("  Last block byte count  %u\n", GET_INT16(entry, 14));
  printf("  File created           %s\n", timestamp_str(&entry[16]));
  printf("  Last modified          %s\n", timestamp_str(&entry[23]));
  printf("  Access permissions     %04Xh\n", GET_INT16(entry, 30));
  printf("  First block            %06Xh\n", GET_INT24(entry, 32));
  printf("  Original file name     %s\n", get_name(entry));
  
  printf("\n");

  release_block(buf);
}
