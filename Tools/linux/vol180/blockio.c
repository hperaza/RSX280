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

/*-----------------------------------------------------------------------*/

extern FILE *imgf;
extern unsigned long img_offset;

int read_block(unsigned long blknum, unsigned char *buf) {
  if (!imgf) return 1;
  //printf("reading block %u\n", block);
  fseek(imgf, blknum * 512L + img_offset, SEEK_SET);
  fread(buf, 1, 512, imgf);
  return 0;
}

int write_block(unsigned long blknum, unsigned char *buf) {
  if (!imgf) return 1;
  //printf("writing block %u\n", block);
  fseek(imgf, blknum * 512L + img_offset, SEEK_SET);
  fwrite(buf, 1, 512, imgf);
  return 0;
}
