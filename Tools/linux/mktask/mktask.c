/***********************************************************************

   Utility to create a RSX180 task image file from a CP/M COM file.
   Copyright (C) 2015, Hector Peraza.

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

/*
 *  Create a RSX180 task image file from a .COM file by adding a task header.
 *  Usage:
 *    mktask inpfile [-o outfile] [-inc xxxx] [-name nnnn] [-priv]
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

void usage(char *progname) {
  fprintf(stderr, "usage: %s inpfile [-o outfile] [-name tskname] [-id tskid]\n"
                  "          [-par parname] [-inc nnnn] [-pri nnn] [-cpu cputype]\n"
                  "          [-priv] [-asg dev:lun:lun,dev:lun-lun...]\n",
                  progname);
}

/* default LUN to device assignment */
char deflun[16][4] = {
  { 'S', 'Y', 0, 0 },  /* 1 */
  { 'S', 'Y', 0, 0 },
  { 'S', 'Y', 0, 0 },
  { 'S', 'Y', 0, 0 },
  { 'T', 'I', 0, 0 },  /* 5 */
  { 'C', 'L', 0, 0 },  /* 6 */
#if 1
  {  0,   0,  0, 0 },
  {  0,   0,  0, 0 },
  {  0,   0,  0, 0 },
  {  0,   0,  0, 0 },  /* 10 */
  {  0,   0,  0, 0 },
  {  0,   0,  0, 0 },
  {  0,   0,  0, 0 },
  {  0,   0,  0, 0 },
  {  0,   0,  0, 0 },
  {  0,   0,  0, 0 }   /* 16 */
#else
  { 'S', 'Y', 0, 0 },
  { 'S', 'Y', 0, 0 },  /* 8 */
  { 'S', 'Y', 0, 0 },
  { 'S', 'Y', 0, 0 },  /* 10 */
  { 'S', 'Y', 0, 0 },
  { 'S', 'Y', 0, 0 },
  { 'O', 'V', 0, 0 },
  { 'O', 'V', 0, 0 },
  { 'O', 'V', 0, 0 },
  { 'O', 'V', 0, 0 }  /* 16 */
#endif
};

int get_devname(char **src, char *dst) {
  int n;
  
  if (!isalpha(**src)) return 0;
  *dst++ = toupper(**src); ++(*src);
  if (!isalpha(**src)) return 0;
  *dst++ = toupper(**src); ++(*src);
  n = strtol(*src, src, 0);
  if ((n < 0) || (n > 255)) return 0;
  *dst++ = n;
  if (**src != ':') return 0;
  ++(*src);
  return 1;
}

int main(int argc, char *argv[]) {
  int  i, j, k, privflag = 0, priority = 50, inc = 0, saddr, eaddr, ept,
       cpu = 0, lunflag = 0;
  FILE *inpf = NULL, *outf = NULL;
  char *ifname = NULL, *ofname = NULL, *p, temp[14];
  char taskname[6], taskid[6], parname[6], header[256], luntbl[16][4];
  
  if (argc == 0) {
    usage(argv[0]);
    return 1;
  }
  
  memset(taskname, ' ', 6);
  memset(taskid, ' ', 6);
  memcpy(parname, "GEN   ", 6);
  memset(luntbl, 0, 16*4);
  
  /* process command line arguments */
  for (i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-h") == 0) {
      usage(argv[0]);
      return 0;
    } else if (strcmp(argv[i], "-o") == 0) {
      if (i < argc - 1) {
        ofname = argv[++i];
      } else {
        fprintf(stderr, "%s: -o requires filename argument\n", argv[0]);
        return 1;
      }
    } else if (strcmp(argv[i], "-inc") == 0) {
      if (i < argc - 1) {
        inc = strtol(argv[++i], NULL, 0);
      } else {
        fprintf(stderr, "%s: -inc requires numeric argument\n", argv[0]);
        return 1;
      }
    } else if (strcmp(argv[i], "-name") == 0) {
      if (i < argc - 1) {
        ++i;
        memset(taskname, ' ', 6);
        for (j = 0; j < 6; ++j) {
          if (argv[i][j] == '\0') break;
          taskname[j] = toupper(argv[i][j]);
        }
      } else {
        fprintf(stderr, "%s: -name requires string argument\n", argv[0]);
        return 1;
      }
    } else if (strcmp(argv[i], "-id") == 0) {
      if (i < argc - 1) {
        ++i;
        memset(taskid, ' ', 6);
        for (j = 0; j < 6; ++j) {
          if (argv[i][j] == '\0') break;
          taskid[j] = toupper(argv[i][j]);
        }
      } else {
        fprintf(stderr, "%s: -id requires string argument\n", argv[0]);
        return 1;
      }
    } else if (strcmp(argv[i], "-par") == 0) {
      if (i < argc - 1) {
        ++i;
        memset(parname, ' ', 6);
        for (j = 0; j < 6; ++j) {
          if (argv[i][j] == '\0') break;
          parname[j] = toupper(argv[i][j]);
        }
      } else {
        fprintf(stderr, "%s: -par requires string argument\n", argv[0]);
        return 1;
      }
    } else if (strcmp(argv[i], "-priv") == 0) {
      privflag = 1;
    } else if (strcmp(argv[i], "-pri") == 0) {
      if (i < argc - 1) {
        priority = strtol(argv[++i], NULL, 0);
      } else {
        fprintf(stderr, "%s: -pri requires numeric argument\n", argv[0]);
        return 1;
      }
    } else if (strcmp(argv[i], "-cpu") == 0) {
      if (i < argc - 1) {
        cpu = strtol(argv[++i], NULL, 0);
        if ((cpu < 0) || (cpu > 2)) {
          fprintf(stderr, "%s: invalid cpu type\n", argv[0]);
          return 1;
        }
      } else {
        fprintf(stderr, "%s: -cpu requires numeric argument\n", argv[0]);
        return 1;
      }
    } else if (strcmp(argv[i], "-asg") == 0) {
      if (i < argc - 1) {
        p = argv[++i];
        while (*p) {
          if (!get_devname(&p, temp)) {
            fprintf(stderr, "%s: invalid device name in LUN assignment\n", argv[0]);
            return 1;
          }
          for (;;) {
            j = strtol(p, &p, 0);
            if ((j < 1) || (j > 16)) {
              fprintf(stderr, "%s: invalid LUN\n", argv[0]);
              return 1;
            }
            memcpy(luntbl[j-1], temp, 3);
            luntbl[j-1][3] = 0;
            lunflag = 1;
            if (!*p) break;
            if (*p == '-') {
              k = strtol(++p, &p, 0);
              if ((k < 1) || (k > 16) || (k <= j)) {
                fprintf(stderr, "%s: invalid LUN range specification\n", argv[0]);
                return 1;
              }
              while (j <= k) {
                memcpy(luntbl[j-1], temp, 3);
                luntbl[j-1][3] = 0;
                ++j;
              }
            }
            if (!*p) break;
            k = *p++;
            if (k == ',') break;
            if (k != ':') {
              fprintf(stderr, "%s: invalid delimiter in LUN assignment\n", argv[0]);
              return 1;
            }
          }
        }
      } else {
        fprintf(stderr, "%s: -asg requires argument\n", argv[0]);
        return 1;
      }
    } else if (argv[i][0] == '-') {
      fprintf(stderr, "%s: invalid option\n", argv[0]);
      return 1;
    } else {
      ifname = argv[i];
    }
  }

  if (!ifname) {
    fprintf(stderr, "%s: missing input filename\n", argv[0]);
    return 1;
  }
  
  if (!ofname) {
    strncpy(temp, ifname, 9);
    temp[9] = '\0';
    p = strchr(temp, '.');
    if (p) *p = '\0';
    strcat(temp, ".tsk");
    ofname = temp;
  }

  if (taskname[0] == ' ') {
    for (i = 0; i < 6; ++i) {
      if ((ofname[i] == '.') || (ofname[i] == '\0')) break;
      taskname[i] = toupper(ofname[i]);
    }
  }
  
  inpf = fopen(ifname, "r");
  if (!inpf) {
    fprintf(stderr, "%s: could not open input file: %s\n",
                    argv[0], strerror(errno));
    return 1;
  }

  outf = fopen(ofname, "w");
  if (!outf) {
    fprintf(stderr, "%s: could not create output file: %s\n",
                    argv[0], strerror(errno));
    fclose(inpf);
    return 1;
  }
  
  saddr = 0x0100;
  fseek(inpf, 0, SEEK_END);
  eaddr = ftell(inpf) + saddr - 1;
  if (eaddr > 0xEFFF) {
    fprintf(stderr, "%s: task image file too big?\n", argv[0]);
    fclose(inpf);
    fclose(outf);
    return 1;
  }
  fseek(inpf, 0, SEEK_SET);
  ept = 0x0100;

  /* create header */
  memset(header, 0, sizeof(header));
  /* magic string */
  memcpy(header, "TSK180", 6);
  /* version number */
  header[0x08] = 2;  /* 1.2 */
  header[0x09] = 1;
  /* CPU type */
  header[0x0A] = cpu;
  /* task name */
  memcpy(&header[0x10], taskname, 6);
  /* task ID */
  memcpy(&header[0x18], taskid, 6);
  /* partition name */
  memcpy(&header[0x20], parname, 6);
  /* start address */
  header[0x28] = saddr & 0xFF;
  header[0x29] = (saddr >> 8) & 0xFF;
  /* end address */
  header[0x2A] = eaddr & 0xFF;
  header[0x2B] = (eaddr >> 8) & 0xFF;
  /* entry point */
  header[0x2C] = ept & 0xFF;
  header[0x2D] = (ept >> 8) & 0xFF;
  /* memory increment (we do not simply add it to the end address,
     because it can be overriden by the user via CLI option) */
  header[0x2E] = inc & 0xFF;
  header[0x2F] = (inc >> 8) & 0xFF;
  /* priority */
  header[0x30] = priority;
  /* privileged flag */
  header[0x31] = privflag;
  /* checksum */
  /* default LUN to device assignment */
  if (lunflag)
    memcpy(&header[0x40], luntbl, 16*4);
  else
    memcpy(&header[0x40], deflun, 16*4);

  /* starting register values? */
  
  /* TODO: add support for segments
     - num segments (1 byte)
     - then:
       * segment type (1 byte): data, code, absolute, reloc, overlay, etc.
       * file offset (4 bytes)
       * load address (2 bytes)
       * length (2 bytes)
     Typical task image files would contain just one segment.
     Tasks with overlays and/or common regions would contain more than one.
  */
  
  fwrite(header, 1, 256, outf);
  
  /* now append the machine code from input file */
  for (;;) {
    i = fread(header, 1, 256, inpf);
    if (i == 0) break;
    fwrite(header, 1, i, outf);
  }

  fclose(inpf);
  fclose(outf);
  
  return 0;
}
