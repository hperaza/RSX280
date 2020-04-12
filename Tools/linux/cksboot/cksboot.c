/***********************************************************************

   Utility to update the checksum field (last byte) of the boot sector
   of a P112 disk. Copyright (C) 2008, Hector Peraza.

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

int main(int argc, char *argv[]) {
  FILE *f;
  int i;
  unsigned char cks, buf[512];

  if (argc != 2) {
    fprintf(stderr, "usage: %s filename\n", argv[0]);
    return 1;
  }
  
  f = fopen(argv[1], "r+");
  if (!f) {
    fprintf(stderr, "%s: could not open file %s\n", argv[0], argv[1]);
    return 1;
  }
  
  memset(buf, 0, 512);
  fread(buf, 1, 512, f);
  
  for (i = 0, cks = 0; i < 511; ++i) cks += buf[i];
  buf[511] = -cks;
  
  fseek(f, 0L, SEEK_SET);
  fwrite(buf, 1, 512, f);
  
  fclose(f);
  
  return 0;
}
