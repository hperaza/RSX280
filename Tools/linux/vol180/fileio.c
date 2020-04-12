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
#include "fileio.h"
#include "indexf.h"
#include "dirio.h"
#include "misc.h"

extern struct FCB *mdfcb, *cdfcb;

extern unsigned char  clfactor;
extern unsigned short defprot;

/* TODO:
 * - set lock bit in inode attrib when file is opened, in case this
 *   application is killed while a file transfer is in progress
 * - ensure seq number is still valid for r/w/seek/etc. operations
 * - update inode when block is allocated/freed
 * - update inode mdate when file is written to
 */

int file_extend(struct FCB *fcb, unsigned long vbn);

/*-----------------------------------------------------------------------*/

static struct FCBheader *fcblist = NULL;

struct FCB *get_fcb(unsigned short ino) {
  struct FCBheader *h = fcblist;

  while (h) {
    if (h->inode == ino) {
      struct FCB *fcb = (struct FCB *) calloc(1, sizeof(struct FCB));
      if (!fcb) return NULL;
      ++h->usecnt;
      fcb->header = h;
      return fcb;
    }
    h = h->next;
  }

  h = (struct FCBheader *) calloc(1, sizeof(struct FCBheader));
  if (!h) return NULL;

  struct FCB *fcb = (struct FCB *) calloc(1, sizeof(struct FCB));
  if (!fcb) { free(h); return NULL; }

  h->usecnt = 1;
  h->inode = ino;
  fcb->header = h;
  
  h->next = fcblist;
  fcblist = h;
  
  return fcb;
}

void free_fcb(struct FCB *fcb) {
  if (--fcb->header->usecnt == 0) {
    struct FCBheader **h = &fcblist;
    
    while (*h) {
      if (*h == fcb->header) {
        *h = (*h)->next;
        break;
      }
      h = &((*h)->next);
    }
    free(fcb->header);
  }
  free(fcb);
}

void dump(unsigned char *buf, int len) {  /* for debug */
  int i, c;
  unsigned addr;

  if (!buf) return;

  len = (len + 15) & 0xFFF0;

  for (addr = 0; addr < len; addr += 16) {
    printf("%04X: ", addr);
    for (i = 0; i < 16; ++i)
      printf("%02X ", buf[addr+i]);
    printf("   ");
    for (i = 0; i < 16; ++i)
      c = buf[addr+i], fputc(((c >= 32) && (c < 127)) ? c : '.', stdout);
    printf("\n");
  }
}

void dump_alloc_map(struct FCB *fcb) {
  int i, n;

  if (!fcb) return;

  if (fcb->header->attrib & _FA_CTG) {
    printf("Contiguous file:\n");
    printf("  %lu blocks allocated, %lu blocks used\n", fcb->header->nalloc, fcb->header->nused);
    printf("  Allocation map:\n");
    printf("  BMAP[0] = %06lX", fcb->header->bmap[0]);
    if (fcb->header->nalloc > 0) {
      printf(" ->");
      for (i = 0, n = 0; i < fcb->header->nalloc; ++i) {
        printf(" %06lX", fcb->header->bmap[0] + i);
        if (++n == 8) {
          printf("\n                     ");
          n = 0;
        }
      }
    }
    printf("\n");
  } else {
    unsigned long prev, next, blkptr;
    struct BUFFER *buf;

    printf("Non-contiguous file:\n");
    printf("  %lu blocks allocated, %lu blocks used\n", fcb->header->nalloc, fcb->header->nused);
    for (i = 0; i < 5; ++i) {
      printf("  BMAP[%d] = %06lX\n", i, fcb->header->bmap[i]);
    }
    printf("  BMAP[5] = %06lX", fcb->header->bmap[5]);
    next = fcb->header->bmap[5];
    if (next) {
      printf(" ->");
      while (next) {
        buf = get_block(next);
        if (!buf) {
          printf("\nCould not read block %06lX\n", next);
          return;
        }
        prev = GET_INT24(buf->data, 0);
        next = GET_INT24(buf->data, 3);
        blkptr = 6;
        printf(" prev %06lX, next %06lX", prev, next);
        printf("\n                     ");
        n = 0;
        while (blkptr < 510) {
          printf(" %06lX", (long) GET_INT24(buf->data, blkptr));
          if (++n == 8) {
            printf("\n                     ");
            n = 0;
          }
          blkptr += 3;
        }
        printf("\n");
        release_block(buf);
        if (next) printf("            %06lX ->", next);
      }
    }
    printf("\n");
  }
}

/* Return a string representing the file name stored in a FCB */
char *get_file_name(struct FCB *fcb) {
  static char str[30];
  int i;
  char *p;

  if (!fcb) return "";

  p = str;
  if (*fcb->header->dirname) {
    *p++ = '[';
    for (i = 0; i < 9; ++i) if (fcb->header->dirname[i] != ' ') *p++ = fcb->header->dirname[i];
    *p++ = ']';
  }
  for (i = 0; i < 9; ++i) if (fcb->header->fname[i] != ' ') *p++ = fcb->header->fname[i];
  *p++ = '.';
  for (i = 0; i < 3; ++i) if (fcb->header->ext[i] != ' ') *p++ = fcb->header->ext[i];
  *p++ = ';';
  snprintf(p, 5, "%d", fcb->header->vers);

  return str;
}

/* Return a string containing the directory name stored in a FCB,
 * excluding the brackets */
char *get_dir_name(struct FCB *fcb) {
  static char str[10];
  int i;
  char *p;

  if (!fcb) return "";

  p = str;
  for (i = 0; i < 9; ++i) if (fcb->header->fname[i] != ' ') *p++ = fcb->header->fname[i];
  *p = '\0';

  return str;
}

/* Parse file name */
int parse_name(char *str, char *dirname, char *fname, char *ext, short *vers) {
  char *p, *q, *r;
  int  len, maxlen;

  *dirname = *fname = *ext = '\0';
  *vers = 0;
  p = str;
  if (*p == '[') {
    q = strchr(++p, ']');
    if (!q) return 0;
    len = q - p;
    if (len > 9) len = 9;
    strncpy(dirname, p, len);
    dirname[len] = '\0';
    p = q + 1;
  }
  q = strchr(p, '.');
  r = fname;
  maxlen = 9;
  if (q) {
    len = q - p;
    if (len > 9) len = 9;
    strncpy(fname, p, len);
    fname[len] = '\0';
    p = q + 1;
    r = ext;
    maxlen = 3;
  }
  q = strchr(p, ';');
  if (q) {
    len = q - p;
    if (len > maxlen) len = maxlen;
    strncpy(r, p, len);
    r[len] = '\0';
    *vers = atoi(q + 1);
  } else {
    len = strlen(p);
    if (len > maxlen) len = maxlen;
    strncpy(r, p, len);
    r[len] = '\0';
  }

  return 1;
}

/* Translate virtual block number to logical block number */
/* Sets fcb->curalloc */
int file_vbn_to_lbn(struct FCB *fcb, unsigned long vbn, unsigned long *lbn) {
  struct BUFFER *buf;
  unsigned int blkptr;
  unsigned long blkno, vcn, clmask;

  if (!fcb) return 1;

  if (fcb->header->nused == 0) {
    /* file is empty */
    return 1;
  } else if (vbn >= fcb->header->nused) {
    /* beyond the end of file */
    return 1;
  }

  fcb->curblk = vbn;
  if (fcb->header->attrib & _FA_CTG) {
    /* compute logical block number for contiguous file */
    *lbn = fcb->header->bmap[0] + vbn;
  } else {
    /* for non-contiguous files, find absolute block number in alloc map */
    vcn = vbn >> clfactor;  /* get virtual cluster number */
    clmask = (1 << clfactor) - 1;
    if (vcn < 5) {
      /* easy */
      fcb->curalloc = 0;
      blkno = fcb->header->bmap[vcn];
      if (blkno == 0) return 1;  /* beyond end of file */
      *lbn = blkno + (vbn & clmask);
      return 0;
    }

    vcn -= 5;
    fcb->curalloc = fcb->header->bmap[5]; /* get first storage alloc block */
    if (fcb->curalloc == 0) return 1; /* beyond end of file */
    buf = get_block(fcb->curalloc);

    while (vcn >= 168) {
      /* traverse list of blocks: skip first complete alloc blocks
         until vcn is in the 0..167 range */
      fcb->curalloc = GET_INT24(buf->data, 3);
      if (fcb->curalloc == 0) return 1;
      release_block(buf);
      buf = get_block(fcb->curalloc);
      vcn -= 168;
    }

    /* set pointer to current block index in alloc buf */
    blkptr = 6 + vcn * 3;
    /* fetch the absolute block number */
    *lbn = GET_INT24(buf->data, blkptr);
    *lbn += vbn & clmask;

    release_block(buf);
  }

  return 0;  /* success */
}

/* Seek file to absolute byte position */
int file_seek(struct FCB *fcb, unsigned long pos) {
  unsigned long vbn, lbn;

  if (!fcb) return 1;

  vbn = (pos >> 9);            /* virtual block number = pos / 512 */
  fcb->byteptr = pos & 0x1FF;  /* byte pos in block = pos % 512 */

  if (fcb->header->nused == 0) {
    /* file is empty */
    fcb->curblk = 0;
    fcb->byteptr = 0;
    return 1;
  } else if (vbn == fcb->header->nused - 1) {
    /* on last block */
    if (fcb->byteptr > fcb->header->lbcount) fcb->byteptr = fcb->header->lbcount;
  } else if (vbn >= fcb->header->nused) {
    /* beyond the end of file */
    vbn = fcb->header->nused - 1;
    fcb->byteptr = fcb->header->lbcount;
  }

  return file_vbn_to_lbn(fcb, vbn, &lbn);
}

/* Return the current file position */
unsigned long file_pos(struct FCB *fcb) {
  unsigned long fpos;

  if (!fcb) return 1;

  if (fcb->header->nused == 0) {
    fpos = 0;
  } else {
    fpos = (unsigned long) fcb->curblk * 512L + (unsigned long) fcb->byteptr;
  }

  return fpos;
}

/* Read 'len' bytes from file, returns the actual number of bytes read.
 * Note that a max of 65536 bytes can be read (full 8080 address space,
 * anyway). Handles both contiguous and non-contiguous files. */
int file_read(struct FCB *fcb, unsigned char *buf, unsigned len) {
  unsigned long vbn, lbn;
  unsigned nbytes, nread, buflen;
  struct BUFFER *filbuf;

  if (!fcb) return 1;

  /* empty file? */
  if (fcb->header->nused == 0) return 0;

  /* see if we are already at the end of file */
  if (end_of_file(fcb)) return 0;

  vbn = fcb->curblk;
  if (file_vbn_to_lbn(fcb, vbn, &lbn)) return 0;
  filbuf = get_block(lbn);

  nread = 0;
  for (;;) {
    /* buflen = how many bytes are in current block */
    buflen = (fcb->curblk == fcb->header->nused - 1) ? fcb->header->lbcount : 512;
    if (fcb->byteptr + len <= buflen) {
      /* all the data we need is on this block */
      memcpy(buf, &filbuf->data[fcb->byteptr], len);
      /* advance pointers and we're done */
      fcb->byteptr += len;
      nread += len;
      break;
    } else {
      /* read operation crosses block boundary */
      nbytes = buflen - fcb->byteptr;  /* bytes left on this block */
      if (nbytes > 0) {
        /* copy whatever we have to dest buffer */
        memcpy(buf, &filbuf->data[fcb->byteptr], nbytes);
        /* advance pointers */
        buf += nbytes;
        len -= nbytes;
        nread += nbytes;
      }
      if (buflen < 512) {
        /* last block is incomplete, done reading (end of file) */
        fcb->byteptr = buflen;
        break;
      }
      /* time to read from next block */
      release_block(filbuf);
      ++fcb->curblk;
      fcb->byteptr = 0;
      vbn = fcb->curblk;
      if (file_vbn_to_lbn(fcb, vbn, &lbn)) return nread; /* eof */
      filbuf = get_block(lbn);
    }
  }
  release_block(filbuf);

  return nread;
}

/* Write 'len' bytes to file, returns the actual number of bytes written.
 * Note that a max of 65536 bytes can be written (full 8080 address space,
 * anyway) */
int file_write(struct FCB *fcb, unsigned char *buf, unsigned len) {
  unsigned long vbn, lbn;
  unsigned nbytes, nwritten, newblk;
  struct BUFFER *filbuf;

  if (!fcb) return 1;

  vbn = fcb->curblk;
  if (vbn >= fcb->header->nused) {
    if (file_extend(fcb, vbn)) return 0; /* no more disk space */
  }
  if (file_vbn_to_lbn(fcb, vbn, &lbn)) return 0; /* should not happen */
  filbuf = get_block(lbn);

  nwritten = 0;
  for (;;) {
    if (fcb->byteptr + len <= 512) {
      /* data to write fits entirely in the current block */
      memcpy(&filbuf->data[fcb->byteptr], buf, len);
      filbuf->modified = 1;
      fcb->byteptr += len;
      if (fcb->header->nused == fcb->curblk + 1) {
        /* we're on last block: adjust lbcount if necessary */
        if (fcb->byteptr > fcb->header->lbcount) fcb->header->lbcount = fcb->byteptr;
      }
      nwritten += len;
      break;
    } else {
      /* data to write will overflow to next block(s) */
      nbytes = 512 - fcb->byteptr;  /* how many bytes left on this block */
      if (nbytes > 0) {
        /* fill the rest of this block */
        memcpy(&filbuf->data[fcb->byteptr], buf, nbytes);
        filbuf->modified = 1;
        buf += nbytes;
        len -= nbytes;
        nwritten += nbytes;
      }
      release_block(filbuf);
      ++fcb->curblk;
      fcb->byteptr = 0;
      fcb->header->lbcount = 0;
      newblk = 0;
      vbn = fcb->curblk;
      if (vbn >= fcb->header->nused) {
        if (file_extend(fcb, vbn)) return nwritten; /* no more disk space */
        newblk = 1;
      }
      if (file_vbn_to_lbn(fcb, vbn, &lbn)) return nwritten; /* should not happen */
      filbuf = newblk ? new_block(lbn) : get_block(lbn);
    }
  }
  release_block(filbuf);

  return nwritten;
}

/* Extend file up to the specified virtual block number */
int file_extend(struct FCB *fcb, unsigned long vbn) {
  unsigned long lbn, prev, vcn, clmask;
  unsigned short blkptr;
  struct BUFFER *buf;
  int i;

  if (!fcb) return 1;

  if (vbn < fcb->header->nused) return 0;  /* nothing to do */

  if (fcb->header->attrib & _FA_CTG) {
    /* contiguous files */
    fcb->header->nused = vbn + 1;
    if (fcb->header->nused > fcb->header->nalloc) return 1; /* TODO: try to extend contiguous space? */
  } else {
    /* non-contiguous files */
    vcn = vbn >> clfactor;  /* get virtual cluster number */
    clmask = (1 << clfactor) - 1;
    if (vbn & clmask) ++vcn;
    if (vcn < 5) {
      for (i = 0; i < 5; ++i) {
        if (fcb->header->bmap[i] == 0) {
          lbn = alloc_cluster();
          if (lbn == 0) return 1;  /* out of disk space */
          fcb->header->bmap[i] = lbn;
          fcb->header->nalloc += (1 << clfactor);
        }
        if (vcn == 0) {
          fcb->header->nused = vbn + 1;
          return 0; /* success */
        }
        --vcn;
      }
    }
    vcn -= 5;
    fcb->curalloc = fcb->header->bmap[5];  /* start from the beginning */

    prev = 0;
    buf = NULL;
    if (fcb->curalloc > 0) {
      buf = get_block(fcb->curalloc);
      while (vcn >= 168) {
        prev = fcb->curalloc;
        fcb->curalloc = GET_INT24(buf->data, 3);
        if (fcb->curalloc == 0) break;
        release_block(buf);
        buf = get_block(fcb->curalloc);
        vcn -= 168;
      }
    }

    if (fcb->curalloc == 0) {
      /* add new block map */
      if ((prev != 0) && ((prev & clmask) != clmask)) {
        /* there is still space on this cluster */
        lbn = prev + 1;
      } else {
        /* allocate new cluster for block map */
        lbn = alloc_cluster();
        if (lbn == 0) {
          if (buf) release_block(buf);
          return 1;  /* out of disk space */
        }
      }

      /* set 'next' link on old */
      if (prev == 0) {
        fcb->header->bmap[5] = lbn;
      } else {
        SET_INT24(buf->data, 3, lbn);
        buf->modified = 1;
        release_block(buf);
      }
      fcb->curalloc = lbn;
      buf = new_block(fcb->curalloc);
      /* set 'prev' link on new */
      SET_INT24(buf->data, 0, prev);
      buf->modified = 1;
    }
    blkptr = 6;
    for (;;) {
      lbn = GET_INT24(buf->data, blkptr);
      if (lbn == 0) {
        lbn = alloc_cluster();
        if (lbn == 0) {
          release_block(buf);
          return 1;  /* out of disk space */
        }
        SET_INT24(buf->data, blkptr, lbn);
        buf->modified = 1;
        fcb->header->nalloc += (1 << clfactor);
      }
      blkptr += 3;
      if (blkptr >= 510) {
        blkptr = 6;
        prev = fcb->curalloc;
        fcb->curalloc = GET_INT24(buf->data, 3);
        if (fcb->curalloc == 0) {
          /* time to add a new block map */
          if ((prev & clmask) != clmask) {
            /* there is still space on this cluster */
            lbn = prev + 1;
          } else {
            /* allocate new cluster for block map */
            lbn = alloc_cluster();
            if (lbn == 0) {
              release_block(buf);
              return 1;  /* out of disk space */
            }
          }
          /* set 'next' link on old */
          SET_INT24(buf->data, 3, lbn);
          buf->modified = 1;
          release_block(buf);
          buf = new_block(lbn);
          /* set 'prev' link on new */
          SET_INT24(buf->data, 0, prev);
          buf->modified = 1;
          fcb->curalloc = lbn;
        } else {
          release_block(buf);
          buf = get_block(fcb->curalloc);
        }
      }
      if (vcn == 0) break;
      --vcn;
    }
    release_block(buf);
  }

  fcb->header->nused = vbn + 1;

  return 0;  /* success */
}

/* Return 1 (true) if file position is at or beyond the end of file */
int end_of_file(struct FCB *fcb) {

  if (!fcb) return 1;

  if (fcb->curblk >= fcb->header->nused) {
    /* beyond last block */
    return 1;
  } else if (fcb->curblk == fcb->header->nused - 1) {
    /* on last block, see if we're beyond byte count */
    if (fcb->byteptr >= fcb->header->lbcount) return 1;
  }
  return 0;
}

/* Close file. The FCB is not freed. */
int close_file(struct FCB *fcb) {
  unsigned char inode[64];
  unsigned short ino;
  time_t now;

  if (!fcb) return 0;

  time(&now);

  ino = fcb->header->inode;
  if (ino == 0) return 0;

  if (!read_inode(ino, inode)) return 0; /* panic */
  if (GET_INT16(inode, 0) == 0) return 0; /* panic */
  SET_INT24(inode,  8, fcb->header->nalloc);
  SET_INT24(inode, 11, fcb->header->nused);
  SET_INT16(inode, 14, fcb->header->lbcount);
  SET_INT24(inode, 32, fcb->header->bmap[0]);
  SET_INT24(inode, 35, fcb->header->bmap[1]);
  SET_INT24(inode, 38, fcb->header->bmap[2]);
  SET_INT24(inode, 41, fcb->header->bmap[3]);
  SET_INT24(inode, 44, fcb->header->bmap[4]);
  SET_INT24(inode, 47, fcb->header->bmap[5]);
  set_mdate(inode, now);
  write_inode(ino, inode);

  return 1;
}

/* Set file dates. */
int set_file_dates(struct FCB *fcb, time_t created, time_t modified) {
  unsigned char inode[64];
  unsigned short ino;

  if (!fcb) return 0;

  ino = fcb->header->inode;
  if (ino == 0) return 0;

  if (!read_inode(ino, inode)) return 0; /* panic */
  if (GET_INT16(inode, 0) == 0) return 0; /* panic */
  set_cdate(inode, created);
  set_mdate(inode, modified);
  write_inode(ino, inode);

  return 1;
}

/* Open file in master directory */
struct FCB *open_md_file(char *name) {
  struct FCB *fcb;
  unsigned char dirent[16];
  unsigned char inode[64];
  unsigned short ino;
  char dname[10], fname[10], ext[4];
  short vers;

  if (!parse_name(name, dname, fname, ext, &vers)) {
    printf("Invalid file name\n");
    return NULL;
  }

  if (!mdfcb) return NULL;

  file_seek(mdfcb, 0L);
  for (;;) {
    if (file_read(mdfcb, dirent, 16) != 16) return NULL;
    ino = GET_INT16(dirent, 0);
    if ((ino != 0) && match(dirent, fname, ext, vers)) {
      if (!read_inode(ino, inode)) return NULL; /* panic */
      if (GET_INT16(inode, 0) == 0) return NULL; /* panic */

      fcb = get_fcb(ino);
      if (fcb->header->usecnt == 1) {
        fcb->header->attrib = inode[2];
        strcpy(fcb->header->dirname, "MASTER");
        strncpy(fcb->header->fname, (char *) &dirent[2], 9);
        strncpy(fcb->header->ext, (char *) &dirent[11], 3);
        fcb->header->vers = GET_INT16(dirent, 14);
        fcb->header->user = inode[6];
        fcb->header->group = inode[7];
        fcb->header->clfactor = inode[3];
        fcb->header->lnkcnt = GET_INT16(inode, 0);
        fcb->header->seqno = GET_INT16(inode, 4);
        fcb->header->nalloc = GET_INT24(inode, 8);
        fcb->header->nused = GET_INT24(inode, 11);
        fcb->header->lbcount = GET_INT16(inode, 14);
        fcb->header->bmap[0] = GET_INT24(inode, 32);
        fcb->header->bmap[1] = GET_INT24(inode, 35);
        fcb->header->bmap[2] = GET_INT24(inode, 38);
        fcb->header->bmap[3] = GET_INT24(inode, 41);
        fcb->header->bmap[4] = GET_INT24(inode, 44);
        fcb->header->bmap[5] = GET_INT24(inode, 47);
      }
      fcb->curblk = 0;
      fcb->byteptr = 0;
      fcb->curalloc = 0;
      return fcb;
    }
  }

  return NULL;
}

/* Open file, returns a newly allocated FCB. */
struct FCB *open_file(char *name) {
  struct FCB *fcb, *dirfcb;
  char dname[10], fname[10], ext[4];
  unsigned char temp[16], dirent[16], inode[64];
  unsigned short ino;
  short vers, dvers, hivers;
  int found, dir_close_flag;

  if (!parse_name(name, dname, fname, ext, &vers)) {
    printf("Invalid file name\n");
    return NULL;
  }

  if (*dname) {
    char mdfile[20];
    strcpy(mdfile, dname);
    strcat(mdfile, ".DIR");
    dirfcb = open_md_file(mdfile);
    dir_close_flag = 1;
  } else {
    dirfcb = cdfcb;
    dir_close_flag = 0;
  }

  if (!dirfcb) return NULL;

  /* if no version specified, open the highest version */
  found = 0;
  hivers = 0;  /* to track highest version number */
  file_seek(dirfcb, 0L);
  for (;;) {
    if (file_read(dirfcb, temp, 16) != 16) break;
    ino = GET_INT16(temp, 0);
    dvers = GET_INT16(temp, 14);
    if ((ino != 0) && match(temp, fname, ext, vers)) {
      if ((vers > 0) || (dvers > hivers)) {
        /* note that this works also in case of explicit version,
         * since 'match' also matches the version number if > 0,
         * and there will be just one single match. */
        memcpy(dirent, temp, 16);
        hivers = dvers;
        found = 1;
      }
    }
  }

  if (!found) return NULL;

  ino = GET_INT16(dirent, 0);
  if (ino == 0) return NULL;  /* should not happen */
  if (!read_inode(ino, inode)) return NULL;
  if (GET_INT16(inode, 0) == 0) return NULL;

  fcb = get_fcb(ino);
  if (fcb->header->usecnt == 1) {
    fcb->header->attrib = inode[2];
    strncpy(fcb->header->dirname, dirfcb->header->fname, 9);
    strncpy(fcb->header->fname, (char *) &dirent[2], 9);
    strncpy(fcb->header->ext, (char *) &dirent[11], 3);
    fcb->header->vers = GET_INT16(dirent, 14);
    fcb->header->user = inode[6];
    fcb->header->group = inode[7];
    fcb->header->clfactor = inode[3];
    fcb->header->lnkcnt = GET_INT16(inode, 0);
    fcb->header->seqno = GET_INT16(inode, 4);
    fcb->header->nalloc = GET_INT24(inode, 8);
    fcb->header->nused = GET_INT24(inode, 11);
    fcb->header->lbcount = GET_INT16(inode, 14);
    fcb->header->bmap[0] = GET_INT24(inode, 32);
    fcb->header->bmap[1] = GET_INT24(inode, 35);
    fcb->header->bmap[2] = GET_INT24(inode, 38);
    fcb->header->bmap[3] = GET_INT24(inode, 41);
    fcb->header->bmap[4] = GET_INT24(inode, 44);
    fcb->header->bmap[5] = GET_INT24(inode, 47);
  }
  fcb->curblk = 0;
  fcb->byteptr = 0;
  fcb->curalloc = 0;

  if (dir_close_flag) {
    close_file(dirfcb);
    free_fcb(dirfcb);
  }

  return fcb;
}

/* Create a file. If the file is contiguous, allocate the specified number
 * of blocks. If not contiguous, allocate just the first allocation block. */
struct FCB *create_file(char *filename, char group, char user,
                        int contiguous, unsigned csize) {
  unsigned char dirent[16], inode[64], found;
  char dname[10], fname[10], ext[4], newname[256];
  unsigned long cpos, fpos;
  unsigned short ino;
  unsigned long  blkno, clno, nclusters, clmask;
  short dvers, vers, hivers;
  time_t now;
  struct FCB *dirfcb;
  int dir_close_flag;

  if (!parse_name(filename, dname, fname, ext, &vers)) {
    printf("Invalid file name\n");
    return NULL;
  }

  /* find a free inode */
  ino = new_inode();
  if (ino == 0) {
    fprintf(stderr, "Index file full\n");
    return NULL;
  }
  if (!read_inode(ino, inode)) return NULL; /* panic */
  if (GET_INT16(inode, 0) != 0) return NULL; /* panic */

  if (*dname) {
    char mdfile[20];
    strcpy(mdfile, dname);
    strcat(mdfile, ".DIR");
    dirfcb = open_md_file(mdfile);
    dir_close_flag = 1;
  } else {
    dirfcb = cdfcb;
    dir_close_flag = 0;
  }

  if (!dirfcb) return NULL;

  /* create a new version if file exists */
  file_seek(dirfcb, 0L);
  found = 0;
  hivers = 0;  /* to track highest version */
  for (;;) {
    cpos = file_pos(dirfcb);
    if (file_read(dirfcb, dirent, 16) != 16) break; /* at end of directory */
    if ((dirent[0] == 0) && (dirent[1] == 0)) {
      if (!found) fpos = cpos;  /* remember this free dir entry */
      found = 1;
    } else if (match(dirent, fname, ext, 0)) {
      dvers = GET_INT16(dirent, 14);
      if ((vers > 0) && (vers == dvers)) {
        fprintf(stderr, "File exists\n");
        if (dir_close_flag) {
          close_file(dirfcb);
          free_fcb(dirfcb);
        }
        return NULL;
      }
      if (dvers > hivers) hivers = dvers;
    }
  }
  /* no free entry found, we'll create a new one at the end */
  if (!found) fpos = cpos;
  /* new file version */
  vers = hivers + 1;

  time(&now);
  set_dir_entry(dirent, ino, fname, ext, vers);

  if (contiguous) {
    /* pre-allocate csize blocks for the file */
    nclusters = csize >> clfactor;
    clmask = (1 << clfactor) - 1;
    if (csize & clmask) ++nclusters;
    blkno = alloc_clusters(nclusters);
    if (blkno == 0) {
      fprintf(stderr, "No contiguous space\n");
      if (dir_close_flag) {
        close_file(dirfcb);
        free_fcb(dirfcb);
      }
      return NULL;
    }
    set_inode(inode, 1, _FA_FILE | _FA_CTG, group, user,
              csize, 0, 0, defprot);
    SET_INT24(inode, 32, blkno);
    memset(&inode[35], 0, 5*3);
  } else {
    set_inode(inode, 1, _FA_FILE, group, user,
              0, 0, 0, defprot);
    memset(&inode[32], 0, 6*3);
  }
  set_cdate(inode, now);
  set_mdate(inode, now);
  set_name(inode, fname, ext, vers);
  file_seek(dirfcb, fpos);
  if (file_write(dirfcb, dirent, 16) != 16) {
    /* failed to extend the directory */
    if (contiguous) {
      clno = blkno >> clfactor;
      while (nclusters > 0) {
        free_cluster(clno++);
        --nclusters;
      }
    }
    fprintf(stderr, "Could not enter file, no space left on device\n");
    if (dir_close_flag) {
      close_file(dirfcb);
      free_fcb(dirfcb);
    }
    return NULL;
  }

  write_inode(ino, inode);

  if (dir_close_flag) {
    close_file(dirfcb);
    free_fcb(dirfcb);
  }

  newname[0] = '\0';
  if (*dname) {
    snprintf(newname, 20, "[%s]", dname);
  }
  snprintf(newname + strlen(newname), 200, "%s.%s;%d", fname, ext, vers);

  return open_file(newname);
}

/* Delete file in current directory */
int delete_file(char *name) {
  unsigned char dirent[16], inode[64];
  char dname[10], fname[10], ext[10];
  short vers;
  unsigned long fpos, blkno, clno, nblks, nclusters, clmask;
  unsigned short ino, blkptr;
  struct BUFFER *buf;
  struct FCB *dirfcb;
  int i, retc, dir_close_flag;

  if (!parse_name(name, dname, fname, ext, &vers)) {
    printf("Invalid file name\n");
    return 0;
  }

  if (*dname) {
    char mdfile[20];
    strcpy(mdfile, dname);
    strcat(mdfile, ".DIR");
    dirfcb = open_md_file(mdfile);
    dir_close_flag = 1;
  } else {
    dirfcb = cdfcb;
    dir_close_flag = 0;
  }

  if (!dirfcb) return 0;

  retc = 0;
  file_seek(dirfcb, 0L);
  for (;;) {
    fpos = file_pos(dirfcb);
    if (file_read(dirfcb, dirent, 16) != 16) return 0; /* file not found */
    ino = GET_INT16(dirent, 0);
    if ((ino != 0) && match(dirent, fname, ext, vers)) {  /* TODO: do not remove dirs */
      if (!read_inode(ino, inode)) return 0;  /* index error */
      if (GET_INT16(inode, 0) == 0) return 0; /* index error */
      clmask = (1 << clfactor) - 1;
      if (inode[2] & _FA_CTG) {
        /* contiguous file */
        blkno = GET_INT24(inode, 32);
        nblks = GET_INT24(inode, 8);  // nalloc
        nclusters = nblks >> clfactor;
        if (nblks & clmask) ++nclusters;
        clno = blkno >> clfactor;
        while (nclusters > 0) {
          free_cluster(clno++);
          --nclusters;
        }
      } else {
        /* non-contiguous file */
        for (i = 0; i < 5; ++i) {
          blkno = GET_INT24(inode, 32+i*3);
          if (blkno > 0) free_cluster(blkno >> clfactor);
        }
        blkno = GET_INT24(inode, 47);
        while (blkno > 0) {
          buf = get_block(blkno);
          for (blkptr = 6; blkptr < 510; blkptr += 3) {
            blkno = GET_INT24(buf->data, blkptr);
            if (blkno > 0) free_cluster(blkno >> clfactor);
          }
          blkno = GET_INT24(buf->data, 3);
          release_block(buf);
          free_cluster(buf->blkno >> clfactor);
        }
      }
      file_seek(dirfcb, fpos);
      SET_INT16(dirent, 0, 0);
      file_write(dirfcb, dirent, 16);  /* TODO: set dirfcb mdate */
      SET_INT16(inode, 0, 0);
      write_inode(ino, inode);
      free_inode(ino);
      retc = 1;
      break;
    }
  }

  if (dir_close_flag) {
    close_file(dirfcb);
    free_fcb(dirfcb);
  }

  return retc;
}
