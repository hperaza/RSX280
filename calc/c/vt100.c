#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "vt100.h"

/*#undef USE_STDIO*/
#define USE_STDIO

#ifdef USE_STDIO
#define PUTSTR(str) printf(str); fflush(stdout);
#else
#include "ttyio.h"
#define PUTSTR(str) put_string(str);
#endif

void clear_scr() {
  PUTSTR("\033[2J");
}

void clear_eos() {
  PUTSTR("\033[J");
}

void clear_eol() {
  PUTSTR("\033[K");
}

void cursor_off() {
  PUTSTR("\033[?25l");
}

void cursor_on() {
  PUTSTR("\033[?25h");
}

void cursor_home() {
  PUTSTR("\033[0;0H");
}

void graph_mode() {
  PUTSTR("\033(0");
}

void alpha_mode() {
  PUTSTR("\033(B");
}

void bold_mode() {
  PUTSTR("\033[1m");
}

void underline_mode() {
  PUTSTR("\033[4m");
}

void reverse_mode() {
  PUTSTR("\033[7m");
}

void set_color(int fg, int bg) {
  char tmp[32];
  
  if (fg < 0) {
    /* only bg */
    sprintf(tmp, "\033[%dm", bg+40);
    PUTSTR(tmp);
  } else if (bg < 0) {
    /* only fg */
    sprintf(tmp, "\033[%dm", fg+30);
    PUTSTR(tmp);
  } else {
    /* both */
    sprintf(tmp, "\033[%d;%dm", fg+30, bg+40);
    PUTSTR(tmp);
  }
}

void attrib_off() {
  PUTSTR("\033[0m");
}

void goto_xy(int x, int y) {
  char tmp[32];
  
  sprintf(tmp, "\033[%d;%dH", y+1, x+1);
  PUTSTR(tmp);
}

#ifdef CPM
#include <stdarg.h>

extern int _doprnt();

int vsnprintf(char *wh, int n, char *fmt, va_list args) {
  static FILE spf;

  spf._file = -1;
  spf._flag = _IOWRT | _IOBINARY | _IOSTRG;
  spf._base = wh;
  spf._ptr  = wh;
  spf._cnt  = n-1;

  _doprnt(&spf, fmt, args);
  *spf._ptr = 0;

  return spf._ptr - wh;
}
#endif

int printf_at(int x, int y, char *str, ...) {
  va_list args;
  int  errc;
  char tmp[128];

  va_start(args, str);
  sprintf(tmp, "\033[%d;%dH", y+1, x+1);
  PUTSTR(tmp);
  errc = vsnprintf(tmp, 128, str, args);
  va_end(args);
  PUTSTR(tmp);
  return errc;
} 
