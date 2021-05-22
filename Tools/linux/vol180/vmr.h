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

#ifndef __VMR_H
#define __VMR_H

#include "fileio.h"

typedef unsigned short address;
typedef unsigned char  byte;

int load_symbols(struct FCB *f);
address get_sym(char *name);

int load_system(struct FCB *f);
int save_system(struct FCB *f);
byte sys_getb(byte bank, address addr);
address sys_getw(byte bank, address addr);
void sys_putw(byte bank, address addr, address val);
void sys_putb(byte bank, address addr, byte val);

void pool_init(void);
address pool_alloc(address size);
void pool_free(address addr, address size);
address pool_avail(void);
void pool_stats(char *msg);

void load_devices(void);
void assign(char *pdev, char *ldev, byte type, char *ttdev);
void deassign(char *ldev, byte type, char *ttdev);
void list_devices(char *name);
address find_device(char *name);
void set_term_opt(char *name, int bitno, int pol);
void list_term_opt(char *msg, int bitno, int pol);
void set_term_speed(char *name, int speed);
void list_term_speed(char *name);
void list_devices_opt(char *msg, int bitno, int pol);

address find_task(char *name);
void install_task(char *name, int argc, char *argv[]);
void remove_task(char *name);
void fix_task(char *name);
void unfix_task(char *name);
void list_tasks(char *name);

address find_partition(char *name);
void add_partition(char *name, address base, address size, byte wcmask, byte type);
void remove_partition(char *name);
address alloc_sub_partition(address mainpcb, byte size);
void list_partitions(void);

int open_system_image(char *imgfile, char *symfile);
int save_system_image(char *imgfile);
int get_line(struct FCB *fcb, char *str, int maxlen);
int vmr(char *cmd);

void pool_trace(void);
void test(void);

#endif  // __VMR_H
