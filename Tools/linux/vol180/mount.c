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
#include "fileio.h"
#include "blockio.h"
#include "mount.h"
#include "misc.h"


/*-----------------------------------------------------------------------*/

FILE *imgf = NULL;
unsigned long  img_offset = 0;

unsigned long  nblocks  = 0;
unsigned long  bmblock  = 0;
unsigned long  ixblock  = 0;
unsigned short defprot  = 0;
unsigned char  clfactor = 0;

struct FCB *mdfcb = NULL, *cdfcb = NULL;

int mount_disk(char *imgname) {
  unsigned char buf[512], vh, vl, *inode;

  dismount_disk();

  imgf = fopen(imgname, "r+");
  if (!imgf) {
    fprintf(stderr, "Could not open disk image \"%s\": %s\n",
                    imgname, strerror(errno));
    return 1;
  }

  init_bufs();

  /* read the volume id block */
  read_block(1, buf);
  
  if (strncmp((char *) buf, "VOL180", 6) != 0) {
    fprintf(stderr, "Invalid volume type\n");
    fclose(imgf);
    imgf = NULL;
    return 1;
  }
  
  vl = buf[8];
  vh = buf[9];
  
  if ((vh != FVER_H) || (vl != FVER_L)) {
    fprintf(stderr, "Invalid filesystem version\n");
    fclose(imgf);
    imgf = NULL;
    return 1;
  }
  
  nblocks  = GET_INT24(buf, 32);
  defprot  = GET_INT16(buf, 36);
  clfactor = buf[48];
  ixblock  = GET_INT24(buf, 64);
  bmblock  = GET_INT24(buf, 68);

  printf("\n");
  printf("Volume label:   %s\n", &buf[16]);
  printf("Created:        %s\n", timestamp_str(&buf[40]));
  printf("Version:        %d.%d\n", vh, vl);
  printf("Volume size:    %lu blocks (%lu bytes)\n", nblocks, nblocks * 512L);
  printf("Cluster factor: %d (%d block%s)\n", clfactor, (1 << clfactor), clfactor ? "s" : "");
  printf("\n");

  /* read the first block of the index file */
  read_block(ixblock, buf);

  /* open the master directory */
  inode = &buf[(5-1)*64];

  mdfcb = get_fcb(5);
  if (mdfcb->header->usecnt == 1) {
    mdfcb->header->attrib = inode[2];
    strncpy(mdfcb->header->dirname, "MASTER   ", 9);
    strncpy(mdfcb->header->fname, "MASTER   ", 9);
    strncpy(mdfcb->header->ext, "DIR", 3);
    mdfcb->header->vers = 1;
    mdfcb->header->user = inode[6];
    mdfcb->header->group = inode[7];
    mdfcb->header->clfactor = inode[3];
    mdfcb->header->lnkcnt = GET_INT16(inode, 0);
    mdfcb->header->seqno = GET_INT16(inode, 4);
    mdfcb->header->nalloc = GET_INT24(inode, 8);
    mdfcb->header->nused = GET_INT24(inode, 11);
    mdfcb->header->lbcount = GET_INT16(inode, 14);
    mdfcb->header->bmap[0] = GET_INT24(inode, 32);
    mdfcb->header->bmap[1] = GET_INT24(inode, 35);
    mdfcb->header->bmap[2] = GET_INT24(inode, 38);
    mdfcb->header->bmap[3] = GET_INT24(inode, 41);
    mdfcb->header->bmap[4] = GET_INT24(inode, 44);
    mdfcb->header->bmap[5] = GET_INT24(inode, 47);
  }
  mdfcb->curblk = 0;
  mdfcb->byteptr = 0;

  cdfcb = open_md_file("MASTER.DIR");
  
  return 0;
}

int dismount_disk(void) {

  if (cdfcb) { close_file(cdfcb); free_fcb(cdfcb); }
  cdfcb = NULL;
  if (mdfcb) { close_file(mdfcb); free_fcb(mdfcb); }
  mdfcb = NULL;

  flush_buffers();

  if (imgf) fclose(imgf);
  imgf = NULL;

  return 1;
}
