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

#ifndef __INDEXF_H
#define __INDEXF_H

void set_inode(unsigned char *entry, unsigned short lnkcnt,
               char attrib, char group, char user,
               unsigned long nalloc, unsigned long nused,
               unsigned short lbcount, unsigned short perm);
int read_inode(unsigned short num, unsigned char *entry);
int write_inode(unsigned short num, unsigned char *entry);
int new_inode(void);
void set_date(unsigned char *buf, time_t t);
void set_cdate(unsigned char *entry, time_t t);
void set_mdate(unsigned char *entry, time_t t);
void set_name(unsigned char *entry, char *fname, char *ext, unsigned short vers);
char *get_name(unsigned char *entry);
void dump_inode(unsigned short num);

#endif
