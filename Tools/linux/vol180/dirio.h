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

#ifndef __DIRIO_H
#define __DIRIO_H

void set_dir_entry(unsigned char *entry, unsigned short inode,
                   char *fname, char *ext, short vers);
int  match(unsigned char *dirent, char *fname, char *ext, short vers);
int  match_fcb(unsigned char *dirent, struct FCB *fcb);
int  create_dir(char *filename, char group, char user);
int  change_dir(char *filename);

#endif
