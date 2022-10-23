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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#include "fileio.h"
#include "blockio.h"
#include "indexf.h"
#include "misc.h"
#include "vmr.h"
#include "rsx180.h"

extern FILE *imgf;

struct symbol {
  char name[8];
  address value;
  int  sys_type;
  int  set;
};

struct symbol symtab[] = {
  { "SYSDAT", 0, 0, 0 },
  { "SYSVER", 0, 0, 0 },
  { "SYSTYP", 0, 0, 0 },
  { "SYSTOP", 0, 0, 0 },
  { "$HOSTN", 0, 0, 0 },
  { "SYSEND", 0, 0, 0 },
  { "$POOL",  0, 0, 0 },
  { "$POLSZ", 0, 0, 0 },
  { "$PLIST", 0, 0, 0 },
  { "$TLIST", 0, 0, 0 },
  { "$CLIST", 0, 0, 0 },
  { "$CLKQ",  0, 0, 0 },
  { "$RNDC",  0, 0, 0 },
  { "$RNDH",  0, 0, 0 },
  { "$RNDL",  0, 0, 0 },
  { "$SWPC",  0, 0, 0 },
  { "$SWPRI", 0, 0, 0 },
  { "$PHYDV", 0, 0, 0 },
  { "$LOGDV", 0, 0, 0 },
  { "$MFLGS", 0, 0, 0 },
  { "IDDTBL", 0, 0, 0 },
  { "$MVTBL", 0, 0, 0 },
  { "CHKTRP", 0, 1, 0 },
  { "SYSENT", 0, 1, 0 },
  { "$DBTRP", 0, 1, 0 }
};

#define NSYM (sizeof(symtab)/sizeof(symtab[0]))

byte system_image[131072];
int  system_type, system_size;

#define WC_BASE  0x01
#define WC_SIZE  0x02

char *speed_str[] = {
  "Unknown",	// S.0
  "50",
  "75",
  "110",
  "134",
  "150",
  "200",
  "300",
  "600",
  "1200",
  "1800",
  "2000",
  "2400",
  "3600",
  "4800",
  "7200",
  "9600",
  "14400",
  "19200",
  "28800",
  "38400",
  "57600",
  "76800",
  "115200"
};

/*--------------------------------------------------------------------*/

/* Symbol table routines */

address read_value(struct FCB *f) {
  int i, c, value;

  value = 0;
  for (i = 0; i < 4; ++i) {
    c = 0;
    file_read(f, (byte *) &c, 1);
    c = toupper(c);
    value <<= 4;
    if ((c >= '0') && (c <= '9')) {
      value += (c - '0');
    } else if ((c >= 'A') && (c <= 'F')) {
      value += (c - 'A' + 10);
    }
  }

  file_read(f, (byte *) &c, 1);  /* skip following space */
  
  return value;
}

char *read_name(struct FCB *f) {
  static char str[16];
  int i, c;
  
  for (i = 0; i < 16; ++i) {
    c = 0;
    file_read(f, (byte *) &c, 1);
    if (!isspace(c)) str[i] = c; else break;
  }
  str[i] = '\0';
  if (c == '\r') file_read(f, (byte *) &c, 1);
  
  return str;
}

int load_symbols(struct FCB *f) {
  address value;
  char *name;
  int  i, retc;

  for (i = 0; i < NSYM; ++i) symtab[i].set = 0;
  system_type = 0;

  retc = 0;  
  while (!end_of_file(f)) {
    value = read_value(f);
    name = read_name(f);
    if (name[0] == '\x1A') break;
    for (i = 0; i < NSYM; ++i) {
      if (strcmp(symtab[i].name, name) == 0) {
        if (symtab[i].set) {
          printf("Duplicate symbol \"%s\"\n", name);
          retc = 1;
        } else {
          symtab[i].value = value;
          symtab[i].set = 1;
        }
      }
    }
  }
  
  system_type = sys_getb(0, get_sym("SYSTYP"));

  for (i = 0; i < NSYM; ++i) {
    if (!symtab[i].set) {
      if ((symtab[i].sys_type == 0) || (symtab[i].sys_type == system_type)) {
        printf("Undefined symbol \"%s\"\n", symtab[i].name);
        retc = 1;
      }
    }
  }

  return retc;
}

address get_sym(char *name) {
  int i;
  
  for (i = 0; i < NSYM; ++i) {
    if (strcmp(symtab[i].name, name) == 0) return symtab[i].value;
  }
  /* should not happen */
  printf("Unknown symbol \"%s\"\n", name);

  return 0;
}

/*--------------------------------------------------------------------*/

/* System image read/write routines */

int load_system(struct FCB *f) {
  system_size = file_read(f, system_image, 65536);
  return system_size;
}

int save_system(struct FCB *f) {
  file_seek(f, 0L);
  if (file_write(f, system_image, system_size) != system_size) {
    printf("Error saving system image\n");
    return 1;
  }
  return 0;
}

byte sys_getb(byte bank, address addr) {
  int a = (int) addr + (int) bank * 4096;
  return system_image[a];
}

address sys_getw(byte bank, address addr) {
  int a = (int) addr + (int) bank * 4096;
  return system_image[a] | (system_image[a+1] << 8);
}

void sys_putb(byte bank, address addr, byte val) {
  int a = (int) addr + (int) bank * 4096;
  system_image[a] = val & 0xff;
}

void sys_putw(byte bank, address addr, address val) {
  int a = (int) addr + (int) bank * 4096;
  system_image[a] = val & 0xff;
  system_image[a+1] = (val >> 8) & 0xff;
}

/*--------------------------------------------------------------------*/

/* Pool management routines */

void pool_init(void) {
  address sysend_addr, systop_addr, poolsize_addr, pool_addr;
  address poolsize, pool_start, pool_end;
  
  sysend_addr = get_sym("SYSEND");
  systop_addr = get_sym("SYSTOP");
  poolsize_addr = get_sym("$POLSZ");
  pool_addr = get_sym("$POOL");
  
  poolsize = sys_getw(0, poolsize_addr);
  if (poolsize != 0) return; /* already initialized */
  
  pool_start = (sysend_addr + 3) & 0xfffc;
  pool_end   = 0xf000 & 0xfffc;  /* being politically correct ;) */
  
  poolsize = pool_end - pool_start;
  sys_putw(0, pool_addr, pool_start);
  sys_putw(0, systop_addr, pool_end);
  sys_putw(0, poolsize_addr, poolsize);
  
  sys_putw(0, pool_start, 0);  /* clear next pointer */
  sys_putw(0, pool_start + 2, poolsize);  /* set size */
}

address pool_alloc(address size) {
  address pool_addr, prev, next, link, blksz;
  
  size = (size + 3) & 0xfffc;  /* ensure 4-byte granularity */
  if (size == 0) {
    printf("Attempting to allocate block of zero size\n");
    return 0;
  }

  pool_addr = get_sym("$POOL");

  prev = pool_addr;
  next = sys_getw(0, prev);
  while (1) {
    if (!next) {
      printf("No pool space\n");
      return 0;
    }
    link  = sys_getw(0, next);  /* get link */
    blksz = sys_getw(0, next + 2);  /* get size */
    if (blksz >= size) break;
    prev = next;
    next = link;
  }
  if (blksz == size) {
    /* exact match, update prev ptr */
    sys_putw(0, prev, link);
  } else {
    sys_putw(0, prev, next + size);  /* set prev ptr to new free block */
    sys_putw(0, next + size, link);  /* set next ptr on new block */
    sys_putw(0, next + size + 2, blksz - size);  /* set size field of new blk */
  }
  return next;
}

void pool_free(address addr, address size) {
  address pool_addr, prev, next, link, blksz;

  size = (size + 3) & 0xfffc;  /* ensure 4-byte granularity */
  if (size == 0) {
    printf("Attempting to free block of zero size\n");
    return;
  }

  pool_addr = get_sym("$POOL");
  
  /* figure out where to reinsert the block */
  prev = pool_addr;
  next = sys_getw(0, prev);
  while (1) {
    next = sys_getw(0, prev);
    if (!next) break;  /* after end of free chain */
    if (next > addr) break;  /* or just here, between prev and next */
    prev = next;
  }
  sys_putw(0, prev, addr);
  sys_putw(0, addr, next);
  sys_putw(0, addr + 2, size);
  
  /* see if we can merge with prev */
  if (prev != pool_addr) {
    blksz = sys_getw(0, prev + 2);
    if (prev + blksz == addr) {
      size += blksz;
      sys_putw(0, prev, next); /* restore */
      sys_putw(0, prev + 2, size);
      addr = prev;
    }
  }
  
  /* see if we can merge with next */
  if (addr + size == next) {
    link = sys_getw(0, next);
    blksz = sys_getw(0, next + 2);
    size += blksz;
    sys_putw(0, addr, link);
    sys_putw(0, addr + 2, size);
  }
}

address pool_avail(void) {
  address poolsize, pool, total;

  poolsize = get_sym("$POLSZ");
  if (!poolsize) return 0;

  total = 0;  
  pool = get_sym("$POOL");
  pool = sys_getw(0, pool);
  
  while (pool) {
    total += sys_getw(0, pool + 2);
    pool = sys_getw(0, pool);
  }
  return total;
}

void pool_stats(char *msg) {
  unsigned short poolsize, pool, size, total, largest, top;
  
  poolsize = get_sym("$POLSZ");
  if (!poolsize) return;

  total = largest = 0;
  top = sys_getw(0, get_sym("SYSTOP"));
  pool = get_sym("$POOL");
  pool = sys_getw(0, pool);
  
  while (pool) {
    size = sys_getw(0, pool + 2);
    total += size;
    if (size > largest) largest = size;
    pool = sys_getw(0, pool);
  }
  if (msg) printf("%s", msg);
  printf("%d:%d:%d\n", top, largest, total);
}

/*--------------------------------------------------------------------*/

/* Device driver routines */

void load_devices(void) {
  address phydev, iddtbl, ddptr, dcb, ucb;
  
  phydev = get_sym("$PHYDV");
  if (sys_getw(0, phydev) != 0) return; /* device drivers already loaded */

  iddtbl = get_sym("IDDTBL");
  ddptr = sys_getw(0, iddtbl);
  while (ddptr) {
    dcb = pool_alloc(DCBSZ);
    if (!dcb) {
      printf("Out of pool space\n");
      return;
    }
    sys_putw(0, phydev + D_LNK, dcb); /* link DCB */
    sys_putw(0, dcb + D_LNK, 0);
    sys_putb(0, dcb + D_ST, 0);
    sys_putw(0, dcb + D_TCNT, 0);
    sys_putw(0, dcb + D_NAME, sys_getw(0, ddptr + 0));
    sys_putb(0, dcb + D_UNITS, sys_getw(0, ddptr + 2));
    sys_putw(0, dcb + D_UCBL, sys_getw(0, ddptr + 3));
    sys_putb(0, dcb + D_BANK, 0);
    sys_putw(0, dcb + D_START, ddptr);
    sys_putw(0, dcb + D_END, 0);
    sys_putw(0, dcb + D_EPT, ddptr + 5);
    
    ucb = sys_getw(0, dcb + D_UCBL);
    while (ucb) {
      sys_putw(0, ucb + U_DCB, dcb); /* set DCB back pointer in UCB */
      ucb = sys_getw(0, ucb + U_LNK);
    }
    phydev = dcb;
    iddtbl += 2;
    ddptr = sys_getw(0, iddtbl);
  }
}

address find_device(char *name) {
  address phydev, ucb;
  byte dname[2], unit;
  
  if (strlen(name) < 3) {
    printf("Invalid device name %s\n", name);
    return 0;
  }
  
  if (name[2] == ':') {
    unit = 0;
  } else {
    unit = atoi(&name[2]);
  }

  phydev = sys_getw(0, get_sym("$PHYDV"));

  while (phydev) {
    dname[0] = sys_getb(0, phydev + D_NAME);
    dname[1] = sys_getb(0, phydev + D_NAME + 1);
    if ((dname[0] == name[0]) && (dname[1] == name[1])) {
      ucb = sys_getw(0, phydev + D_UCBL);
      while (ucb) {
        if (unit == sys_getb(0, ucb + U_UNIT)) return ucb;
        ucb = sys_getw(0, ucb + U_LNK);
      }
    }
    phydev = sys_getw(0, phydev + D_LNK);
  }
  return 0;
}

void assign(char *pdev, char *ldev, byte type, char *ttdev) {
}

void deassign(char *ldev, byte type, char *ttdev) {
}

void set_term_opt(char *name, int bitno, int pol) {
  address phydev, ucb;
  byte dname[2], unit, tc;
  
  if (strlen(name) < 3) {
    printf("Invalid device name %s\n", name);
    return;
  }
  
  if (name[2] == ':') {
    unit = 0;
  } else {
    unit = atoi(&name[2]);
  }

  phydev = sys_getw(0, get_sym("$PHYDV"));

  while (phydev) {
    dname[0] = sys_getb(0, phydev + D_NAME);
    dname[1] = sys_getb(0, phydev + D_NAME + 1);
    if ((dname[0] == name[0]) && (dname[1] == name[1])) {
      ucb = sys_getw(0, phydev + D_UCBL);
      while (ucb) {
        if (unit == sys_getb(0, ucb + U_UNIT)) {
          if ((sys_getb(0, ucb + U_CW) & (1 << DV_TTY)) == 0) {
            printf("Not a terminal device\n");
            return;
          }
          tc = sys_getb(0, ucb + U_CW + 1);
          if (pol) {
            sys_putb(0, ucb + U_CW + 1, tc | (1 << bitno));
          } else {
            sys_putb(0, ucb + U_CW + 1, tc & ~(1 << bitno));
          }
          return;
        }
        ucb = sys_getw(0, ucb + U_LNK);
      }
    }
    phydev = sys_getw(0, phydev + D_LNK);
  }
  printf("No such device %s\n", name);
}

void list_term_opt(char *msg, int bitno, int pol) {
  address phydev, ucb;
  byte dname[2], unit, cw, cw1;
  int match;

  phydev = sys_getw(0, get_sym("$PHYDV"));

  while (phydev) {
    dname[0] = sys_getb(0, phydev + D_NAME);
    dname[1] = sys_getb(0, phydev + D_NAME + 1);
    ucb = sys_getw(0, phydev + D_UCBL);
    while (ucb) {
      unit = sys_getb(0, ucb + U_UNIT);
      cw  = sys_getb(0, ucb + U_CW);
      cw1 = sys_getb(0, ucb + U_CW + 1);
      if (cw & (1 << DV_TTY)) {
        if (pol) {
          match = ((cw1 & (1 << bitno)) != 0);
        } else {
          match = ((cw1 & (1 << bitno)) == 0);
        }
        if (match) {
          printf("%s=%c%c%d:\n", msg, dname[0], dname[1], unit);
        }
      }
      ucb = sys_getw(0, ucb + U_LNK);
    }
    phydev = sys_getw(0, phydev + D_LNK);
  }
}

void set_term_speed(char *name, int speed) {
  address phydev, ucb;
  byte dname[2], unit;
  
  if (strlen(name) < 3) {
    printf("Invalid device name %s\n", name);
    return;
  }
  
  if (name[2] == ':') {
    unit = 0;
  } else {
    unit = atoi(&name[2]);
  }

  phydev = sys_getw(0, get_sym("$PHYDV"));

  while (phydev) {
    dname[0] = sys_getb(0, phydev + D_NAME);
    dname[1] = sys_getb(0, phydev + D_NAME + 1);
    if ((dname[0] == name[0]) && (dname[1] == name[1])) {
      ucb = sys_getw(0, phydev + D_UCBL);
      while (ucb) {
        if (unit == sys_getb(0, ucb + U_UNIT)) {
          if ((sys_getb(0, ucb + U_CW) & (1 << DV_TTY)) == 0) {
            printf("Not a terminal device\n");
            return;
          }
          sys_putb(0, ucb + UX_BDR, speed);
          return;
        }
        ucb = sys_getw(0, ucb + U_LNK);
      }
    }
    phydev = sys_getw(0, phydev + D_LNK);
  }
  printf("No such device %s\n", name);
}

void list_term_speed(char *name) {
  address phydev, ucb;
  byte dname[2], unit, speed;

  if (strlen(name) < 3) {
    printf("Invalid device name %s\n", name);
    return;
  }
  
  if (name[2] == ':') {
    unit = 0;
  } else {
    unit = atoi(&name[2]);
  }

  phydev = sys_getw(0, get_sym("$PHYDV"));

  while (phydev) {
    dname[0] = sys_getb(0, phydev + D_NAME);
    dname[1] = sys_getb(0, phydev + D_NAME + 1);
    if ((dname[0] == name[0]) && (dname[1] == name[1])) {
      ucb = sys_getw(0, phydev + D_UCBL);
      while (ucb) {
        if (unit == sys_getb(0, ucb + U_UNIT)) {
          if ((sys_getb(0, ucb + U_CW) & (1 << DV_TTY)) == 0) {
            printf("Not a terminal device\n");
            return;
          }
          speed = sys_getb(0, ucb + UX_BDR);
          if ((speed > S_115K2) || (speed < 0)) speed = 0;
          printf("SPEED=%c%c%d:%s\n", dname[0], dname[1], unit, speed_str[speed]);
          return;
        }
        ucb = sys_getw(0, ucb + U_LNK);
      }
    }
    phydev = sys_getw(0, phydev + D_LNK);
  }
  printf("No such device %s\n", name);
}

void list_devices(char *name) {
  address phydev, ucb, ctlw;
  byte dname[2], stat;
  int match;

  phydev = sys_getw(0, get_sym("$PHYDV"));

  while (phydev) {
    dname[0] = sys_getb(0, phydev + D_NAME);
    dname[1] = sys_getb(0, phydev + D_NAME + 1);
    if (name && *name) {
      match = ((dname[0] == name[0]) && (dname[1] == name[1]));
    } else {
      match = 1;
    }
    ucb = sys_getw(0, phydev + D_UCBL);
    while (ucb) {
      if (match) {
        printf("%c%c%d: ", dname[0], dname[1], sys_getb(0, ucb + U_UNIT));
        stat = sys_getb(0, ucb + U_ST);
        ctlw = sys_getw(0, ucb + U_CW);
        if ((ctlw & (1 << DV_PSE)) == 0) {
          if (stat & (1 << US_PUB)) printf("Public ");
          printf("Loaded");
        }
        printf("\n");
      }
      ucb = sys_getw(0, ucb + U_LNK);
    }
    phydev = sys_getw(0, phydev + D_LNK);
  }
}

void list_devices_opt(char *msg, int bitno, int pol) {
  address phydev, ucb;
  byte dname[2], unit, stat;
  int match;

  phydev = sys_getw(0, get_sym("$PHYDV"));

  while (phydev) {
    dname[0] = sys_getb(0, phydev + D_NAME);
    dname[1] = sys_getb(0, phydev + D_NAME + 1);
    ucb = sys_getw(0, phydev + D_UCBL);
    while (ucb) {
      unit = sys_getb(0, ucb + U_UNIT);
      stat = sys_getb(0, ucb + U_ST);
      if (pol) {
        match = ((stat & (1 << bitno)) != 0);
      } else {
        match = ((stat & (1 << bitno)) == 0);
      }
      if (match) {
        printf("%s=%c%c%d:\n", msg, dname[0], dname[1], unit);
      }
      ucb = sys_getw(0, ucb + U_LNK);
    }
    phydev = sys_getw(0, phydev + D_LNK);
  }
}

/*--------------------------------------------------------------------*/

/* Task-related routines */

address find_task(char *name) {
  address poolsize, tlist, prev;
  char tname[6];
  int i;

  poolsize = sys_getw(0, get_sym("$POLSZ"));
  if (poolsize == 0) return 0; /* virgin system - no tasks installed yet */

  prev = get_sym("$TLIST");
  tlist = sys_getw(0, prev);

  while (tlist) {
    for (i = 0; i < 6; ++i) tname[i] = sys_getb(0, tlist + T_NAME + i);
    if (strncmp(name, tname, 6) == 0) return tlist;
    prev = tlist + T_TCBL;
    tlist = sys_getw(0, prev);
  }
  return 0;
}

void install_task(char *name, int argc, char *argv[]) {
  struct FCB *fcb;
  byte attr, thdr[THSZ];
  address tcb, tlist, prev, pcb, ucb, tiucb;
  unsigned long blkno, nblks, tskend;
  char *p, filename[256], pname[6], tname[6];
  int i, len, pri, inc, ckd, cli, acp, prv;

  p = name;
  filename[0] = '\0';
  if (*p == '$') {
    ++p;
    /*strcpy(filename, "LB:[SYSTEM]");*/
    strcpy(filename, "[SYSTEM]");
  }
  strcat(filename, p);
  p = strchr(filename, '.');
  if (!p) strcat(filename, ".TSK");

  pname[0] = tname[0] = '\0';
  pri = inc = 0;
  cli = acp = ckd = prv = -1;
  for (i = 0; i < argc; ++i) {
    if (strncmp(argv[i], "PAR=", 4) == 0) {
      len = strlen(argv[i] + 4);
      if (len > 6) len = 6;
      strncpy(pname, argv[i] + 4, len);
      while (len < 6) pname[len++] = ' ';
    } else if (strncmp(argv[i], "PRI=", 4) == 0) {
      pri = atoi(argv[i] + 4);
      if ((pri < 0) || (pri > 250)) {
        printf("Invalid priority value\n");
        return;
      }
    } else if (strncmp(argv[i], "INC=", 4) == 0) {
      inc = atoi(argv[i] + 4);
      if ((inc < 0) || (inc > 65535-4096)) {
        printf("Invalid increment value\n");
        return;
      }
    } else if (strncmp(argv[i], "TASK=", 5) == 0) {
      len = strlen(argv[i] + 5);
      if (len > 6) len = 6;
      strncpy(tname, argv[i] + 5, len);
      while (len < 6) tname[len++] = ' ';
    } else if (strncmp(argv[i], "CKP=YES", 7) == 0) {
      ckd = 0;
    } else if (strncmp(argv[i], "CKP=NO", 6) == 0) {
      ckd = 1;
    } else if (strncmp(argv[i], "CLI=YES", 7) == 0) {
      cli = 1;
    } else if (strncmp(argv[i], "ACP=YES", 7) == 0) {
      acp = 1;
    } else if (strncmp(argv[i], "PRIV", 4) == 0) {
      prv = 1;
    } else {
      printf("Unknown option switch\n");
      return;
    }
  }

  fcb = open_file(filename);
  if (!fcb) {
    printf("File not found\n");
    return;
  }

  if (!(fcb->header->attrib & _FA_CTG)) {
    printf("File not contiguous\n");
    close_file(fcb);
    free_fcb(fcb);
    return;
  }
  
  blkno = fcb->header->bmap[0];
  nblks = fcb->header->nused;

  if (file_read(fcb, thdr, THSZ) != THSZ) {
    printf("Error reading Task Header\n");
    close_file(fcb);
    free_fcb(fcb);
    return;
  }

  close_file(fcb);
  free_fcb(fcb);
  
  //printf("Install device not LB0:\n");
  ucb = find_device("LB0");
  if (!ucb) {
    printf("No such device LB0:\n");
    return;
  }
  
  tiucb = find_device("CO0");
  if (!tiucb) {
    printf("No such device CO0:\n");
    return;
  }
  
  if (strncmp((char *) &thdr[TH_HDR], "TSK180", 6) != 0) {
    printf("Invalid Task Header\n");
    return;
  }
  
  if ((thdr[TH_VER] != 2) && (thdr[TH_VER + 1] != 1)) {
    printf("Invalid Task File version\n");
    return;
  }

  /* check for existing installed task with the same name */
  if (find_task((char *) &thdr[TH_NAME])) {
    printf("Task name in use\n");
    return;
  }

  if (!*pname) for (i = 0; i < 6; ++i) pname[i] = thdr[TH_PAR + i];
  pcb = find_partition(pname);
  if (!pcb) {
    printf("Partition not in system\n");
    return;
  }
  
  tcb = pool_alloc(TCBSZ);
  if (!tcb) {
    printf("Out of pool space\n");
    return;
  }
  
  sys_putw(0, tcb + T_LNK, 0);
  sys_putw(0, tcb + T_ACTL, 0);
  attr = thdr[TH_ATTR];
  if (prv == 0) attr &= ~(1 << TA_PRV); else if (prv == 1) attr |= (1 << TA_PRV);
  if (ckd == 0) attr &= ~(1 << TA_CKD); else if (ckd == 1) attr |= (1 << TA_CKD);
  if (cli == 0) attr &= ~(1 << TA_CLI); else if (cli == 1) attr |= (1 << TA_CLI);
  if (acp == 0) attr &= ~(1 << TA_ACP); else if (acp == 1) attr |= (1 << TA_ACP);
  sys_putb(0, tcb + T_ATTR, attr);
  sys_putw(0, tcb + T_ST, 0);
  if (pri == 0) pri = thdr[TH_PRI];
  sys_putb(0, tcb + T_DPRI, pri);
  sys_putb(0, tcb + T_PRI, pri);
  if (*tname) p = tname; else p = (char *) &thdr[TH_NAME];
  for (i = 0; i < 6; ++i) sys_putb(0, tcb + T_NAME + i, (byte) *p++);
  for (i = 0; i < 6; ++i) sys_putb(0, tcb + T_VID + i, thdr[TH_VID + i]);
  sys_putw(0, tcb + T_CMD, 0);
  sys_putb(0, tcb + T_IOC, 0);
  sys_putw(0, tcb + T_RCVL, 0);
  sys_putw(0, tcb + T_OCBL, 0);
  sys_putw(0, tcb + T_ASTL, 0);
  sys_putw(0, tcb + T_SAST, 0);
  for (i = 0; i < 4; ++i) sys_putb(0, tcb + T_FLGS + i, 0);
  for (i = 0; i < 4; ++i) sys_putb(0, tcb + T_WAIT + i, 0);
  sys_putw(0, tcb + T_CTX, 0);
  sys_putw(0, tcb + T_TI, tiucb);

  sys_putw(0, tcb + T_LDEV, ucb);
#if 1
  sys_putw(0, tcb + T_SBLK, blkno & 0xFFFF);
  sys_putw(0, tcb + T_SBLK + 2, (blkno >> 16) & 0xFFFF);
#else
  sys_putw(0, tcb + T_SBLK, fcb->inode);
  sys_putw(0, tcb + T_SBLK + 2, 0);
#endif
  sys_putw(0, tcb + T_NBLK, nblks);
  sys_putw(0, tcb + T_PCB, pcb);
  sys_putw(0, tcb + T_CPCB, 0);
  tskend = thdr[TH_END] | (thdr[TH_END+1] << 8);
  if (inc == 0) inc = thdr[TH_INC] | (thdr[TH_INC+1] << 8);
  tskend += inc;
  if (tskend > ((system_type == 2) ? 0x10000 : 0xf000)) {
    printf("Program too big\n");
    return;
  }
  sys_putw(0, tcb + T_STRT, thdr[TH_STRT] | (thdr[TH_STRT+1] << 8));
  sys_putw(0, tcb + T_END, tskend);
  sys_putw(0, tcb + T_DEND, tskend);
  sys_putw(0, tcb + T_SP, tskend + 1);
  sys_putw(0, tcb + T_EPT, thdr[TH_EPT] | (thdr[TH_EPT+1] << 8));
  sys_putb(0, tcb + T_SVST, 0);
  
  /* link TCB */
  prev = get_sym("$TLIST");
  tlist = sys_getw(0, prev);

  while (tlist) {
    if (sys_getb(0, tlist + T_PRI) < thdr[TH_PRI]) break;
    prev = tlist + T_TCBL;
    tlist = sys_getw(0, prev);
  }
  sys_putw(0, prev, tcb);
  sys_putw(0, tcb + T_TCBL, tlist);
}

void remove_task(char *name) {
  address poolsize, tlist, prev, next;
  byte attr;
  char tname[6];
  int i;

  poolsize = sys_getw(0, get_sym("$POLSZ"));
  if (poolsize == 0) return; /* virgin system - no tasks installed yet */

  prev = get_sym("$TLIST");
  tlist = sys_getw(0, prev);

  while (tlist) {
    attr = sys_getb(0, tlist + T_ATTR);
    for (i = 0; i < 6; ++i) tname[i] = sys_getb(0, tlist + T_NAME + i);
    if (strncmp(name, tname, 6) == 0) {
      if (attr & (1 << TA_FIX)) {
        printf("Task fixed in memory\n");
        return;
      }
      /* unlink TCB */
      next = sys_getw(0, tlist + T_TCBL);
      sys_putw(0, prev, next);
      /* free TCB */
      pool_free(tlist, TCBSZ);
      return;
    }
    prev = tlist + T_TCBL;
    tlist = sys_getw(0, prev);
  }
  printf("Task not in system\n");
}

static address task_size(address tcb) {
  address nblks, size;
  int dsz, bsz;

  nblks = sys_getw(0, tcb + T_NBLK);
  dsz = ((int) nblks + 7) / 8;  /* convert disk blocks to pages */

  size = sys_getw(0, tcb + T_END);
  bsz = ((int) size + 4095) / 4096; /* convert size in byte to pages */

  size = (address) ((dsz > bsz) ? dsz : bsz);
  if (size == 0) {
    printf("Task has zero size\n");
    return 0;
  }
  if (size > ((system_type == 2) ? 16 : 15)) {
    printf("Task too large\n");
    return 0;
  }
  return size;
}

static int load_task(address tcb) {
  int  i, sblk, nblk;
  address addr, pcb;
  byte bank, buf[512];

  sblk = sys_getw(0, tcb + T_SBLK) + (sys_getw(0, tcb + T_SBLK + 2) << 16);
  nblk = sys_getw(0, tcb + T_NBLK);
  pcb = sys_getw(0, tcb + T_PCB);
  bank = sys_getb(0, pcb + P_BASE);
  if (read_block(sblk++, buf)) {
    printf("Task load error\n");
    return 1;
  }
  /* TODO: validate header, etc */
  addr = sys_getw(0, tcb + T_STRT);
  for (i = 0; i < 256; ++i) sys_putb(bank, addr++, buf[256+i]);
  while (--nblk > 0) {
    if (read_block(sblk++, buf)) {
      printf("Task load error\n");
      return 1;
    }
    for (i = 0; i < 512; ++i) sys_putb(bank, addr++, buf[i]);
  }
  return 0;
}

void fix_task(char *name) {
  address poolsize, tcb, size, mainpcb, subpcb, ctx;
  byte stat, attr, bank;
  int i, ctxsz;
  
  /* We are loading the task directly here. Alternatively, we could
     just allocate the PCB, set the TA_FIX bit and place the TCB in
     the loader queue. */

  poolsize = sys_getw(0, get_sym("$POLSZ"));
  if (poolsize == 0) return; /* virgin system - no tasks installed yet */

  tcb = find_task(name);
  if (!tcb) {
    printf("Task not in system\n");
    return;
  }

  attr = sys_getb(0, tcb + T_ATTR);
  if (attr & (1 << TA_FIX)) {
    printf("Task already fixed\n");
    return;
  }
  size = task_size(tcb);  /* get task size in pages */
  if (!size) return;
      
  mainpcb = sys_getw(0, sys_getw(0, tcb + T_PCB) + P_MAIN);
  attr = sys_getb(0, mainpcb + P_ATTR);
  if ((attr & (1 << PA_SYS)) == 0) {
    /* not system-controlled */
    stat = sys_getb(0, mainpcb + P_STAT);
    if ((stat & (1 << PS_BSY)) != 0) {
      printf("Partition busy\n");
      return;
    }
    if (sys_getb(0, mainpcb + P_SIZE) < size) {
      printf("No space\n");
      return;
    }
    sys_putb(0, mainpcb + P_STAT, (1 << PS_BSY));
    subpcb = mainpcb;
  } else {
    /* system-controlled, allocate sub-partition */
    subpcb = alloc_sub_partition(mainpcb, (byte) size);
    if (!subpcb) {
      printf("No space\n");
      return;
    }
  }
  sys_putw(0, tcb + T_PCB, subpcb);
  sys_putw(0, subpcb + P_TCB, tcb);
  /* setup zero-page vectors */
  bank = sys_getb(0, subpcb + P_BASE);
  for (i = 0; i < 8; ++i) {
    if (system_type != 2) {
      sys_putb(bank, i*8, 0xC3);
      sys_putw(bank, i*8+1, get_sym("CHKTRP"));
    } else {
      sys_putb(bank, i*8, 0x76);
    }
  }
  if (system_type != 2) {
    sys_putb(bank, SYSRST, 0xC3);
    sys_putw(bank, SYSRST+1, get_sym("SYSENT"));
    sys_putb(bank, DBGRST, 0xC3);
    sys_putw(bank, DBGRST+1, get_sym("$DBTRP"));
  }
  /* allocate context block */
  ctxsz = (system_type == 2) ? CTX280SZ : CTX180SZ;
  ctx = pool_alloc(ctxsz);
  if (!ctx) {
    // ...delete subpartition
  }
  sys_putw(0, tcb + T_CTX, ctx);
  for (i = 0; i < ctxsz; ++i) sys_putb(0, ctx + i, 0);
  attr = sys_getb(0, tcb + T_ATTR);
  attr |= (1 << TA_FIX);
  sys_putb(0, tcb + T_ATTR, attr);
  /* load task */
  if (load_task(tcb)) {
    // ...delete subpartition
    // ...delete context block
  }
  i = (int) bank * 4096 + (int) size * 4096;
  if (i > system_size) system_size = i;
}

void unfix_task(char *name) {
}

void list_tasks(char *name) {
  address poolsize, tcb, pcb, ucb, dcb;
  unsigned long sblk, size; //, nblk;
  byte pri, attr;
  char tname[6], ident[6], par[6], dv[3];
  int i;

  poolsize = sys_getw(0, get_sym("$POLSZ"));
  if (poolsize == 0) return; /* virgin system - no tasks installed yet */

  tcb = sys_getw(0, get_sym("$TLIST"));
  while (tcb) {
    if (sys_getb(0, tcb + T_NAME) != '*') {
      attr = sys_getb(0, tcb + T_ATTR);
      for (i = 0; i < 6; ++i) tname[i] = sys_getb(0, tcb + T_NAME + i);
      for (i = 0; i < 6; ++i) ident[i] = sys_getb(0, tcb + T_VID + i);
      pcb = sys_getw(0, tcb + T_PCB);
      pcb = sys_getw(0, pcb + P_MAIN);
      for (i = 0; i < 6; ++i) par[i] = sys_getb(0, pcb + P_NAME + i);
      pri = sys_getb(0, tcb + T_PRI);
      ucb = sys_getw(0, tcb + T_LDEV);
      dcb = sys_getw(0, ucb + U_DCB);
      dv[0] = sys_getb(0, dcb + D_NAME);
      dv[1] = sys_getb(0, dcb + D_NAME + 1);
      dv[2] = sys_getb(0, ucb + U_UNIT);
      sblk = sys_getw(0, tcb + T_SBLK) + (sys_getw(0, tcb + T_SBLK + 2) << 16);
      //nblk = sys_getw(0, tcb + T_NBLK);
      size = sys_getw(0, tcb + T_DEND) + 1;
      
      if (!name || !*name || (strncmp(name, tname, 6) == 0)) {
#if 1
        printf("%.6s %.6s %04X %.6s %3d %08lX %c%c%d:%08lX %s\n",
               tname, ident, tcb, par, pri, size, dv[0], dv[1], dv[2],
               sblk, (attr & (1 << TA_FIX)) ? "FIXED" : "");
#else
        printf("%.6s %.6s %04X %.6s %3d %08lX %c%c%d:- FILE ID:%ld %s\n",
               tname, ident, tcb, par, pri, size, dv[0], dv[1], dv[2],
               sblk, (attr & (1 << TA_FIX)) ? "FIXED" : "");
#endif
      }
    }
    tcb = sys_getw(0, tcb + T_TCBL);
  }
}

/*--------------------------------------------------------------------*/

/* Partition-related routines */

address find_partition(char *name) {
  address poolsize, pcb;
  char pname[6];
  int i;

  poolsize = sys_getw(0, get_sym("$POLSZ"));
  if (poolsize == 0) return 0; /* virgin system - no partitions yet */

  pcb = sys_getw(0, get_sym("$PLIST"));
  while (pcb) {
    for (i = 0; i < 6; ++i) pname[i] = sys_getb(0, pcb + P_NAME + i);
    if (strncmp(name, pname, 6) == 0) return pcb;
    pcb = sys_getw(0, pcb + P_LNK);
  }
  return 0;
}

#if 0
void add_partition(char *name, address base, address size, byte type) {
  address poolsize, plist, pbase, psize, prev, pcb;
  int i;
  
  if (size == 0) {
    printf("Partition can't have zero size\n");
    return;
  }
  
  poolsize = sys_getw(0, get_sym("$POLSZ"));
  if (poolsize == 0) return; /* virgin system - has not been setup yet */

  /* check for existing partition with the same name */
  pcb = find_partition(name);
  if (pcb) {
    printf("Partition name in use\n");
    return;
  }

  /* find a place to insert the partition, keep the list sorted by address */
  prev = get_sym("$PLIST");
  plist = sys_getw(0, prev);
  while (plist) {
    pbase = sys_getw(0, plist + P_BASE);
    psize = sys_getw(0, plist + P_SIZE);

    if (pbase + psize > base) {
      printf("Partition alignment error\n");
      return;
    }
    
    if (pbase > base) {
      if (base + size > pbase) {
        printf("Partition alignment error\n");
        return;
      }
      break;
    }
    prev = plist + P_LNK;
    plist = sys_getw(0, prev);
  }
  
  /* create main partition */
  pcb = pool_alloc(PCBSZ);
  if (!pcb) {
    printf("Out of pool space\n");
    return;
  }
  sys_putw(0, pcb + P_BASE, base);
  sys_putw(0, pcb + P_SIZE, size);
  sys_putw(0, pcb + P_MAIN, pcb);
  for (i = 0; i < 6; ++i) sys_putb(0, pcb + P_NAME + i, name[i]);
  sys_putw(0, pcb + P_SUB, 0);
  sys_putb(0, pcb + P_ATTR, type);
  sys_putb(0, pcb + P_STAT, 0);
  sys_putw(0, pcb + P_TCB, 0);

  /* link partition into partition list */  
  sys_putw(0, prev + P_LNK, pcb);
  sys_putw(0, pcb + P_LNK, plist);
}
#else
void add_partition(char *name, address base, address size, byte wcmask,
                   byte type) {
  address poolsize, plist, pbase, psize, prev, pcb;
  int i;
  
  if ((size == 0) && ((wcmask & WC_SIZE) == 0)) {
    printf("Partition can't have zero size\n");
    return;
  }
  
  poolsize = sys_getw(0, get_sym("$POLSZ"));
  if (poolsize == 0) return; /* virgin system - has not been setup yet */

  /* check for existing partition with the same name */
  pcb = find_partition(name);
  if (pcb) {
    printf("Partition name in use\n");
    return;
  }
  
  if (wcmask & WC_BASE) base = 16;   /* start after kernel partition */
  if (wcmask & WC_SIZE) size = 256;  /* 1Mb system */

  /* find a place to insert the partition, keep the list sorted by address */
  prev = get_sym("$PLIST");
  plist = sys_getw(0, prev);
  while (plist) {
    pbase = sys_getw(0, plist + P_BASE);
    psize = sys_getw(0, plist + P_SIZE);
    
    if (wcmask & WC_BASE) {
      /* wildcard base, check for gap */
      if (pbase > base) {
        /* gap found */
        if (wcmask & WC_SIZE) {
          /* wildcard size, make the partition as large as the gap */
          size = pbase; /* - base; - will be subtracted below */
          break;
        } else if (pbase - base >= size) {
          /* size specified and partition fits in gap */
          break;
        }
      }
      base = pbase + psize;
      if (base >= 256) {
        printf("Partition alignment error\n");
        return;
      }
    } else {
      /* base specified */
      if (pbase > base) {
        if (wcmask & WC_SIZE) {
          /* wildcard size, make the partition as large as possible */
          size = pbase; /* - base; - will be subtracted below */
          break;
        } else if (pbase - base >= size) {
          /* size specified and partition fits in gap */
          break;
        }
        printf("Partition alignment error\n");
        return;
      }
      if (pbase + psize > base) {
        printf("Partition alignment error\n");
        return;
      }
    }

    prev = plist + P_LNK;
    plist = sys_getw(0, prev);
  }
  
  if (wcmask & WC_SIZE) size -= base;
  
  /* create main partition */
  pcb = pool_alloc(PCBSZ);
  if (!pcb) {
    printf("Out of pool space\n");
    return;
  }

  sys_putw(0, pcb + P_BASE, base);
  sys_putw(0, pcb + P_SIZE, size);
  sys_putw(0, pcb + P_MAIN, pcb);
  for (i = 0; i < 6; ++i) sys_putb(0, pcb + P_NAME + i, name[i]);
  sys_putw(0, pcb + P_SUB, 0);
  sys_putw(0, pcb + P_WAIT, 0);
  sys_putb(0, pcb + P_ATTR, type);
  sys_putb(0, pcb + P_STAT, 0);
  sys_putw(0, pcb + P_TCB, 0);

  /* link partition into partition list */  
  sys_putw(0, prev + P_LNK, pcb);
  sys_putw(0, pcb + P_LNK, plist);
}
#endif

void remove_partition(char *name) {
  address poolsize, pcb, prev, next, p;
  char pname[6];
  int i;

  poolsize = sys_getw(0, get_sym("$POLSZ"));
  if (poolsize == 0) return; /* virgin system - no partitions yet */

  prev = get_sym("$PLIST");
  pcb = sys_getw(0, prev);
  while (pcb) {
    for (i = 0; i < 6; ++i) pname[i] = sys_getb(0, pcb + P_NAME + i);
    if (strncmp(name, pname, 6) == 0) {
      p = sys_getw(0, pcb + P_SUB);
      if (p) {
        printf("Partition not empty\n");
        return;
      }
      p = sys_getw(0, pcb + P_WAIT);
      if (p) {
        printf("Partition wait list not empty\n");
        return;
      }
      /* unlink PCB */
      next = sys_getw(0, pcb + P_LNK);
      sys_putw(0, prev, next);
      /* free PCB */
      pool_free(pcb, PCBSZ);
      return;
    }
    prev = pcb + P_LNK;
    pcb = sys_getw(0, prev);
  }
  printf("Partition not in system\n");
}

static int find_gap(address mainpcb, byte size,
                    address *prvlnk, address *next, byte *base) {
  byte attr, hsize;
  
  attr = sys_getb(0, mainpcb + P_ATTR);
  if ((attr & (1 << PA_SYS)) == 0) return 0;  /* not system-controlled */

  *prvlnk = mainpcb + P_SUB;  
  *base = sys_getb(0, mainpcb + P_BASE);  /* remember base */
  
  *next = sys_getw(0, mainpcb + P_SUB);
  if (!*next) {
    /* no subpartitions yet */
    hsize = sys_getb(0, mainpcb + P_SIZE);
    return (hsize >= size);
  }
  
  /* loop over subpartitions */
  while (*next) {
    hsize = sys_getb(0, *next + P_BASE) - *base;
    if (hsize >= size) return 1; /* big enough */
    *prvlnk = *next + P_LNK;
    *base = sys_getb(0, *next + P_BASE) + sys_getb(0, *next + P_SIZE);
    *next = sys_getw(0, *prvlnk);
  }
  /* end of subpartition list */
  hsize = sys_getb(0, mainpcb + P_SIZE) + sys_getb(0, mainpcb + P_BASE) - *base;
  return (hsize >= size);
}

address alloc_sub_partition(address mainpcb, byte size) {
  address prvlnk, next, subpcb;
  byte base;
  int i;

  if (!find_gap(mainpcb, size, &prvlnk, &next, &base)) return 0; /* no space */
  
  subpcb = pool_alloc(PCBSZ);
  if (!subpcb) return 0;  /* no pool space */
  
  for (i = 0; i < 6; ++i) sys_putb(0, subpcb + P_NAME + i, ' ');
  sys_putb(0, subpcb + P_ATTR, (1 << PA_SUB));
  sys_putb(0, subpcb + P_STAT, (1 << PS_BSY));
  sys_putb(0, subpcb + P_BASE, base);
  sys_putb(0, subpcb + P_SIZE, size);
  sys_putw(0, subpcb + P_MAIN, mainpcb);
  
  /* link PCB */
  sys_putw(0, subpcb + P_LNK, next);
  sys_putw(0, prvlnk, subpcb);
  
  return subpcb;
}

#if 0
void find_main_gap(address *base, address *size) {
  address poolsize, plist, pbase, psize;

  *base = 0;  /* = 16 to exclude kernel partition */
  *size = 0;

  poolsize = sys_getw(0, get_sym("$POLSZ"));
  if (poolsize == 0) return; /* virgin system - no partitions yet */

  *size = 256;  /* P112 with 1Mb RAM */

  plist = sys_getw(0, get_sym("$PLIST"));
  while (plist) {
    pbase = sys_getw(0, plist + P_BASE);
    psize = sys_getw(0, plist + P_SIZE);
    
    if (pbase > *base) {
      /* Hole found */
      *size = pbase - *base;
      return;
    }
    
    *base = pbase + psize;
    plist = sys_getw(0, plist + P_LNK);
  }
  
  *size -= *base;
}
#endif

void list_partitions(void) {
  address poolsize, plist, sublist, pbase, psize, tcb;
  byte attr;
  char pname[6], tname[6];
  int i;

  poolsize = sys_getw(0, get_sym("$POLSZ"));
  if (poolsize == 0) return; /* virgin system - no partitions yet */

  plist = sys_getw(0, get_sym("$PLIST"));

  while (plist) {
    attr = sys_getb(0, plist + P_ATTR);
    for (i = 0; i < 6; ++i) pname[i] = sys_getb(0, plist + P_NAME + i);
    pbase = sys_getw(0, plist + P_BASE);
    psize = sys_getw(0, plist + P_SIZE);
    
    printf("%.6s %04X %03X000 %03X000 %-4s %-4s\n",
           pname, plist, pbase, psize,
           (attr & (1 << PA_SUB)) ? "SUB" : "MAIN",
           (attr & (1 << PA_SYS)) ? "SYS" : "TASK");
    sublist = sys_getw(0, plist + P_SUB);
    while (sublist) {
      attr = sys_getb(0, sublist + P_ATTR);
      for (i = 0; i < 6; ++i) pname[i] = sys_getb(0, sublist + P_NAME + i);
      pbase = sys_getw(0, sublist + P_BASE);
      psize = sys_getw(0, sublist + P_SIZE);
      tcb = sys_getw(0, sublist + P_TCB);
      if (tcb) {
        for (i = 0; i < 6; ++i) tname[i] = sys_getb(0, tcb + T_NAME + i);
      }
      printf("%.6s %04X %03X000 %03X000 %-4s (%.6s)\n",
              pname, sublist, pbase, psize,
              (attr & (1 << PA_SUB)) ? "SUB" : "MAIN", tname);
      sublist = sys_getw(0, sublist + P_LNK);
    }
    plist = sys_getw(0, plist + P_LNK);
  }
}

/*-----------------------------------------------------------------------*/

/* Open system image */
int open_system_image(char *imgfile, char *symfile) {
  struct FCB *fcb;
  unsigned char b1, b2;
  unsigned short addr;
  char *p, *signature = "SYSDAT";
  int i, syssz;
  
  fcb = open_file(imgfile);
  if (fcb) {
    syssz = load_system(fcb);
    close_file(fcb);
    free_fcb(fcb);
  } else {
    printf("Could not open system image file\n");
    return 1;
  }
  
  fcb = open_file(symfile);
  if (fcb) {
    load_symbols(fcb);
    close_file(fcb);
    free_fcb(fcb);
  } else {
    printf("Could not open symbol file\n");
    return 1;
  }
  
  /* validate signature */
  addr = get_sym("SYSDAT");
  p = signature;
  for (i = 0; i < 6; ++i) {
    if (sys_getb(0, addr++) != *p++) {
      printf("Invalid system image signature\n");
      return 1;
    }
  }
  addr = get_sym("SYSVER");
  b2 = sys_getb(0, addr++);
  b1 = sys_getb(0, addr);
  p = (system_type == 2) ? "RSX280" : "RSX180";
  printf("System image %s V%d.%02d, size 0%04Xh", p, b1, b2, syssz);
  addr = get_sym("$POLSZ");
  addr = sys_getw(0, addr);
  if (addr == 0) printf(", not yet configured");
  printf("\n");

  pool_init();
  load_devices();
  
  return 0;
}

/* Save system image */
int save_system_image(char *imgfile) {
  struct FCB *fcb;

#if 0
  fcb = create_file(imgfile, 1, 1, 0, 0);
#else
  fcb = open_file(imgfile);
#endif
  if (fcb) {
    save_system(fcb);
    close_file(fcb);
    free_fcb(fcb);
  } else {
    printf("Could not open/create system image file\n");
    return 0;
  }
  
  return 1;
}

/*-----------------------------------------------------------------------*/

int get_line(struct FCB *fcb, char *str, int maxlen) {
  int c, len;
  
  len = 0;
  for (;;) {
    c = 0;
    if (file_read(fcb, (byte *) &c, 1) != 1) break;
    if (c == '\r') continue;
    if (c == '\n') break;
    if (len < maxlen) str[len++] = c;
  }
  str[len] = '\0';
  return len;
}

int vmr_command(char *cmd, char *args) {
  char *p, *argv[20];
  int  argc;

  p = args;
  for (argc = 0; argc < 19; ++argc) {
    p = strchr(p, '/');
    if (p) *p++ = '\0'; else break;
    argv[argc] = p;
    strupr(argv[argc]);
  }
  argv[argc] = 0;

  strupr(cmd);
  if (strcmp(cmd, "INS") == 0) {
    strupr(args);
    install_task(args, argc, argv);
  } else if (strcmp(cmd, "REM") == 0) {
    int i = strlen(args);
    strupr(args);
    for (; i < 6; ++i) args[i] = ' ';
    remove_task(args);
  } else if (strcmp(cmd, "FIX") == 0) {
    int i = strlen(args);
    strupr(args);
    for (; i < 6; ++i) args[i] = ' ';
    fix_task(args);
  } else if (strcmp(cmd, "TAS") == 0) {
    strupr(args);
    list_tasks(args);
  } else if (strcmp(cmd, "SET") == 0) {
    unsigned short base, size;
    unsigned char type, wcmask;
    char name[10];
    int i, err;
      
    if (argv[0]) {
      if (strncmp(argv[0], "PAR=", 4) == 0) {
        err = 0;
        base = size = 0;
        wcmask = 0;
        p = argv[0] + 4;
        for (i = 0; i < 6; ++i) {
          if (*p && (*p != ':')) name[i] = *p++; else name[i] = ' ';
        }
        if (*p == ':') ++p; else err = 1;
        if (*p == '*') wcmask |= WC_BASE, ++p; else base = strtol(p, &p, 10);
        if (*p == ':') ++p; else err = 1;
        if (*p == '*') wcmask |= WC_SIZE, ++p; else size = strtol(p, &p, 10);
        if (*p == ':') ++p; else err = 1;
        if (strcmp(p, "SYS") == 0) type = (1 << PA_SYS); else type = 0;
        if (!err) {
          add_partition(name, base, size, wcmask, type);
        } else {
          fprintf(stderr, "Syntax error\n");
        }
      } else if (strncmp(argv[0], "NOPAR=", 6) == 0) {
        p = argv[0] + 6;
        for (i = 0; i < 6; ++i) {
          if (*p) name[i] = *p++; else name[i] = ' ';
        }
        remove_partition(name);
      } else if (strncmp(argv[0], "ECHO", 4) == 0) {
        if (argv[0][4] == '=') {
          set_term_opt(&argv[0][5], TC_NEC, 0);
        } else {
          list_term_opt("ECHO", TC_NEC, 0);
        }
      } else if (strncmp(argv[0], "NOECHO", 6) == 0) {
        if (argv[0][6] == '=') {
          set_term_opt(&argv[0][7], TC_NEC, 1);
        } else {
          list_term_opt("NOECHO", TC_NEC, 1);
        }
      } else if (strncmp(argv[0], "LOWER", 5) == 0) {
        if (argv[0][5] == '=') {
          set_term_opt(&argv[0][6], TC_SMR, 1);
        } else {
          list_term_opt("LOWER", TC_SMR, 1);
        }
      } else if (strncmp(argv[0], "NOLOWER", 7) == 0) {
        if (argv[0][7] == '=') {
          set_term_opt(&argv[0][8], TC_SMR, 0);
        } else {
          list_term_opt("NOLOWER", TC_SMR, 0);
        }
      } else if (strncmp(argv[0], "CRT", 3) == 0) {
        if (argv[0][3] == '=') {
          set_term_opt(&argv[0][4], TC_SCP, 1);
        } else {
          list_term_opt("CRT", TC_SCP, 1);
        }
      } else if (strncmp(argv[0], "NOCRT", 5) == 0) {
        if (argv[0][5] == '=') {
          set_term_opt(&argv[0][6], TC_SCP, 0);
        } else {
          list_term_opt("NOCRT", TC_SCP, 0);
        }
      } else if (strncmp(argv[0], "SLAVE", 5) == 0) {
        if (argv[0][5] == '=') {
          set_term_opt(&argv[0][6], TC_SLV, 1);
        } else {
          list_term_opt("SLAVE", TC_SLV, 1);
        }
      } else if (strncmp(argv[0], "NOSLAVE", 7) == 0) {
        if (argv[0][7] == '=') {
          set_term_opt(&argv[0][8], TC_SLV, 0);
        } else {
          list_term_opt("NOSLAVE", TC_SLV, 0);
        }
      } else if (strncmp(argv[0], "BRO", 3) == 0) {
        if (argv[0][3] == '=') {
          set_term_opt(&argv[0][4], TC_NBR, 0);
        } else {
          list_term_opt("BRO", TC_NBR, 0);
        }
      } else if (strncmp(argv[0], "NOBRO", 5) == 0) {
        if (argv[0][5] == '=') {
          set_term_opt(&argv[0][6], TC_NBR, 1);
        } else {
          list_term_opt("NOBRO", TC_NBR, 1);
        }
      } else if (strncmp(argv[0], "PUB", 3) == 0) {
        if (argv[0][3] == '=') {
        } else {
          list_devices_opt("PUB", US_PUB, 1);
        }
      } else if (strncmp(argv[0], "NOPUB", 5) == 0) {
        if (argv[0][5] == '=') {
        } else {
          list_devices_opt("NOPUB", US_PUB, 0);
        }
      } else if (strncmp(argv[0], "SPEED", 5) == 0) {
        if (argv[0][5] == '=') {
          p = strchr(argv[0] + 6, ':');
          if (!p) {
            printf("Syntax error\n");
            return 1;
          }
          ++p;
          i = p - (argv[0] + 6);
          if (i > 9) i = 9;
          strncpy(name, argv[0] + 6, i);
          name[i] = '\0';
          if (*p) {
            int br = atoi(p);
            for (i = 0; i <= S_115K2; ++i) {
              if (br == atoi(speed_str[i])) {
                set_term_speed(name, i);
                return 0;
              }
            }
            printf("Invalid argument\n");
            return 1;
          } else {
            list_term_speed(name);
          }
        } else {
          printf("Syntax error\n");
          return 1;
        }
      } else if (strncmp(argv[0], "LOGON", 4) == 0) {
        byte b;
        address a;
        a = get_sym("$MFLGS");
        b = sys_getb(0, a);
        sys_putb(0, a, b | 0x01);
      } else if (strncmp(argv[0], "NOLOGON", 6) == 0) {
        byte b;
        address a;
        a = get_sym("$MFLGS");
        b = sys_getb(0, a);
        sys_putb(0, a, b & ~0x01);
      } else if (strncmp(argv[0], "POOL", 4) == 0) {
        if (argv[0][4] == '=') {
        } else {
          pool_stats("POOL=");
        }
      } else if (strncmp(argv[0], "HOST", 4) == 0) {
        address a;
        char name[10];
        a = get_sym("$HOSTN");
        if (argv[0][4] == '=') {
          p = argv[0] + 5;
          for (i = 0; i < 9; ++i) {
            if (*p) name[i] = *p++; else name[i] = ' ';
          }
          for (i = 0; i < 9; ++i) sys_putb(0, a + i, name[i]);
        } else {
          for (i = 0; i < 9; ++i) name[i] = sys_getb(0, a + i);
          name[9] = '\0';
          printf("HOST=%s\n", name);
        }
      } else if (strncmp(argv[0], "RNDC", 4) == 0) {
        address a;
        byte b;
        a = get_sym("$RNDC");
        if (argv[0][4] == '=') {
          b = atoi(argv[0] + 5);
          if ((b >= 1) && (b <= 255)) {
            sys_putb(0, a, b);
          } else {
            printf("Argument out of range\n");
          }
        } else {
          b = sys_getb(0, a);
          printf("RNDC=%d\n", b);
        }
      } else if (strncmp(argv[0], "RNDH", 4) == 0) {
        address a, al;
        byte b, bl;
        a  = get_sym("$RNDH");
        al = get_sym("$RNDL");
        bl = sys_getb(0, al);
        if (argv[0][4] == '=') {
          b = atoi(argv[0] + 5);
          if ((b >= bl) && (b <= 250)) {
            sys_putb(0, a, b);
          } else {
            printf("Argument out of range\n");
          }
        } else {
          b = sys_getb(0, a);
          printf("RNDH=%d\n", b);
        }
      } else if (strncmp(argv[0], "RNDL", 4) == 0) {
        address a, ah;
        byte b, bh;
        a  = get_sym("$RNDL");
        ah = get_sym("$RNDH");
        bh = sys_getb(0, ah);
        if (argv[0][4] == '=') {
          b = atoi(argv[0] + 5);
          if ((b >= 0) && (b < bh)) {
            sys_putb(0, a, b);
          } else {
            printf("Argument out of range\n");
          }
        } else {
          b = sys_getb(0, a);
          printf("RNDL=%d\n", b);
        }
      } else if (strncmp(argv[0], "SWPC", 4) == 0) {
        address a;
        byte b;
        a  = get_sym("$SWPC");
        if (argv[0][4] == '=') {
          b = atoi(argv[0] + 5);
          if ((b >= 0) && (b <= 255)) {
            sys_putb(0, a, b);
          } else {
            printf("Argument out of range\n");
          }
        } else {
          b = sys_getb(0, a);
          printf("SWPC=%d\n", b);
        }
      } else if (strncmp(argv[0], "SWPR", 4) == 0) {
        address a;
        byte b;
        a  = get_sym("$SWPRI");
        if (argv[0][4] == '=') {
          b = atoi(argv[0] + 5);
          if ((b >= 0) && (b <= 127)) {
            sys_putb(0, a, b);
          } else {
            printf("Argument out of range\n");
          }
        } else {
          b = sys_getb(0, a);
          printf("SWPR=%d\n", b);
        }
      } else {
        fprintf(stderr, "Unknown SET option\n");
        return 1;
      }
    } else {
      fprintf(stderr, "Invalid SET command\n");
      return 1;
    }
  } else if (strcmp(cmd, "PAR") == 0) {
    list_partitions();
  } else if (strcmp(cmd, "DEV") == 0) {
    strupr(args);
    list_devices(args);
  } else {
    fprintf(stderr, "Unknown command: %s\n", cmd);
    return 1;
  }
  return 0;
}

int vmr(char *cmdstr) {
  int  n;
  char *p, imgnam[256], symnam[256], cmd[256], args[256];

  imgnam[0] = cmd[0] = args[0] = '\0';
  n = 0;
  sscanf(cmdstr, "%s %s %n", imgnam, cmd, &n);
  if (n > 0) strcpy(args, cmdstr + n);
        
  if (!*imgnam) return 1;

  if (imgnam[0] == '@') {
    /* process command file */
    struct FCB *fcb;
    int  len;
    char line[256];

    p = strchr(imgnam, '.');
    if (!p) strcat(imgnam, ".cmd");
    strupr(imgnam);
    
    fcb = open_file(imgnam + 1);
    if (!fcb) {
      fprintf(stderr, "File not found\n");
      return 1;
    }
    /* first line contains the name of the system image file */
    len = get_line(fcb, imgnam, 256);
    if (len == 0) {
      fprintf(stderr, "Command file empty\n");
      close_file(fcb);
      free_fcb(fcb);
      return 1;
    }

    strcpy(symnam, imgnam);
    p = strchr(symnam, '.');
    if (p) *p = '\0';
    strcat(symnam, ".sym");

    p = strchr(imgnam, '.');
    if (!p) strcat(imgnam, ".sys");
    strupr(imgnam);
    strupr(symnam);

    if (open_system_image(imgnam, symnam)) {
      fprintf(stderr, "File not found\n");
      close_file(fcb);
      free_fcb(fcb);
      return 1;
    }
   
    for (;;) {
      /* get line */
      len = get_line(fcb, line, 256);
      if (len == 0) break;
      p = strchr(line, '!');
      if (p) *p = '\0';
      len = strlen(line);
      while ((len > 0) && isblank(line[len-1])) line[--len] = '\0';
      cmd[0] = args[0] = '\0';
      n = 0;
      sscanf(line, "%s %n", cmd, &n);
      if (n > 0) strcpy(args, line + n);
      if (*cmd && (*cmd != ';')) vmr_command(cmd, args);
    }
    save_system_image(imgnam);
    close_file(fcb);
    free_fcb(fcb);
    
  } else {
    /* process single command */
    if (!*cmd) return 1;

    strcpy(symnam, imgnam);
    p = strchr(symnam, '.');
    if (p) *p = '\0';
    strcat(symnam, ".sym");

    p = strchr(imgnam, '.');
    if (!p) strcat(imgnam, ".sys");
    strupr(imgnam);
    strupr(symnam);
    open_system_image(imgnam, symnam);
    
    strupr(cmd);
    cmd[3] = '\0';
    
    vmr_command(cmd, args);

    save_system_image(imgnam);
  }

  return 0;
}
