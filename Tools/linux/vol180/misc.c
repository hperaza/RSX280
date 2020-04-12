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
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "misc.h"

/*-----------------------------------------------------------------------*/

void strupr(char *s) {
  while (*s) *s = toupper(*s), ++s;
}

char *timestamp_str(unsigned char *e) {
  static char str[40];
  unsigned yy, mm, dd, h, m, s;
  static char *month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
  
  yy = ((e[0] >> 4) & 0x0F) * 1000 + (e[0] & 0x0F) * 100 +
       ((e[1] >> 4) & 0x0F) * 10 + (e[1] & 0x0F);
  mm = ((e[2] >> 4) & 0x0F) * 10 + (e[2] & 0x0F);
  dd = ((e[3] >> 4) & 0x0F) * 10 + (e[3] & 0x0F);
  h  = ((e[4] >> 4) & 0x0F) * 10 + (e[4] & 0x0F);
  m  = ((e[5] >> 4) & 0x0F) * 10 + (e[5] & 0x0F);
  s  = ((e[6] >> 4) & 0x0F) * 10 + (e[6] & 0x0F);
  if ((mm >= 1) && (mm <= 12)) {
    sprintf(str, "%2d-%s-%04d  %02d:%02d:%02d", dd, month[mm-1], yy, h, m, s);
  } else {
    strcpy(str, "");
  }
  
  return str;
}

long timestamp_to_secs(unsigned char *e) {
  struct tm tms;
  time_t t;

  t = time(NULL);
  tms = *localtime(&t);
  tms.tm_year = ((e[0] >> 4) & 0x0F) * 1000 + (e[0] & 0x0F) * 100 +
                ((e[1] >> 4) & 0x0F) * 10 + (e[1] & 0x0F) - 1900;
  tms.tm_mon  = ((e[2] >> 4) & 0x0F) * 10 + (e[2] & 0x0F) - 1;
  tms.tm_mday = ((e[3] >> 4) & 0x0F) * 10 + (e[3] & 0x0F);
  tms.tm_hour = ((e[4] >> 4) & 0x0F) * 10 + (e[4] & 0x0F);
  tms.tm_min  = ((e[5] >> 4) & 0x0F) * 10 + (e[5] & 0x0F);
  tms.tm_sec  = ((e[6] >> 4) & 0x0F) * 10 + (e[6] & 0x0F);
  if ((tms.tm_mon >= 0) && (tms.tm_mon < 12)) {
    return (long) mktime(&tms);
  } else {
    return 0;
  }
}

char *perm_str(unsigned short perm) {
  static char temp[20];
  
  temp[0] = '\0';
  if (perm & 0x8000) strcat(temp, "R");
  if (perm & 0x4000) strcat(temp, "W");
  if (perm & 0x2000) strcat(temp, "E");
  if (perm & 0x1000) strcat(temp, "D");
  strcat(temp, ",");
  if (perm & 0x0800) strcat(temp, "R");
  if (perm & 0x0400) strcat(temp, "W");
  if (perm & 0x0200) strcat(temp, "E");
  if (perm & 0x0100) strcat(temp, "D");
  strcat(temp, ",");
  if (perm & 0x0080) strcat(temp, "R");
  if (perm & 0x0040) strcat(temp, "W");
  if (perm & 0x0020) strcat(temp, "E");
  if (perm & 0x0010) strcat(temp, "D");
  strcat(temp, ",");
  if (perm & 0x0008) strcat(temp, "R");
  if (perm & 0x0004) strcat(temp, "W");
  if (perm & 0x0002) strcat(temp, "E");
  if (perm & 0x0001) strcat(temp, "D");

  return temp;
}
