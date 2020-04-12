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
#include "indexf.h"
#include "dirio.h"
#include "misc.h"

extern struct FCB *mdfcb, *cdfcb;

/*-----------------------------------------------------------------------*/

void set_dir_entry(unsigned char *entry, unsigned short inode,
                   char *fname, char *ext, short vers) {
  int i;

  SET_INT16(entry, 0, inode);
  for (i = 0; i < 9; ++i) entry[i+2] = *fname ? *fname++ : ' ';
  for (i = 0; i < 3; ++i) entry[i+11] = *ext   ? *ext++   : ' ';
  SET_INT16(entry, 14, vers);
}

int match(unsigned char *dirent, char *fname, char *ext, short vers) {
  int  i;
  char *p;
  
  p = fname;
  for (i = 0; i < 9; ++i) {
    if (!*p && (dirent[i+2] == ' ')) break;
    if (dirent[i+2] != *p++) return 0;
  }
  
  p = ext;
  for (i = 0; i < 3; ++i) {
    if (!*p && (dirent[i+11] == ' ')) break;
    if (dirent[i+11] != *p++) return 0;
  }
  
  if (vers > 0) return (GET_INT16(dirent, 14) == vers);

  return 1;
}

int match_fcb(unsigned char *dirent, struct FCB *fcb) {
  int i;
  
  for (i = 0; i < 9; ++i) {
    if (dirent[i+2] != fcb->header->fname[i]) return 0;
  }
  for (i = 0; i < 3; ++i) {
    if (dirent[i+11] != fcb->header->ext[i]) return 0;
  }
  if (GET_INT16(dirent, 14) != fcb->header->vers) return 0;

  return 1;
}

int create_dir(char *filename, char group, char user) {
  unsigned char dirent[16];
  unsigned char inode[64];
  char fname[13], *ext, *pvers;
  unsigned short ino;
  short vers;
  unsigned long fpos;
  time_t now;

  if (!mdfcb) return 0;

  strncpy(fname, filename, 13);
  fname[13] = '\0';
  
  ext = strchr(fname, '.');
  if (ext) {
    *ext++ = '\0';
  } else {
    ext = "DIR";
  }

  pvers = strchr(ext ? ext : fname, ';');
  if (pvers) {
    *pvers++ = '\0';
  }
  vers = 1;  /* TODO: do not allow duplicate or new version of dirs */

  /* find a free inode */
  ino = new_inode();
  if (ino == 0) {
    fprintf(stderr, "Index file full\n");
    return 0;
  }
  if (read_inode(ino, inode) == 0) return 0; /* panic */
  if (GET_INT16(inode, 0) != 0) return 0; /* panic */

  /* find a free directory entry */
  file_seek(mdfcb, 0L);
  for (;;) {
    fpos = file_pos(mdfcb);
    if (file_read(mdfcb, dirent, 16) == 16) {
      if (GET_INT16(dirent, 0) == 0) break;  /* free dir entry */
    } else {
      break; /* at the end of directory */
    }
  }

  time(&now);
  
  set_inode(inode, 1, _FA_DIR, group, user, 0, 0, 0, 0xFFF8);
  SET_INT24(inode, 32, 0);
  set_cdate(inode, now);
  set_mdate(inode, now);
  set_name(inode, fname, ext, vers);
  write_inode(ino, inode);
  set_dir_entry(dirent, ino, fname, ext, vers);
  file_seek(mdfcb, fpos);
  file_write(mdfcb, dirent, 16);

  return 1;
}

int change_dir(char *filename) {
  struct FCB *fcb;

  fcb = open_md_file(filename);
  if (!fcb) return 0;

  if (!(fcb->header->attrib & _FA_DIR)) {
    /* not a directory */
    free_fcb(fcb);
    return 0;
  }

  if (cdfcb) {
    close_file(cdfcb);
    free_fcb(cdfcb);
  }
  cdfcb = fcb;
  
  return 1;
}
