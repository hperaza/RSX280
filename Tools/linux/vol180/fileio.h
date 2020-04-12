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

#ifndef __FILEIO_H
#define __FILEIO_H

/* file attributes */

#define _FA_DIR  0x80
#define _FA_FILE 0x01
#define _FA_CTG  0x08

struct FCBheader {
  struct FCBheader *next;
  unsigned int  usecnt;
  unsigned char attrib;
  char dirname[9], fname[9], ext[3];
  short vers;
  char user, group;
  unsigned short inode;
  unsigned short seqno;
  unsigned char  clfactor;
  unsigned short lnkcnt;
  unsigned long  nalloc;
  unsigned long  nused;
  unsigned short lbcount;
  unsigned long  bmap[6];
};

struct FCB {
  struct FCBheader *header;
  unsigned long  curalloc;    /* absolute current alloc block in allocbuf */
  unsigned long  curblk;      /* relative current block */
  unsigned short byteptr;     /* current block byte pointer */
};

void dump_alloc_map(struct FCB *fcb);
struct FCB *get_fcb(unsigned short ino);
void free_fcb(struct FCB *fcb);
char *get_file_name(struct FCB *fcb);
char *get_dir_name(struct FCB *fcb);
int parse_name(char *str, char *dirname, char *fname, char *ext, short *vers);
int file_seek(struct FCB *fcb, unsigned long pos);
unsigned long file_pos(struct FCB *fcb);
int file_read(struct FCB *fcb, unsigned char *buf, unsigned len);
int file_write(struct FCB *fcb, unsigned char *buf, unsigned len);
int end_of_file(struct FCB *fcb);
int close_file(struct FCB *fcb);
int set_file_dates(struct FCB *fcb, time_t created, time_t modified);
struct FCB *open_md_file(char *fname);
struct FCB *open_file(char *fname);
struct FCB *create_file(char *filename, char user, char group,
                        int contiguous, unsigned csize);
int delete_file(char *fname);

#endif
