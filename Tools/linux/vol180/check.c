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
#include <ctype.h>
#include <time.h>

#include "check.h"
#include "indexf.h"
#include "fileio.h"
#include "dirio.h"
#include "blockio.h"
#include "buffer.h"
#include "bitmap.h"
#include "misc.h"

//#define DEBUG

unsigned long  nblocks  = 0;  /* total disk blocks */
unsigned long  bmblock  = 0;  /* bitmap file starting block number */
unsigned long  ixblock  = 0;  /* index file starting block number */
unsigned short defprot  = 0;  /* default file protection */
unsigned char  clfactor = 0;  /* cluster factor */

struct FCB *mdfcb = NULL, *cdfcb = NULL;

FILE *imgf = NULL;
unsigned long img_offset = 0;

static unsigned long  ixblks = 0;  /* *current* index file size in blocks */
static unsigned short inodes = 0;  /* inode capacity of index file */
static unsigned long  bmsize = 0;  /* bitmap file size in bytes */
static unsigned long  bmblks = 0;  /* bitmap file size in blocks */

static int errcnt;
static int read_only = 0;

static unsigned char vh, vl;

/*-----------------------------------------------------------------------*/

void usage(char *p) {
  fprintf(stderr, "usage: %s [-n] filename [offset]\n", p);
}

int main(int argc, char *argv[]) {
  char *name, *mode, *p;
  
  if (argc < 2) {
    usage(argv[0]);
    return 1;
  }

  name = argv[1];
  mode = "r+b";
  p = NULL;
  if (strcmp(argv[1], "-n") == 0) {
    if (argc < 3) {
      usage(argv[0]);
      return 1;
    }
    read_only = 1;
    name = argv[2];
    mode = "rb";
    if (argc > 3) p = argv[3];
  } else {
    if (argc > 2) p = argv[2];
  }

  if (p) {
    if (*p == '+') {
      img_offset = atol(p+1);
    } else {
      img_offset = atol(p) * 512L;
    }
  }
  
  imgf = fopen(name, mode);
  if (!imgf) {
    fprintf(stderr, "%s: could not open %s\n", argv[0], argv[1]);
    return 1;
  }
  
  init_bufs();
  
  check();

//  if (cdfcb) { close_file(cdfcb); free(cdfcb); }
//  cdfcb = NULL;

  if (mdfcb) { close_file(mdfcb); free(mdfcb); }
  mdfcb = NULL;
  cdfcb = NULL;

  flush_buffers();

  if (imgf) fclose(imgf);
  imgf = NULL;
  
  return 0;
}

/*-----------------------------------------------------------------------*/

int check(void) {

  errcnt = 0;

  printf("1. Checking Volume ID\n");
  if (!check_volume_id()) return 0;

  printf("2. Checking Index File\n");
  if (!check_index_file()) return 0;

  printf("3. Checking Master Directory\n");
  if (!check_master_dir()) return 0;

  printf("4. Checking Files and Directories\n");
  if (!check_directories()) return 0;

  printf("5. Checking Storage Allocation Bitmap\n");
  if (!check_alloc_map()) return 0;
  
  if (errcnt == 0) {
    printf("No errors were found.\n");
  } else {
    if (read_only) {
      printf("%d error%s found.\n", errcnt, (errcnt == 1) ? "" : "s");
    } else {
      printf("%d error%s fixed.\n", errcnt, (errcnt == 1) ? "" : "s");
    }
  }
  
  return 1;
}

/* Check Volume ID block */
int check_volume_id(void) {
  unsigned char buf[512], *inode;
  
  /*
   * Volume ID check:
   * 1) ensure block 0 passes P112 checksum test (if bootable)
   * 2) ensure Volume ID is valid on block 1, else abort operation
   * 3) ensure filesystem version number is valid, else abort operation
   */
  
  /* read the volume ID block */
  read_block(1, buf);

  /* set variables to be used by this and other routines */
  nblocks  = GET_INT24(buf, 32);
  defprot  = GET_INT16(buf, 36);
  clfactor = buf[48];
  ixblock  = GET_INT24(buf, 64);
  bmblock  = GET_INT24(buf, 68);
  
  if (strncmp((char *) buf, "VOL180", 6) != 0) {
    /* this can happen only in the following situations:
     * 1) the user may be trying to fix a non-RSX180 disk or partition
     * 2) the volume ID block got somehow overwritten
     * we can't proceed in case 1 (obviously) because there is no valid
     * disk structure and the user may lose valuable data;
     * we can't proceed in case 2 either because pointers to important
     * system files have been lost (manually recovery with a disk editor
     * still possible).
     */
    printf("*** Invalid volume signature, aborting.\n");
    return 0;
  }

  vl = buf[8];
  vh = buf[9];  

  if ((vh != FVER_H) || (vl != FVER_L)) {
    printf("*** Invalid filesystem version, aborting.\n");
    return 0;
  }
  
  /* check nblocks */
//  if ((nblocks == 0) || (nblocks > dev_blocks) {
//  }
  
  /* check ixblock */
  if ((ixblock < 2) || (ixblock >= nblocks)) {
  }

  /* check bmblock */
  if ((bmblock < 2) || (bmblock >= nblocks)) {
  }
  
  /* save changes back */
  if (!read_only) write_block(1, buf);
  
  /* read the first block of the index file */
  read_block(ixblock, buf);

  /* open the master directory, so we can use later on the file I/O routines */
  /* basically, we're 'mounting' the volume at this point */
  inode = &buf[(5-1)*64]; /* assumes MASTER.DIR has not moved! */
  mdfcb = get_fcb(5);
  if (mdfcb->header->usecnt == 1) {
    mdfcb->header->attrib = inode[2];
    strncpy(mdfcb->header->fname, "MASTER   ", 9);
    strncpy(mdfcb->header->ext, "DIR", 3);
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
  mdfcb->curalloc = 0;
  mdfcb->curblk = 0;
  mdfcb->byteptr = 0;

  cdfcb = mdfcb;

  return 1;
}

/* Check index file */
int check_index_file(void) {
  unsigned char  inode[64], attrib;
  unsigned short lcnt, lbcnt, fid;
  unsigned long  blkno, nalloc, nused, nclusters, ixbmsize, ixbmblks;
  int i;
//  char s[80];
  
  /*
   * Index file check:
   * 1) check reserved inodes for valid data (block number values, etc.)
   * 2) check all other inodes for valid data:
   *    - check contiguous files for valid block numbers and ranges (i.e.
   *      ensure they fall within disk limits.)
   *    - for non-contiguous files check allocation maps, etc.
   *    - .DIR files in [MASTER] should have the 'dir' attrib set.
   * [Move to file and directory check:]
   * 3a) build an array with link count of all inode entries in index file.
   * 3b) scan all directories and compare inode number of every valid (used)
   *     dir entry with array built in 3a). If value > 1, --value. If value
   *     == 0, report the filename on dir entry and mark the directory entry
   *     as deleted.
   * 3c) check the array for any remaining values > 0. For each one, create
   *     an entry in [MASTER]LOSTFOUND.DIR. If value > 1, force value = 1.
   */

  if (!read_inode(1, inode)) {  /* INDEXF.SYS */
    printf("*** Could not read inode 1, aborting.\n");
    return 0;
  }
  lcnt   = GET_INT16(inode, 0);
  attrib = inode[2];
  nalloc = GET_INT24(inode, 8);
  nused  = GET_INT24(inode, 11);
  lbcnt  = GET_INT16(inode, 14);
  if (lcnt == 0) {
    printf("*** INDEXF.SYS has a link count of 0, fixing.\n");
    inode[0] = 1;
    inode[1] = 0;
    ++errcnt;
  }
  blkno = GET_INT24(inode, 32);
  if (blkno != ixblock) {
    /* which one to use? */
  }
  if (nalloc != nused) {
  }
  ixblks = nused;
  inodes = ixblks * 8;  /* there are 8 inodes in a block */

  fid = 2;  /* BITMAP.SYS */
  if (!read_inode(fid, inode)) {
    printf("*** Could not read inode %d, aborting.\n", fid);
    return 0;
  }
  lcnt   = GET_INT16(inode, 0);
  attrib = inode[2];
  nalloc = GET_INT24(inode, 8);
  nused  = GET_INT24(inode, 11);
  lbcnt  = GET_INT16(inode, 14);
  if (lcnt == 0) {
    printf("*** BITMAP.SYS has a link count of 0, fixing.\n");
    SET_INT16(inode, 0, 1);
    ++errcnt;
  }
  blkno = GET_INT24(inode, 32);
  if (blkno != bmblock) {
    /* which one to use? */
  }
  /* compute nused from disk size and compare with stored value */
  nclusters = nblocks >> clfactor;
  bmsize = (nclusters + 7) / 8 + BMHDRSZ;
  bmblks = (bmsize + 511) / 512;
  ixbmsize = (inodes + 7) / 8 + BMHDRSZ;
  ixbmblks = (ixbmsize + 511) / 512;
  if (nalloc != bmblks + ixbmblks) {
    printf("*** Wrong bitmap file size, fixing.\n");
    SET_INT24(inode, 8, bmblks + ixbmblks);
    ++errcnt;
  }
  /* TODO: check also lbcnt! */
  if (nused != nalloc) {
    inode[11] = inode[8];
    inode[12] = inode[9];
    inode[13] = inode[10];
  }
  if (!read_only) {
    if (!write_inode(fid, inode)) {
      printf("*** Could not write inode %d, aborting.\n", fid);
      return 0;
    }
  }
  
  ++fid;  /* BADBLK.SYS */
  ++fid;  /* BOOT.SYS */
  if (!read_inode(fid, inode)) {
    printf("*** Could not read inode %d, aborting.\n", fid);
    return 0;
  }
  lcnt   = GET_INT16(inode, 0);
  attrib = inode[2];
  nalloc = GET_INT24(inode, 8);
  nused  = GET_INT24(inode, 11);
  lbcnt  = GET_INT16(inode, 14);
  if (lcnt == 0) {
    printf("*** BOOT.SYS has a link count of 0, fixing.\n");
    inode[0] = 1;
    inode[1] = 0;
    ++errcnt;
  }
  blkno = GET_INT24(inode, 32);
  if (blkno != 0) {
    inode[32] = 0;
    inode[33] = 0;
    inode[34] = 0;
  }
  if (nalloc != 2) {
    printf("*** BOOT.SYS allocated block count is wrong, fixing.\n");
    inode[8]  = 2;
    inode[9]  = 0;
    inode[10] = 0;
    ++errcnt;
  }
  if (nused != nalloc) {
    printf("*** BOOT.SYS used block count is wrong, fixing.\n");
    inode[11] = inode[8];
    inode[12] = inode[9];
    inode[13] = inode[10];
    ++errcnt;
  }
  if (!read_only) {
    if (!write_inode(fid, inode)) {
      printf("*** Could not write inode %d, aborting.\n", fid);
      return 0;
    }
  }

  ++fid;  /* MASTER.DIR */
  if (!read_inode(fid, inode)) {
    printf("*** Could not read inode %d, aborting.\n", fid);
    return 0;
  }
  lcnt  = inode[0] | (inode[1] << 8);
  if (lcnt == 0) {
    printf("*** MASTER.DIR has a link count of 0, fixing.\n");
    inode[0] = 1;
    inode[1] = 0;
    ++errcnt;
  }
  
  /* TODO: check the inode bitmap! */
  
  for (i = 0; i < inodes; ++i) {
    if (!read_inode(i+1, inode)) {
      printf("*** Could not read inode %d, aborting.\n", i+1);
      return 0;
    }
    lcnt   = GET_INT16(inode, 0);
    attrib = inode[2];
    nalloc = GET_INT24(inode, 8);
    nused  = GET_INT24(inode, 11);
    lbcnt  = GET_INT16(inode, 14);
    if (lcnt > 0) {
      if (attrib & _FA_CTG) {
        blkno = GET_INT24(inode, 32);
        /* contiguous file */
        if (blkno >= nblocks) {
          printf("*** Inode %d (%s): invalid starting block number of contiguous file, deleting.\n",
                 i+1, get_name(inode));
          /* invalid starting block number, delete the file */
          lcnt = 0;
          ++errcnt;
        } else {
          if (blkno + nalloc > nblocks) {
            /* truncate the file */
            nalloc = nblocks - blkno - 1;
            if (nalloc == 0) {
              /* delete the file if nalloc became 0 */
              lcnt = 0;
              printf("*** Inode %d (%s): contiguous file outside of volume limits, truncating.\n",
                     i+1, get_name(inode));
            } else {
              printf("*** Inode %d (%s): contiguous file extends beyond end of volume, truncating.\n",
                     i+1, get_name(inode));
            }
            ++errcnt;
          }
          if (nused > nalloc) {
            printf("*** Inode %d (%s): wrong block count of contiguous file, fixing.\n",
                   i+1, get_name(inode));
            nused = nalloc;
            ++errcnt;
          }
        }
      } else {
        /* non-contiguous file */
        blkno = GET_INT24(inode, 32);
        /* !!!!! TODO: 5.0 !!!!! */
        if (blkno == 0) {
          if (nalloc > 0) {
            /* nalloc must be zero if blkno is zero */
            /* delete the file */
            printf("*** Inode %d (%s): null allocation block on non-empty file, deleting.\n",
                   i+1, get_name(inode));
            lcnt = 0;
            ++errcnt;
          }
        } else if ((blkno < 2) || (blkno >= nblocks)) {
          /* invalid starting allocation block number, delete the file; */
          /* any lost blocks will be recovered in phase 5 */
          printf("*** Inode %d (%s): invalid allocation block number, deleting.\n",
                 i+1, get_name(inode));
          lcnt = 0;
          ++errcnt;
        }
      }
      if (lbcnt > 512) {
        printf("*** Inode %d (%s): last block byte count larger than block size, truncating.\n",
               i+1, get_name(inode));
        lbcnt = 512;
        ++errcnt;
      }
      SET_INT16(inode,  0, lcnt);
      SET_INT24(inode,  8, nalloc);
      SET_INT24(inode, 11, nused);
      SET_INT16(inode, 14, lbcnt);
      if (!read_only) {
        if (!write_inode(i+1, inode)) {
          printf("*** Could not write inode %d, aborting.\n", i+1);
          return 0;
        }
      }
    }
  }
  
  return 1;
}

/* Check master directory */
int check_master_dir(void) {
  int vifound = 0, ixfound = 0, bmfound = 0, bbfound = 0, mdfound = 0;
  unsigned char dirent[16];
  unsigned short ino;
  unsigned long fpos;

  /*
   * Master directory check:
   * 1) ensure the boot, index, bitmap and master directory file exist.
   * 2) check the boot and label/home block, ensure location is correct
   *    in the special master directory files, and ensure that the location
   *    of the special files is correct in the home block.
   */

  /* ensure boot, index, bitmap, badblk and master dir files exist */
  file_seek(mdfcb, 0L);
  for (;;) {
    fpos = file_pos(mdfcb);
    if (file_read(mdfcb, dirent, 16) != 16) break; /* EOF */
    ino = GET_INT16(dirent, 0);
    switch (ino) {
      case 1:
        ixfound = 1;
        break;
      
      case 2:
        bmfound = 1;
        break;
        
      case 3:
        bbfound = 1;
        break;
        
      case 4:
        vifound = 1;
        break;

      case 5:
        mdfound = 1;
        if (!match(dirent, "MASTER", "DIR", 1)) {
          printf("*** MASTER.DIR entry has wrong name, restoring.\n");
          file_seek(mdfcb, fpos);
          set_dir_entry(dirent, ino, "MASTER", "DIR", 1);
          if (!read_only) {
            if (file_write(mdfcb, dirent, 16) != 16) {
              printf("*** Could not enter file into Master Directory, aborting.\n");
              return 0;
            }
          }
          ++errcnt;
        }
        break;
    }
  }

  file_seek(mdfcb, fpos);
  ino = 1;
  if (!ixfound) {
    printf("*** INDEXF.SYS entry not found in Master Directory, restoring.\n");
    set_dir_entry(dirent, ino, "INDEXF", "SYS", 1);
    if (!read_only) {
      if (file_write(mdfcb, dirent, 16) != 16) {
        printf("*** Could not enter file into Master Directory, aborting.\n");
        return 0;
      }
    }
    ++errcnt;
  }
  ++ino;
  if (!bmfound) {
    printf("*** BITMAP.SYS entry not found in Master Directory, restoring.\n");
    set_dir_entry(dirent, ino, "BITMAP", "SYS", 1);
    if (!read_only) {
      if (file_write(mdfcb, dirent, 16) != 16) {
        printf("*** Could not enter file into Master Directory, aborting.\n");
        return 0;
      }
    }
    ++errcnt;
  }
  ++ino;
  if (!bbfound) {
    printf("*** BADBLK.SYS entry not found in Master Directory, restoring.\n");
    set_dir_entry(dirent, ino, "BADBLK", "SYS", 1);
    if (!read_only) {
      if (file_write(mdfcb, dirent, 16) != 16) {
        printf("*** Could not enter file into Master Directory, aborting.\n");
        return 0;
      }
    }
    ++errcnt;
  }
  ++ino;
  if (!vifound) {
    printf("*** BOOT.SYS entry not found in Master Directory, restoring.\n");
    set_dir_entry(dirent, ino, "BOOT", "SYS", 1);
    if (!read_only) {
      if (file_write(mdfcb, dirent, 16) != 16) {
        printf("*** Could not enter file into Master Directory, aborting.\n");
        return 0;
      }
    }
    ++errcnt;
  }
  ++ino;
  if (!mdfound) {
    printf("*** MASTER.DIR entry not found in Master Directory, restoring.\n");
    set_dir_entry(dirent, ino, "MASTER", "DIR", 1);
    if (!read_only) {
      if (file_write(mdfcb, dirent, 16) != 16) {
        printf("*** Could not enter file into Master Directory, aborting.\n");
        return 0;
      }
    }
    ++errcnt;
  }
  
  /* TODO: ensure bitmap and master dir block info in directory matches values
     in volume ID block */
  
  return 1;
}

static char *file_name(unsigned char *dirent) {
  static char str[20];
  int i;
  unsigned short vers;
  char *p;
  
  p = str;
  for (i = 0; i < 9; ++i) if (dirent[2+i] != ' ') *p++ = dirent[2+i];
  *p++ = '.';
  for (i = 0; i < 3; ++i) if (dirent[11+i] != ' ') *p++ = dirent[11+i];
  *p++ = ';';
  vers = GET_INT16(dirent, 14);
  snprintf(p, 3, "%d", vers);

  return str;
}

static unsigned char valid_char(unsigned char c) {
  static char *invalid = "%?*:;.,()<>[]{}/\\|";
  c = toupper(c);
  if (strchr(invalid, c)) c = '_';
  return c;
}

static int valid_name(unsigned char *dirent) {
  int i, space, valid;
  unsigned char c;
  
  valid = 1;
  space = -1;
  for (i = 0; i < 9; ++i) {
    c = dirent[2+i];
    if (c == ' ') {
      if (space < 0) space = i;
      continue;
    } else {
      if (space >= 0) {
        /* found invalid embedded space */
        i = space;  /* restart from there */
        space = -1;
        dirent[2+i] = '_';
        valid = 0;
      } else {
        /* check for valid char */
        c = valid_char(c);
        if (c != dirent[2+i]) {
          dirent[2+i] = c;
          valid = 0;
        }
      }
    }
  }
#if 0
  if (space == 0) {
    dirent[2] = '_';
    valid = 0;
  }
#endif
    
  space = -1;
  for (i = 0; i < 3; ++i) {
    c = dirent[11+i];
    if (c == ' ') {
      if (space < 0) space = i;
      continue;
    } else {
      if (space >= 0) {
        /* found invalid embedded space */
        i = space;  /* restart from there */
        space = -1;
        dirent[11+i] = '_';
        valid = 0;
      } else {
        /* check for valid char */
        c = valid_char(c);
        if (c != dirent[11+i]) {
          dirent[11+i] = c;
          valid = 0;
        }
      }
    }
  }
#if 0
  if (space == 0) {
    dirent[11] = '_';
    valid = 0;
  }
#endif
  
  if (!valid) {
    printf("*** Invalid filename in directory entry, changed to %s\n", file_name(dirent));
    ++errcnt;
  }

  return valid;
}

/* Check a single directory */
static int check_directory(char *filename, unsigned short *ixmap) {
  unsigned char dirent[16], inode[64], attrib;
  struct FCB *fcb;
  unsigned short ino, lcnt;
  unsigned long fpos;
  int save;
  
  fcb = open_file(filename);
  if (!fcb) {
    printf("*** Could not open %s directory, aborting.\n", filename);
    return 0;
  }

  /* check filenames for valid characters, illegal embedded spaces,
     duplicate version numbers, etc. */
  file_seek(fcb, 0L);
  for (;;) {
    fpos = file_pos(fcb);
    if (file_read(fcb, dirent, 16) != 16) break; /* EOF */
    ino = GET_INT16(dirent, 0);
    if (ino == 0) continue; /* deleted entry */
    save = 0;
    if (ino >= inodes) {
      printf("*** %s on %s has invalid inode number, deleting.\n",
             file_name(dirent), filename);
      dirent[0] = 0;
      dirent[1] = 0;
      save = 1;
      ++errcnt;
    } else {
      if (!read_inode(ino, inode)) {
        printf("*** Could not read inode %d, aborting.\n", ino);
        return 0;
      }
      lcnt = GET_INT16(inode, 0);
      if (lcnt == 0) {
        /* TODO: check that inode blocks are released in bitmap; if not,
           recover the file? */
        printf("*** File %s has a link count of zero, deleting.\n", file_name(dirent));
        dirent[0] = 0;
        dirent[1] = 0;
        save = 1;
        ++errcnt;
      } else {
        ++ixmap[ino-1];
        attrib = inode[2];
        if (attrib & _FA_DIR) {
          /* nested dirs allowed by the specs, but not supported by the
             standard SYSFCP */
        }
        save |= !valid_name(dirent);
      }
    }
    if (save && !read_only) {
      file_seek(fcb, fpos);
      if (file_write(fcb, dirent, 16) != 16) {
        printf("*** Could not update entry in Master Directory, aborting.\n");
        return 0;
      }
    }
  }
     
  return 1;
}
  
/* Scan the Master Directory and check all directories */
int check_directories(void) {
  unsigned char dirent[16], inode[64], attrib;
  unsigned short i, ino, lcnt, *ixmap;
  unsigned long fpos;
  int save;
  
  ixmap = calloc(inodes, sizeof(unsigned short));
  if (!ixmap) {
    printf("*** Not enough memory, aborting.\n");
    return 0;
  }
  
  /* open each directory and check filenames for valid characters,
     illegal embedded spaces, duplicate version numbers, etc. */
  file_seek(mdfcb, 0L);
  for (;;) {
    fpos = file_pos(mdfcb);
    if (file_read(mdfcb, dirent, 16) != 16) break; /* EOF */
    ino = GET_INT16(dirent, 0);
    if (ino == 0) continue; /* deleted entry */
    save = 0;
    if (ino >= inodes) {
      printf("*** %s on Master Directory has invalid inode number, deleting.\n",
             file_name(dirent));
      dirent[0] = 0;
      dirent[1] = 0;
      ++errcnt;
    } else {
      if (!read_inode(ino, inode)) {
        printf("*** Could not read inode %d, aborting.\n", ino);
        free(ixmap);
        return 0;
      }
      lcnt = GET_INT16(inode, 0);
      if (lcnt == 0) {
        printf("*** File %s has a link count of zero, deleting.\n", file_name(dirent));
        dirent[0] = 0;
        dirent[1] = 0;
        save = 1;
        ++errcnt;
      } else {
        // ++ixmap[ino-1]; -- don't do this here, since the master directory
        //                    will be checked again in check_directory()
        attrib = inode[2];
        if (attrib & _FA_DIR) {
          char temp[20];
        
          strcpy(temp, file_name(dirent));
          if (!check_directory(temp, ixmap)) {
            free(ixmap);
            return 0;
          }
        }
        save |= valid_name(dirent);
      }
    }
    if (save && !read_only) {
      file_seek(mdfcb, fpos);
      if (file_write(mdfcb, dirent, 16) != 16) {
        printf("*** Could not update entry in Master Directory, aborting.\n");
        free(ixmap);
        return 0;
      }
    } else {
      /* need this since md file position can change in check_directory() */
      file_seek(mdfcb, fpos+16L);
    }
  }
  
  /* check for lost files */
  for (i = 0; i < inodes; ++i) {
    if (!read_inode(i+1, inode)) {
      printf("*** Could not read inode %d, aborting.\n", i+1);
      free(ixmap);
      return 0;
    }
    save = 0;
    lcnt = GET_INT16(inode, 0);
    if (lcnt != ixmap[i]) {
      printf("*** Inode %d (%s) has a link count of %d (%d expected)\n",
             i+1, get_name(inode), lcnt, ixmap[i]);
      if (lcnt < ixmap[i]) {
        /* there are more references to the file than what the inode
         * says, thus set inode's link count to ixmap[i].
         */
        lcnt = ixmap[i];
        SET_INT16(inode, 0, lcnt);
        save = 1;
      } else {  /* lcnt > ixmap[i] */
        if (ixmap[i] != 0) {
          /* there is at least one directory entry out there that references
           * the file; thus update inode's link count as above.
           */
          lcnt = ixmap[i];
          SET_INT16(inode, 0, lcnt);
          save = 1;
        } else {  /* ixmap[i] == 0 */
          /* no file is referencing the inode and we must either recover
           * the file (preferably, since lcnt > 0), or set the count to
           * zero (safest).
           * - to recover the file, a new entry needs to be created in
           *   [lostfiles] and assigned to that inode; then, the blocks
           *   assigned to the file must be validated - if they are free
           *   then the file was deleted and the inode count set to zero,
           *   if they are not, then if the file is non-contiguous the
           *   allocation block(s) must be validated, which is not easy
           *   as they do not have a magic number or any safe to do the
           *   validation - the only way is to ensure that 1) the block
           *   numbers are within disk limits, and 2) that the bitmap bit
           *   is set for the block and no other file is using that block.
           * - In principle, one could set lcount to zero and then let
           *   the bitmap check routine to create new (contiguous) files
           *   in [lostfiles] containing chunks of allocated, but not
           *   referenced, blocks (and maybe a block map report?). There
           *   some allocation blocks may appear, and further recovery
           *   could be done by copying + concatenating + deleting
           *   lost file contents.
           * - Another possibility would be to dump the block contents
           *   and let the user decide?
           */
          /* TODO */
        }
      }
      ++errcnt;
      if (save && !read_only) {
        if (!write_inode(i+1, inode)) {
          printf("*** Could not write inode %d, aborting.\n", i+1);
          free(ixmap);
          return 0;
        }
      }
    }
  }
  
  free(ixmap);
     
  return 1;
}

int check_alloc_map(void) {
  unsigned char inode[64], *bm;
  unsigned short lcnt, mask;
  unsigned long  i, j, k, l, clmask;

  /*
   * Block allocation check:
   * 1) loop through each file (inode) and create an allocation bitmap from
   *    scratch:
   *    a) when allocating a block in the new bitmap for a file, if the bit
   *       is set that means the block is already allocated and so the file
   *       is cross-linked to another (it won't be simple to figure out to
   *       which one - rescan will have to be restarted).
   * 2) compare the new bitmap to the one saved on disk:
   *    a) if there are less blocks allocated on the disk bitmap, report it
   *       (it won't be easy to find out affected files unless in step 1
   *       above each file is compared to the old bitmap as well).
   *    b) if there are more blocks in the old bitmap, report it and create
   *       a (possibly contiguous?) file to claim the orphaned blocks. When
   *       creating the new file, use the new bitmap for allocation of file
   *       allocation map blocks (in case the file could not be contiguous).
   * 3) save the new bitmap to the disk, overwriting the old one.
   */
   
  clmask = (1 << clfactor) - 1;

#ifdef DEBUG
  printf("%d inodes in %ld blocks\n", inodes, ixblks);
#endif
  
  bm = calloc(bmsize, sizeof(unsigned char)); /* initialized to zero */
  if (!bm) {
    printf("*** Not enough memory, aborting.\n");
    return 0;
  }
  
  for (i = 0; i < inodes; ++i) {
    if (!read_inode(i+1, inode)) {
      printf("*** Could not read inode %ld, aborting.\n", i+1);
      free(bm);
      return 0;
    }
    lcnt = GET_INT16(inode, 0);
    if (lcnt > 0) {  /* inode in use */
      unsigned long blkno, nalloc, clno, ncls;
      unsigned char attrib;
      
      attrib = inode[2];
      nalloc = GET_INT24(inode, 8);
#ifdef DEBUG
      printf("processing inode %06lX: %ld block(s) allocated\n", i+1, nalloc);
#endif
      if (attrib & _FA_CTG) {
        /* contiguous file: simply mark 'nalloc' bits starting from 'blkno' */
        blkno = GET_INT24(inode, 32);
        clno  = blkno >> clfactor;
        ncls  = nalloc >> clfactor;
        if (nalloc & clmask) ++ncls;
        mask  = (0x80 >> (clno & 7));
        k = clno / 8;
        //!!! if (k >= bmsize) ...
        for (j = 0; j < ncls; ++j) {
          if (bm[k] & mask) {
            printf("*** Multiple allocation of cluster %ld, inode %ld (%s)\n",
                   clno+j, i+1, get_name(inode));
            list_cluster_usage(clno+j);
            ++errcnt;
          } else {
#ifdef DEBUG
            printf("set cluster %06lX, offset %04lX bit mask %02X\n", clno+j, k, mask);
#endif
            bm[k] |= mask;
          }
          mask >>= 1;
          if (mask == 0) {
            mask = 0x80;
            ++k;
            // if (k >= bmsize) ...
          }
        }
      } else {
        /* non-contiguous file: walk the chain of allocated blocks */
        unsigned long next;
        struct BUFFER *buf = NULL;
        int blkptr;

        for (blkptr = 32; blkptr < 47; blkptr += 3) {
          blkno = GET_INT24(inode, blkptr);
          clno  = blkno >> clfactor;
          if (clno > 0) {
            /* mark the block in the bitmap */
            mask = (0x80 >> (clno & 7));
            k = clno / 8;
            // if (k >= bmsize) ...
            if (bm[k] & mask) {
              printf("*** Multiple allocation of cluster %ld, inode %ld (%s)\n",
                     clno, i+1, get_name(inode));
              list_cluster_usage(clno);
              ++errcnt;
            } else {
#ifdef DEBUG
              printf("set cluster %06lX, offset %04lX bit mask %02X\n", clno, k, mask);
#endif
              bm[k] |= mask;
            }
          }
        }

        blkno = GET_INT24(inode, 47);
        if (blkno > 0) {  /* allocation block allocated */
          buf = get_block(blkno);
          if (!buf) {
            printf("*** Could not read block %ld, aborting.\n", blkno);
            free(bm);
            return 0;
          }
          /* mark the allocation block in the bitmap */
          clno = blkno >> clfactor;
          mask = (0x80 >> (clno & 7));
          k = clno / 8;
          // if (k >= bmsize) ...
          if (bm[k] & mask) {
            printf("*** Multiple allocation of cluster %ld, inode %ld (%s)\n",
                   clno, i+1, get_name(inode));
            list_cluster_usage(clno);
            ++errcnt;
          } else {
#ifdef DEBUG
            printf("set cluster %06lX, offset %04lX bit mask %02X (alloc blk)\n", clno, k, mask);
#endif
            bm[k] |= mask;
          }
          next = GET_INT24(buf->data, 3);
          blkptr = 6;
        } else {
          next = 0;
          blkptr = 512;
          buf = NULL;
        }
        
        ncls = nalloc >> clfactor;
        if (nalloc & clmask) ++ncls;
        for (j = 5; j < ncls; ++j) {
          if (blkptr >= 510) {
            if (next == 0) {
              /* this should have been fixed during index file check:
                 stblk = 0 when nalloc != 0 and file is not contiguous -
                 the inode should be marked as deleted - then in the
                 directory check pass the file will be reported and
                 deleted as well */
              printf("*** Null allocation block number, aborting.\n");
              if (buf) release_block(buf);
              free(bm);
              return 0;
            }
            if (buf) release_block(buf);
            buf = get_block(next);
            if (!buf) {
              printf("*** Could not read block %ld, aborting.\n", next);
              free(bm);
              return 0;
            }
            clno = next >> clfactor;
            if ((next & clmask) == 0) {
              /* mark the allocation block in the bitmap */
              mask = (0x80 >> (clno & 7));
              k = clno / 8;
              // if (k >= bmsize) ...
              if (bm[k] & mask) {
                printf("*** Multiple allocation of cluster %ld, inode %ld (%s)\n",
                       clno, i+1, get_name(inode));
                list_cluster_usage(clno);
                ++errcnt;
              } else {
#ifdef DEBUG
                printf("set cluster %06lX, offset %04lX bit mask %02X (alloc blk)\n", clno, k, mask);
#endif
                bm[k] |= mask;
              }
            }
            next = GET_INT24(buf->data, 3);
            blkptr = 6;
          }
          blkno = GET_INT24(buf->data, blkptr);
          blkptr += 3;
          /* mark cluster in bitmap */
          clno = blkno >> clfactor;
          mask = (0x80 >> (clno & 7));
          k = clno / 8;
          // if (k >= bmsize) ...
          if (bm[k] & mask) {
            printf("*** Multiple allocation of cluster %ld, inode %ld (%s)\n",
                   clno, i+1, get_name(inode));
            list_cluster_usage(clno);
            ++errcnt;
          } else {
#ifdef DEBUG
            printf("set cluster %06lX, offset %04lX bit mask %02X\n", clno, k, mask);
#endif
            bm[k] |= mask;
          }
        }
        
        if (buf) release_block(buf);
      }
    }
  }
  
  /* compare bitmaps */
  k = 0;
  j = BMHDRSZ;
  for (i = 0; i < bmblks; ++i) {
    struct BUFFER *buf;

    buf = get_block(bmblock + i);
    if (!buf) {
      printf("*** Could not read block %ld, aborting.\n", bmblock + i);
      free(bm);
      return 0;
    }
    if (i == 0) {
      unsigned long bmsz;
      
      bmsz = GET_INT24(buf->data, 0);
      if (bmsz != nblocks) {
        printf("*** Block count in bitmap header wrong, fixing.\n");
        SET_INT24(buf->data, 0, nblocks);
        buf->modified = 1;
      }
      if (buf->data[4] != clfactor) {
        printf("*** Cluster factor in bitmap header wrong, fixing.\n");
        buf->data[4] = clfactor;
        buf->modified = 1;
      }
    }
    for ( ; j < 512; ++j) {
      if (k >= bmsize) break;
      if (bm[k] != buf->data[j]) {
        for (l = 0, mask = 0x80; l < 8; ++l, mask >>= 1) {
          unsigned long blkno = k * 8 + l;
          if ((bm[k] & mask) != (buf->data[j] & mask)) {
            if (bm[k] & mask) {
              printf("*** Allocated cluster %ld appears free in bitmap.\n", blkno);
              /* TODO: which file? if we do the comparison in the loop above
               * then we could at least report the affected inode number */
              /* When scanning dirs we could also build a reverse lookup table
                 inode->file name (or inode->file direntry [dirblk+ofs]) */
              /* For large devices, may require creating a vm array on another
                 (scratch) disk */
            } else {
              printf("*** Free cluster %ld appears allocated in bitmap.\n", blkno);
              /* See note in the check_directories() routine. The blocks might
                 originate from an orphaned inode, in which case they should
                 be assigned to a new file. Assignment should be done only
                 *after* all unset bits are set (maybe we should split this
                 loop in two?) in order to avoid damaging a block that is
                 allocated to a file */
            }
            ++errcnt;
          }
        }
        buf->data[j] = bm[k];
        buf->modified = 1;
      }
      ++k;
    }
    release_block(buf);
    j = 0;
  }
  
  free(bm);

  return 1;
}

/* Still TODO: search for (and fix) cross-linked files */

int list_cluster_usage(unsigned long cluster) {
  unsigned char inode[64];
  unsigned long i, j, k, clmask;
  unsigned short lcnt;

  clmask = (1 << clfactor) - 1;

  for (i = 0; i < inodes; ++i) {
    if (!read_inode(i+1, inode)) {
      printf("*** Could not read inode %ld, aborting.\n", i+1);
      return 0;
    }
    lcnt = GET_INT16(inode, 0);
    if (lcnt > 0) {  /* inode in use */
      unsigned long blkno, nalloc, clno, ncls;
      unsigned char attrib;
      
      attrib = inode[2];
      nalloc = GET_INT24(inode, 8);
      if (attrib & _FA_CTG) {
        /* contiguous file: 'nalloc' blocks starting from 'blkno' */
        blkno = GET_INT24(inode, 32);
        clno  = blkno >> clfactor;  /* starting cluster number */
        ncls  = nalloc >> clfactor; /* allocated clusters */
        for (j = 0; j < ncls; ++j) {
          if (cluster == clno+j) {
            printf("    Cluster %ld allocated to inode %ld (%s) with VCN %ld\n",
                   cluster, i+1, get_name(inode), j);
          }
        }
      } else {
        /* non-contiguous file: walk the chain of allocated blocks */
        unsigned long next;
        struct BUFFER *buf = NULL;
        int blkptr;

        for (blkptr = 32, j = 0; blkptr < 47; blkptr += 3, ++j) {
          blkno = GET_INT24(inode, blkptr);
          clno  = blkno >> clfactor;
          if ((clno > 0) && (cluster == clno)) {
            printf("    Cluster %ld allocated to inode %ld (%s) with VCN %ld\n",
                   cluster, i+1, get_name(inode), j);
          }
        }

        k = 0;
        blkno = GET_INT24(inode, 47);
        if (blkno > 0) {  /* allocation block allocated */
          buf = get_block(blkno);
          if (!buf) {
            printf("*** Could not read block %ld, aborting.\n", blkno);
            return 0;
          }
          clno = blkno >> clfactor;
          if ((clno > 0) && (cluster == clno)) {
            printf("    Cluster %ld allocated to inode %ld (%s) as allocation map %ld\n",
                   cluster, i+1, get_name(inode), k);
          }
          next = GET_INT24(buf->data, 3);
          blkptr = 6;
        } else {
          next = 0;
          blkptr = 512;
          buf = NULL;
        }
        
        ncls = nalloc >> clfactor;
        if (nalloc & clmask) ++ncls;
        for (j = 5; j < ncls; ++j) {
          if (blkptr >= 510) {
            if (next == 0) {
              /* this should have been fixed during index file check:
                 stblk = 0 when nalloc != 0 and file is not contiguous -
                 the inode should be marked as deleted - then in the
                 directory check pass the file will be reported and
                 deleted as well */
              if (buf) release_block(buf);
              return 0;
            }
            if (buf) release_block(buf);
            buf = get_block(next);
            if (!buf) {
              printf("*** Could not read block %ld, aborting.\n", next);
              return 0;
            }
            ++k;
            clno = next >> clfactor;
            if ((next & clmask) == 0) {
              if ((clno > 0) && (cluster == clno)) {
                printf("    Cluster %ld allocated to inode %ld (%s) as allocation map %ld\n",
                       cluster, i+1, get_name(inode), k);
              }
            }
            next = GET_INT24(buf->data, 3);
            blkptr = 6;
          }
          blkno = GET_INT24(buf->data, blkptr);
          blkptr += 3;
          clno = blkno >> clfactor;
          if ((clno > 0) && (cluster == clno)) {
            printf("    Cluster %ld allocated to inode %ld (%s) with VCN %ld\n",
                   cluster, i+1, get_name(inode), j);
          }
          ++j;
        }
    
        if (buf) release_block(buf);
      }
    }
  }
  return 1;
}
