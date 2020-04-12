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
#include "dirio.h"
#include "indexf.h"
#include "mkdisk.h"
#include "bitmap.h"
#include "misc.h"


/*-----------------------------------------------------------------------*/

int create_disk(char *fname, unsigned long nblocks, unsigned nfiles) {
  FILE *f;
  unsigned clsize, clfactor, bmsize, ixbmsize;
  unsigned long ixstblk, bmstblk, bmblocks, ixblocks, ixbmblocks, mdstblk;
  unsigned long nclusters, clmask, blkcnt;
  unsigned short dfprot;
  unsigned char  block[512], mask, bmask;
  int i, j;
  time_t now;
  
  if (nblocks < 128) {
    fprintf(stderr, "too few blocks for disk image \"%s\": %lu\n",
                    fname, nblocks);
    return 1;
  }

  if (nblocks > 4194304 /*8388608*/) {
    fprintf(stderr, "too many blocks for disk image \"%s\": %lu\n",
                    fname, nblocks);
    return 1;
  }
  
  if (nfiles < 32) {
    fprintf(stderr, "too few files for disk image \"%s\": %u\n",
                    fname, nfiles);
    return 1;
  }

  if ((nfiles > 65536) || (nfiles > nblocks / 2)) {
    fprintf(stderr, "too many files for disk image \"%s\": %u\n",
                    fname, nfiles);
    return 1;
  }
  
  if (nblocks <= 131072) {
    clfactor = 0;
  } else if (nblocks <= 524288) {
    clfactor = 1;
  } else if (nblocks <= 2097152) {
    clfactor = 2;
  } else {
    clfactor = 3;
  }
  
  clsize = (1 << clfactor);      /* in blocks */
  clmask = clsize - 1;
  nclusters = nblocks / clsize;
  nblocks = nclusters * clsize;  /* rounded to the lower cluster boundary */

  f = fopen(fname, "w");
  if (!f) {
    fprintf(stderr, "could not create disk image \"%s\": %s\n",
                    fname, strerror(errno));
    return 1;
  }
  
  dfprot = 0xFFF8;
  
  ixblocks = (nfiles + 7) / 8;

  bmsize = (nclusters + 7) / 8 + BMHDRSZ;  /* in bytes */
  bmblocks = (bmsize + 511) / 512;         /* in blocks */
  
  ixbmsize = (nfiles + 7) / 8 + BMHDRSZ;   /* index file bitmap follows */
  ixbmblocks = (ixbmsize + 511) / 512;

#define CLUSTER_ROUND(blk) (((blk + clsize - 1) / clsize) * clsize)
  ixstblk = CLUSTER_ROUND(2);
  bmstblk = CLUSTER_ROUND(ixstblk + ixblocks);
  mdstblk = CLUSTER_ROUND(bmstblk + bmblocks + ixbmblocks);

  blkcnt = nblocks;
  
  time(&now);

  /* write boot block */
  memset(block, 0, 512);
  fwrite(block, 1, 512, f); /* block 0 */
  --blkcnt;
  
  /* write volume id */
  memcpy(block, "VOL180", 6);
  block[8] = FVER_L;             /* filesystem version */
  block[9] = FVER_H;
  strcpy((char *) &block[16], "RSX180 DISK");  /* volume label */
  SET_INT24(block, 32, nblocks); /* disk size in blocks */
  SET_INT16(block, 36, dfprot);  /* default file protection */
  set_date(&block[40], now);     /* created timestamp */
  block[48] = clfactor;          /* cluster factor */
  SET_INT24(block, 64, ixstblk); /* starting index file block */
  SET_INT24(block, 68, bmstblk); /* starting bitmap block */
  SET_INT24(block, 72, 0);       /* starting system image block, none yet */
  fwrite(block, 1, 512, f); /* block 1 */
  --blkcnt;
  
  memset(block, 0, 512);
  for (i = 2; i < ixstblk; ++i) {
    fwrite(block, 1, 512, f); /* pad cluster */
    --blkcnt;
  }

  /* write the index file */
  memset(block, 0, 512);
  set_inode(&block[0],  1, _FA_FILE | _FA_CTG,     /* INDEXF.SYS */
            1, 1, ixblocks, ixblocks, 512, 0xCCC8);
  SET_INT24(block, 0+32, ixstblk);
  set_cdate(&block[0], now);
  set_mdate(&block[0], now);
  set_name(&block[0], "INDEXF", "SYS", 1);

  set_inode(&block[64], 1, _FA_FILE | _FA_CTG,     /* BITMAP.SYS */
            1, 1, bmblocks + ixbmblocks, bmblocks + ixbmblocks,
            ixbmsize & 0x1FF, 0xCCC8);
  set_cdate(&block[64], now);
  set_mdate(&block[64], now);
  SET_INT24(block, 64+32, bmstblk);
  set_name(&block[64], "BITMAP", "SYS", 1);

  set_inode(&block[128], 1, _FA_FILE,               /* BADBLK.SYS */
            1, 1, 0, 0, 0, 0xCCC8);
  set_cdate(&block[128], now);
  set_mdate(&block[128], now);
  set_name(&block[128], "BADBLK", "SYS", 1);

  set_inode(&block[192], 1, _FA_FILE | _FA_CTG,     /* BOOT.SYS */
            1, 1, 2, 2, 512, 0xCCC8);
  set_cdate(&block[192], now);
  set_mdate(&block[192], now);
  SET_INT24(block, 192+32, 0);
  set_name(&block[192], "BOOT", "SYS", 1);

  set_inode(&block[256], 1, _FA_DIR,               /* MASTER.DIR */
            1, 1, 2, 2, 512, 0xCCC8);
  set_cdate(&block[256], now);
  set_mdate(&block[256], now);
  SET_INT24(block, 256+32, mdstblk);
  if (clfactor == 0) {
    SET_INT24(block, 256+35, mdstblk + 1);
  }
  set_name(&block[256], "MASTER", "DIR", 1);

  set_inode(&block[320], 1, _FA_FILE | _FA_CTG,    /* CORIMG.SYS */
            1, 1, 0, 0, 0, 0xDDD8);
  set_cdate(&block[320], now);
  set_mdate(&block[320], now);
  set_name(&block[320], "CORIMG", "SYS", 1);

  set_inode(&block[384], 1, _FA_FILE | _FA_CTG,    /* SYSTEM.SYS */
            1, 1, 0, 0, 0, 0xDDD8);
  set_cdate(&block[384], now);
  set_mdate(&block[384], now);
  set_name(&block[384], "SYSTEM", "SYS", 1);

  fwrite(block, 1, 512, f); /* block ixstblk */
  --blkcnt;
  memset(block, 0, 512);
  for (i = 1; i < ixblocks; ++i) {
    fwrite(block, 1, 512, f); /* remaining index file blocks */
    --blkcnt;
  }

  for (i = ixstblk + ixblocks; i < bmstblk; ++i) {
    fwrite(block, 1, 512, f); /* pad cluster */
    --blkcnt;
  }

  /* write the bitmap file, blocks 0 to mdstblk+1 are already allocated
     to system files. */
  nclusters = (mdstblk + 1 + 1) >> clfactor;
  if ((mdstblk + 1 + 1) & clmask) ++nclusters;
  for (i = 0; i < bmblocks; ++i) {
    memset(block, 0, 512);
    if (i == 0) {
      SET_INT24(block, 0, nblocks);      /* set device size in header */
      SET_INT24(block, 8, bmblocks);     /* VBN of index file bitmap */
      block[4] = clfactor;
      j = 16;
    } else {
      j = 0;
    }
    if (nclusters > 0) {
      bmask = 0x00;
      mask = 0x80;
      for ( ; j < 512 && nclusters > 0; --nclusters) {
        bmask |= mask;
        mask >>= 1;
        if (mask == 0) {
          block[j++] = bmask;
          bmask = 0x00;
          mask = 0x80;
        }
      }
      if (mask != 0) block[j] = bmask;
    }
    fwrite(block, 1, 512, f);
    --blkcnt;
  }
  /* append the index file bitmap */
  nclusters = 7;  /* reusing nclusters as number of system files */
  for (i = 0; i < ixbmblocks; ++i) {
    memset(block, 0, 512);
    if (i == 0) {
      SET_INT24(block, 0, nfiles);       /* set max files in header */
      j = 16;
    } else {
      j = 0;
    }
    if (nclusters > 0) {
      bmask = 0x00;
      mask = 0x80;
      for ( ; j < 512 && nclusters > 0; --nclusters) {
        bmask |= mask;
        mask >>= 1;
        if (mask == 0) {
          block[j++] = bmask;
          bmask = 0x00;
          mask = 0x80;
        }
      }
      if (mask != 0) block[j] = bmask;
    }
    fwrite(block, 1, 512, f);
    --blkcnt;
  }
  for (i = bmstblk + bmblocks + ixbmblocks; i < mdstblk; ++i) {
    fwrite(block, 1, 512, f); /* pad cluster */
    --blkcnt;
  }

  /* write master directory */
  memset(block, 0, 512);
  set_dir_entry(&block[0],  1, "INDEXF", "SYS", 1);
  set_dir_entry(&block[16], 2, "BITMAP", "SYS", 1);
  set_dir_entry(&block[32], 3, "BADBLK", "SYS", 1);
  set_dir_entry(&block[48], 4, "BOOT",   "SYS", 1);
  set_dir_entry(&block[64], 5, "MASTER", "DIR", 1);
  set_dir_entry(&block[80], 6, "CORIMG", "SYS", 1);
  set_dir_entry(&block[96], 7, "SYSTEM", "SYS", 1);
  fwrite(block, 1, 512, f); /* block mdstblk + 1 */
  --blkcnt;
  memset(block, 0, 512);
  fwrite(block, 1, 512, f); /* block mdstblk + 2 */
  --blkcnt;
  
  /* fill the remaining of the disk with "formatted" bytes */
  while (blkcnt > 0) {
    memset(block, 0xE5, 512);
    fwrite(block, 1, 512, f); /* block n..end */
    --blkcnt;
  }
  
  fclose(f);

  return 0;
}
