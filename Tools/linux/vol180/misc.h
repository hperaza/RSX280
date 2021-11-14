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

#ifndef __MISC_H
#define __MISC_H

#define FVER_H   5  /* filesystem version */
#define FVER_L   0

#define GET_INT16(p, i) (p[i] | (p[(i)+1] << 8))
#define SET_INT16(p, i, v) p[i] = (v) & 0xFF; p[(i)+1] = ((v) >> 8) & 0xFF;

#define GET_INT24(p, i) (p[i] | (p[(i)+1] << 8) | (p[(i)+2] << 16))
#define SET_INT24(p, i, v) p[i] = (v) & 0xFF; p[(i)+1] = ((v) >> 8) & 0xFF; p[(i)+2] = ((v) >> 16) & 0xFF;

#ifndef __MINGW32__
void strupr(char *s);
#endif
char *timestamp_str(unsigned char *entry);
long timestamp_to_secs(unsigned char *entry);
char *perm_str(unsigned short perm);

#endif
