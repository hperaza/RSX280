/* Free Calc - A spreadsheet program written and donated to public domain
by Martin Burger, all rights reserved, not for sale. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "ttyio.h"
#include "vt100.h"
#include "bcdconv.h"

/*typedef unsigned char byte;*/

struct cell {    /* cell structure on memory */
  struct cell *next;
  byte row,col;  /* cell coordinates */
  char flag;     /* 'A' = alpha, 'F' = formula */
  byte ready;    /* re-calc status, on file = cell width */
  double value;  /* computed value */
  byte align,    /* 0 = align left, 1 = align right, 2 = center */
       prec,     /* decimal digits (precision) */
       alen;     /* form[] length */
  char form[0];  /* [1..70] */
};

struct wcell {
  struct cell *next;
  byte row,col;
  char flag;
  byte ready;
  double value;
  byte align, prec, alen;
  char form[70];
};

#ifdef CPM
typedef unsigned long uint32_t;
#else
#include <stdint.h>
#endif

#ifdef CPM
struct fcell {   /* for file I/O */
#else
struct __attribute__((packed)) fcell {   /* for file I/O */
#endif
  byte row,col;
  char flag;
  byte ready;
  byte bcd_value[6];
  byte align, prec, alen;
};

#ifndef bool
#define bool byte
#define false 0
#define true  1 /*(!false)*/
#endif

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279
#endif

enum mode { wait, cursor, command, formulas, parameter, execute_command,
            execute_formula, execute_parameter };

enum arrow { up, down, right, left, none };

struct cell *chead, *ctail;
struct wcell work_cell;
byte width[256];
char heading[41];
char *df_heading = "Free Calc V1.1";
byte home_r, home_c;
byte cur_r, cur_c;
byte screen;
byte precision;
short justify;
bool local_edit;
byte test_r, test_c;
short del_row, del_col; /* byte */
bool error, calc_error;
char buffer[70];
short ip;
short command_type;
enum mode state;
bool done;
bool global;
enum arrow direction;

bool color = false;

double formula(struct cell *p, short *ip);
double expression(struct cell *p, short *ip);

void gotoxy(short r, short c) {
  goto_xy(c, r);
}

void setrev(byte c) {
  if (color) {
    switch (c) {
      case 1:  /* cell header */
        set_color(7, 1);
        break;
        
      default:
      case 2:  /* selected cell */
        set_color(0, 7);
        break;
        
    }
  } else {
    reverse_mode();
  }
}

void resrev(byte c) {
  if (color) {
    switch (c) {
      case 0:  /* normal background */
        set_color(7, 4);
        break;
        
      case 1:  /* header */
        set_color(0, 2);
        break;
        
      default:
      case 2:  /* raw cell contents */
        set_color(6, 4);
        break;
        
    }
  } else {
    attrib_off();
  }
}

void clr_rest() {
  clear_eol();
}

void bell() {
  put_char(7);
}

void bwrite(short w) {
  while (w-- > 0) put_char(' ');
}
    
void clr_buf() {
  int i;

  for (i = 0; i < 70; ++i) buffer[i] = '\0';
}

void upper() {
  int i;

  for (i = 0; i < 70; ++i) buffer[i] = toupper(buffer[i]);
}

void set_heading(char *str, int len) {
  int i, j;

  if (len > 0) {
    for (i = 0; i < 40; ++i) heading[i] = ' ';
    if (len > 40) len = 40;
    j = (40 - len) / 2;
    if (j < 0) j = 0;
    for (i = 0; i < len; ++i) heading[i+j] = str[i];
    heading[40] = '\0';
  }
}

void reset_calc() { /* reset variables (note: does not delete old spreadsheet!) */
  int i;

  for (i = 0; i < 256; ++i) width[i] = 9;
  chead = NULL;
  ctail = NULL;
  home_r = 0;
  home_c = 0;
  cur_r = 0;
  cur_c = 0;
  test_r = 0;
  test_c = 0;
  del_row = 0;
  del_col = 0;
  precision = 2;
  justify = 0;
  local_edit = false;
  set_heading(df_heading, strlen(df_heading));
}

bool on_screen(byte r, byte c) {
  short i, col;

  if ((r < home_r) || (c < home_c) || (r >= home_r+20)) return false;
  col = 4;
  for (i = home_c; i <= c; ++i) col += width[i];
  return (col < screen);
}

void dis_alfa(struct cell *p, byte i) {
  short w1, w2;

  w1 = p->alen - 1; /* exclude trailing null */
  if (w1 > width[i]) w1 = width[i];
  if (w1 < 0) w1 = 0;
  w2 = width[i]-w1;
  if (p->align == 1) {
    /* justify right */
    if (w2 > 0) bwrite(w2);
    if (w1 > 0) printf("%*s", w1, p->form);
    fflush(stdout);
  } else if (p->align == 2) {
    /* center */  
    w2 /= 2;
    if (w2 > 0) bwrite(w2);
    if (w1 > 0) printf("%*s", w1, p->form);
    fflush(stdout);
    w2 = width[i]-w1-w2;
    if (w2 > 0) bwrite(w2);
  } else {
    /* justify left */
    if (w1 > 0) printf("%*s", w1, p->form);
    fflush(stdout);
    if (w2 > 0) bwrite(w2);
  }
}

void whead(byte r, byte c) {
  short i, n, col, v1, v2;

  bwrite(4);
  col = 4;
  for (i = c; (i <= 255) && (col+width[i] < screen); ++i) {
    if (width[i] > 0) {
      v2 = i/26;
      if (v2 == 0) n = 1; else n = 2;
      v1 = (width[i]-n)/2;
      if (v1 > 0) {
        put_char('-');
        bwrite(v1-1);
      }
      if ((n > 1) && (width[i] > 1)) put_char('A'+v2-1);
      v2 = i%26;
      put_char('A'+v2);
      v1 = width[i]-n-v1;
      if (v1 > 0) {
        bwrite(v1-1);
        put_char('-');
      }
    }
    col += width[i];
  }
  clr_rest();
}

void dis_form(struct cell *p, byte i) {
  if (p) if (p->alen > 0) printf("%*s", p->alen, p->form);
  fflush(stdout);
}

void dis_val(struct cell *p, byte i);

void cal_form(struct cell *p, byte i) {  /* local to dis_val() */
  short j;

  error = false;
  calc_error = false;
  p->ready = 2;
  j = 0;
  p->value = formula(p, &j);
  if (!calc_error) p->ready = 1;
  if (error) p->ready = 3;
  dis_val(p, i);
}

void dis_val(struct cell *p, byte i) {
  int  j;
  char *fmt;

  if (!p) {
    bwrite(width[i]);
  } else {
    if (width[i] > 0) {
      if (p->flag == 'A') {
        dis_alfa(p, i);
      } else {
        if (p->ready == 3) {
          printf("%*s", width[i], "ERROR");
        } else if (p->ready == 2) {
          printf("%*s", width[i], "<VAL/ERR>");
        } else if (p->ready != 0) {
          j = (p->prec >> 5) & 0x3;
          if (j == 1) {
            fmt = "%*.*f";
          } else if (j == 2) {
            fmt = "%*.*E";
          } else {
#if 0
            fmt = "%*.*g";
#else
            fmt = ((fabs(p->value) < .01)) || ((fabs(p->value) > 1e10)) ? "%*.*E" : "%*.*f";
#endif
          }
          printf(fmt, width[i], p->prec & 0x1f, p->value);
        } else {
          cal_form(p, i);  /* recursive call! */
        }
      }
    }
    fflush(stdout);
  }
}

void clr_ready() {
  struct cell *head;

  head = chead;
  while (head) {
    head->ready = 0;
    head = head->next;
  }
}

struct cell *get_point(byte r, byte c) {
  struct cell *head;

  head = chead;
  while (head) {
    if ((head->row == r) && (head->col == c)) return head;
    head = head->next;
  }
  return NULL;
}

void dis_row(byte r, byte c) {
  struct cell *p;
  short i, col;

  setrev(1);
  printf("%3d-", r+1);  /* output row header */
  fflush(stdout);
  resrev(0);
  col = 4;
  for (i = c; (i <= 255) && (col+width[i] < screen); ++i) {
    p = get_point(r, i);
    dis_val(p, i);
    col += width[i];
  }
  clr_rest();
}

void posit(byte r, byte c) {
  short i, col;

  col = 4;
  if (c > home_c) for (i = home_c; i < c; ++i) col += width[i];
  gotoxy(3+r-home_r, col);
}

void dis_rc(byte r, byte c) {
  short v1;

  v1 = c / 26;
  if (v1 != 0) put_char('A'+v1-1);
  v1 = c % 26;
  put_char('A'+v1);
  printf("%d", r+1);
  fflush(stdout);
}

unsigned short mem_used() {
  unsigned short n;
  struct cell *head;
  
  /* estimate memory used */
  n = 0;
  head = chead;
  while (head) {
    n += sizeof(struct cell) + head->alen;
    head = head->next;
  }  
  return n;
}  

void prompt(byte r, byte c) {
  struct cell *p;
  unsigned short m;

  p = get_point(r, c);
  posit(r, c);
  setrev(2);
  dis_val(p, c);  /* highlight current cell */
  resrev(2);
  gotoxy(1, 0);
  dis_rc(r, c);
  clr_rest();
  put_char(':');
  gotoxy(1, 6);
  dis_form(p, c); /* display raw cell contents on top row */
  if (color) resrev(0);
  gotoxy(0, screen-17);
  m = mem_used();
  if (color) resrev(1);
  printf("%5d bytes used", m);
  if (color) resrev(0);
  gotoxy(23, 0);
  printf("Command: ");
  clr_rest();
}

void display(byte r, byte c) {
  short i, rs, re;

  rs = r;
  re = rs+19;
  if (re > 255) {
    re = 255;
    if (color) resrev(0);
    clear_scr();
  }
  gotoxy(0, 0);
  if (color) resrev(1);
  clr_rest();
  i = (screen-40)/2;
  gotoxy(0, i);
  printf("%40s", heading);
  gotoxy(2, 0);
  setrev(1);
  whead(r, c);  /* display column headers */
  resrev(0);
  for (i = rs; i <= re; ++i) {
    gotoxy(3+i-rs, 0);
    dis_row(i, c);
  }
  prompt(cur_r, cur_c);
}

void rem_blanks(struct cell *p, short *ip) {
  while (p->form[*ip] == ' ') ++(*ip);
  if (*ip > p->alen) error = true;
}

void eat_char(struct cell *p, short *ip, char c) {
  rem_blanks(p, ip);
  if (p->form[*ip] == c) ++(*ip); else error = true;
}

void eat_then(struct cell *p, short *ip) {
  rem_blanks(p, ip);
  if (strncmp(&p->form[*ip], "@THEN", 5) == 0) *ip += 5; else error = true;
}

void eat_else(struct cell *p, short *ip) {
  rem_blanks(p, ip);
  if (strncmp(&p->form[*ip], "@ELSE", 5) == 0) *ip += 5; else error = true;
}

struct cell *get_new(byte i) {
  struct cell *p;
  
  p = calloc(i+sizeof(struct cell), 1);
  return p;
}

void do_dispose(struct cell *p) {
  struct cell *head, *last;

  head = chead;
  last = NULL;
  while (head && (head != p)) {
    last = head;
    head = head->next;
  }
  if (last) last->next = p->next; else chead = p->next;
  if (!p->next) ctail = last;
  free(p);
}

void dispose_all() { /* delete spreadsheet */
  struct cell *head, *curr;

  head = chead;
  while (head) {  /* delete old spreadsheet */
    curr = head->next;
    do_dispose(head);
    head = curr;
  }
  chead = NULL;
  ctail = NULL;
}

double atom(short rrow, short ccol) {
  struct cell *p;
  double f, atom;
  short i;
  bool c_error, e_error;

  atom = 0;
  if (!local_edit) {
    i = 0;
    p = get_point(rrow, ccol);
    if (p) {
      if (p->flag == 'A') calc_error = true;
      else if (p->ready > 1) calc_error = true;
      else if (p->ready != 0) atom = p->value;
      else {
        c_error = calc_error;
        e_error = error;
        calc_error = false;
        error = false;
        f = formula(p, &i);
        if (calc_error) p->ready = 2; else p->ready = 1;
        if (error) p->ready = 3;
        calc_error = calc_error || c_error || error;
        error = e_error;
        if ((p->value != f) || calc_error) {
          p->value = f;
          if (on_screen(rrow, ccol)) {
            posit(rrow, ccol);
            dis_val(p, ccol);
          }
        }
        atom = p->value;
      }
    } else {
      calc_error = true;
    }
  }
  return atom;
}

void get_xy(struct cell *p, short *ip, short *rrow, short *ccol) {
  short is, absolute_r, absolute_c;

  *rrow = 0;
  *ccol = 0;
  is = *ip;
  absolute_r = 0;
  absolute_c = 0;
  if (p->form[*ip] == '$') {
    absolute_c = 1;
    ++(*ip);
  }
  while ((p->form[*ip] >= 'A') && (p->form[*ip] <= 'Z')) {
    *ccol = *ccol*26 + p->form[*ip]-'A'+1;
    ++(*ip);
  }
  --(*ccol);
  if (p->form[*ip] == '$') {
    absolute_r = 1;
    ++(*ip);
  }
  while ((p->form[*ip] >= '0') && (p->form[*ip] <= '9')) {
    *rrow = *rrow*10 + p->form[*ip]-'0';
    ++(*ip);
  }
  --(*rrow);

  if (local_edit) {
    /* modify */
    short i, k, l, r, c;

    /* test_r and test_c are the row or column being inserted/deleted
       (only one is set) */
    if ((*rrow >= test_r) && (*ccol >= test_c)) {
      /* cell is to the right or below the inserted/delete column or row */
      r = *rrow + del_row;
      if (absolute_r) r = *rrow;
      c = *ccol + del_col;
      if (absolute_c) c = *ccol;
      /* compute new cell name length */
      if (r < 9) l = 1; else if (r < 99) l = 2; else l = 3;
      if (c < 26) ++l; else l += 2;
      l += absolute_r + absolute_c;
      /* now, replace (note: working on work_cell when local_edit is set,
         so we have a 70 char buffer to work with */
      if ((p->alen+l-(*ip)+is < 70) && (*ip <= p->alen) && (*ip > is) &&
          (r >= 0) && (r <= 255) && (c >= 0) && (c <= 255)) {
        for (i = *ip; i <= p->alen; ++i)
          p->form[i-(*ip)+is] = p->form[i];
        for (i = is; i < *ip; ++i)
          p->form[p->alen+is-i] = '\0';
        for (i = p->alen-1; i >= *ip; --i)
          p->form[i-(*ip)+is+l] = p->form[i-(*ip)+is];
        i = is;
        if (absolute_c) p->form[i++] = '$';
        k = c/26;
        if (k > 0) p->form[i++] = k-1+'A';
        k = c%26;
        p->form[i++] = k+'A';
        if (absolute_r) p->form[i++] = '$';
        ++r;
        k = r/100;
        if (k > 0) p->form[i++] = k+'0';
        k = r%100;
        k = k/10;
        if (r > 9) p->form[i++] = k+'0';
        k = r%10;
        p->form[i] = k+'0';
        p->alen += l-(*ip)+is;
        *ip = is+l;
      }
    }
  }
}

double elementary(struct cell *p, short *ip) {
  short rrow, ccol;

  rem_blanks(p, ip);
  get_xy(p, ip, &rrow, &ccol);
  if ((rrow < 0) || (rrow > 255) || (ccol < 0) || (ccol > 255)) {
    error = true;
    return 0;
  } else {
    return atom(rrow, ccol);
  }
}

bool get_range(struct cell *p, short *ip,
               short *r1, short *c1, short *r2, short *c2) {
  get_xy(p, ip, r1, c1);
  eat_char(p, ip, ':');
  get_xy(p, ip, r2, c2);
  if ((*r1 < 0) || (*c1 < 0) || (*r2 < 0) || (*c2 < 0) ||
      (*r1 > 255) || (*c1 > 255) || (*r2 > 255) || (*c2 > 255))
    return false;
  else
    return true;
}

double array_function(struct cell *p, short *ip,
                      short tok_type) {  /* local to get_function() */
  short i, j, r1, c1, r2, c2, items;
  double t, f;
  bool ok, flg;

  rem_blanks(p, ip);
  f = 0;
  ok = true;
  flg = true; /* set means first item */
  items = 0;  /* item count for AVG function */
  while (ok) {
    rem_blanks(p, ip);
    i = *ip;
    while (((p->form[i] >= 'A') && (p->form[i] <= 'Z')) ||
           ((p->form[i] >= '0') && (p->form[i] <= '9')) ||
            (p->form[i] == '$') || (p->form[i] == ' ')) ++i;
    if (p->form[i] == ':') {
      /* range */
      if (get_range(p, ip, &r1, &c1, &r2, &c2)) {
        for (i = r1; i <= r2; ++i) {
          for (j = c1; j <= c2; ++j) {
            t = atom(i, j);
            if (flg) f = t;
            ++items;
            switch (tok_type) {
              case 2:  /* SUM */
              case 5:  /* AVG */
                if (!flg) f += t;
                break;
                
              case 3:  /* MAX */
                if (t > f) f = t;
                break;
                
              case 4:  /* MIN */
                if (t < f) f = t;
                break;
            }
            flg = false;
          }
        }
      } else {
        error = true;
      }
    } else {
      /* single cell in a comma-separated list */
      t = formula(p, ip);
      if (flg) f = t;
      ++items;
      switch (tok_type) {
        case 2:  /* SUM */
        case 5:  /* AVG */
          if (!flg) f += t;
          break;
          
        case 3:  /* MAX */
          if (t > f) f = t;
          break;
          
        case 4:  /* MIN */
          if (t < f) f = t;
          break;
      }
      flg = false;
    }
    rem_blanks(p, ip);
    if (p->form[*ip] == ',') ++(*ip); else ok = false;
  }
  eat_char(p, ip, ')');
  if (tok_type == 5) if (items != 0) f /= items;  /* AVG */
  return f;
}

double frac(double f) {  /* local to get_function() */
  double f1, f2;

  if (f >= 0) f1 = f; else f1 = -f;
  while (f1 > 32767) {
    f2 = 32768.0;
    while (f2 < f1) f2 = f2*2;
    f2 = f2/2;
    f1 = f1-f2;
  }
  f1 = f1-trunc(f1);
  if (f >= 0) return f1; else return -f1;
}

double fact(double f) {
  double f1;
  
  if ((f == 0.0) || (f == 1.0)) return 1.0;

  if (f < 0.0) {
    calc_error = true;
    return 1.0;
  }

  if (trunc(f) != f) {
    calc_error = true;
    return 1.0;
  }
  
  f1 = 1.0;
  while (f > 0.0) {
    f1 *= f;
    f -= 1.0;
  }
  
  return f1;
}

double do_lookup(struct cell *p, short *ip) { /* local to get_function() */
  short i, r1, c1, r2, c2;
  bool flg;
  double f, f1;

  rem_blanks(p, ip);
  f = formula(p, ip);
  eat_char(p, ip, ',');
  rem_blanks(p, ip);
  if (get_range(p, ip, &r1, &c1, &r2, &c2)) {
    if ((c2 != (c1+1)) || (c1 > c2)) {
      error = true;
    } else {
      flg = false;
      f1 = 0;
      for (i = r1; i <= r2; ++i) {
        if (f >= atom(i, c1)) {
          f1 = atom(i, c2);
          flg = true;
        }
      }
      if (!flg) calc_error = true;
    }
  } else {
    error = true;
  }
  eat_char(p, ip, ')');
  return f1;
}

double get_function(struct cell *p, short *ip) {
  short i, tok_type;
  double f;
  char token[9];

  ++(*ip);
  i = 0;
  while ((i < 8) &&
         (((p->form[*ip] >= 'A') && (p->form[*ip] <= 'Z')) ||
          ((p->form[*ip] >= '0') && (p->form[*ip] <= '9')))) {
    token[i++] = p->form[*ip];
    ++(*ip);
  }
  token[i] = '\0';
  tok_type = 0;
       if (strcmp(token, "PI") == 0)     tok_type = 1;
  else if (strcmp(token, "SUM") == 0)    tok_type = 2;
  else if (strcmp(token, "MAX") == 0)    tok_type = 3;
  else if (strcmp(token, "MIN") == 0)    tok_type = 4;
  else if (strcmp(token, "AVG") == 0)    tok_type = 5;
  else if (strcmp(token, "LOOKUP") == 0) tok_type = 6;
  else if (strcmp(token, "ABS") == 0)    tok_type = 7;
  else if (strcmp(token, "INT") == 0)    tok_type = 8;
  else if (strcmp(token, "SIGN") == 0)   tok_type = 9;
  else if (strcmp(token, "ROUND") == 0)  tok_type = 10;
  else if (strcmp(token, "SQRT") == 0)   tok_type = 11;
  else if (strcmp(token, "EXP") == 0)    tok_type = 12;
  else if (strcmp(token, "EXP10") == 0)  tok_type = 13;
  else if (strcmp(token, "LOG") == 0)    tok_type = 14;
  else if (strcmp(token, "LOG10") == 0)  tok_type = 15;
  else if (strcmp(token, "SIN") == 0)    tok_type = 16;
  else if (strcmp(token, "COS") == 0)    tok_type = 17;
  else if (strcmp(token, "TAN") == 0)    tok_type = 18;
  else if (strcmp(token, "ATN") == 0)    tok_type = 19;
  else if (strcmp(token, "FACT") == 0)   tok_type = 20;
  else error = true;
  if (tok_type == 1) { /* PI */
    return M_PI;
  }
  eat_char(p, ip, '(');
  switch (tok_type) {
    case 2: /* SUM */
    case 5: /* AVG */
      f = array_function(p, ip, tok_type);
      break;
      
    case 3: /* MAX */
      f = array_function(p, ip, tok_type);
      break;
      

    case 4: /* MIN */
      f = array_function(p, ip, tok_type);
      break;
      
    case 6: /* LOOKUP */
      f = do_lookup(p, ip);
      break;

    case 7: /* ABS */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      if (f < 0) f = -f;
      break;

    case 8: /* INT */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      if ((f > 32767) || (f < -32767))
        f = f-frac(f);
      else
        f = trunc(f);
      break;
      
    case 9: /* SIGN */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      f = (f == 0) ? 0.0 : ((f > 0) ? 1.0 : -1.0);
      break;
      
    case 10: /* ROUND */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      if ((f > 32767) || (f < -32767))
        f = f-frac(f)+round(frac(f));
      else
        f = round(f);
      break;
      
    case 11: /* SQRT */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      if (f >= 0)
        f = sqrt(f);
      else
        calc_error = true;
      break;
      
    case 12: /* EXP */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      f = exp(f);
      break;
      
    case 13: /* EXP10 */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      f = exp(f * log(10.0));
      break;
      
    case 14: /* LOG */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      if (f > 0)
        f = log(f);
      else
        calc_error = true;
      break;
      
    case 15: /* LOG10 */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      if (f > 0)
        f = log(f) / log(10.0);
      else
        calc_error = true;
      break;
      
    case 16: /* SIN */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      f = sin(f);
      break;
      
    case 17: /* COS */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      f = cos(f);
      break;
      
    case 18: /* TAN */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      f = tan(f);
      break;
      
    case 19: /* ATN */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      f = atan(f);
      break;
      
    case 20: /* FACT */
      f = formula(p, ip);
      eat_char(p, ip, ')');
      f = fact(f);
      break;
      
    default:
      f = 0;
      break;
  }
  return f;
}

double simple(struct cell *p, short *ip) {
  bool sign;
  double f;

  rem_blanks(p, ip);
  sign = false;
  if (p->form[*ip] == '+') ++(*ip);
  if (p->form[*ip] == '-') {
    ++(*ip);
    sign = true;
  }
  if (p->form[*ip] == '@')
    f = get_function(p, ip);
  else
    f = elementary(p,ip);

  if (sign) return -f; else return f;
}

bool is_constant(struct cell *p, short *ip) {
  short i;

  rem_blanks(p, ip);
  i = *ip;
  if ((p->form[i] == '+') || (p->form[i] == '-')) ++i;
  if ((p->form[i] == '.') ||
      ((p->form[i] >= '0') && (p->form[i] <= '9')))
    return true;
  else
    return false;
}

double get_constant(struct cell *p, short *ip) {
#if 0
  bool sign;
  double f, ff;

  rem_blanks(p, ip);
  sign = false;
  if (p->form[*ip] == '+') ++(*ip);
  if (p->form[*ip] == '-') {
    ++(*ip);
    sign = true;
  }
  f = 0;
  while ((p->form[*ip] >= '0') && (p->form[*ip] <= '9')) {
    f = f*10+p->form[*ip]-'0';
    ++(*ip);
  }
  if (p->form[*ip] == '.') {
    ff = 0.1;
    ++(*ip);
    while ((p->form[*ip] >= '0') && (p->form[*ip] <= '9')) {
      f = f+(p->form[*ip]-'0')*ff;
      ff *= 0.1;
      ++(*ip);
    }
  }
  if (sign) f = -f;
#else
  char *ptr, *endptr;
  double f;

  rem_blanks(p, ip);
  ptr = p->form + (*ip);
  f = strtod(ptr, &endptr);
  (*ip) += endptr - ptr;
#endif
  return f;
}

short get_log(struct cell *p, short *ip) {
  short getlog;

  rem_blanks(p, ip);
  getlog = 0;
  switch (p->form[*ip]) {
    case '=':
      ++(*ip);
      getlog = 1;
      break;
      
    case '<':
      ++(*ip);
      getlog = 4;
      switch (p->form[*ip]) {
        case '>':
          ++(*ip);
          getlog = 2;
          break;
          
        case '=':
          ++(*ip);
          getlog = 6;
          break;
      }
      break;

    case '>':
      ++(*ip);
      getlog = 3;
      if (p->form[*ip] == '=') {
        ++(*ip);
        getlog = 5;
      }
      break;
  }
  if (getlog == 0) error = true;
  return getlog;
}

bool compare(double f1, double f2, byte op) {
  switch (op) {
    case 1: return (f1 == f2);
    case 2: return (f1 != f2);
    case 3: return (f1  > f2);
    case 4: return (f1  < f2);
    case 5: return (f1 >= f2);
    case 6: return (f1 <= f2);
    default: return false;
  }
}

double variable(struct cell *p, short *ip) {
  double v;
  
  if (is_constant(p, ip)) {
    v = get_constant(p, ip);
  } else if (p->form[*ip] == '(') {
    ++(*ip);
    v = formula(p, ip);
    eat_char(p, ip, ')');
  } else {
    v = simple(p, ip);
  }
  return v;
}

double power(struct cell *p, short *ip) {
  double f1, f2;

  f1 = variable(p, ip);
  rem_blanks(p, ip);
  while (p->form[*ip] == '^') {
    ++(*ip);
    f2 = variable(p, ip);
    if (f1 > 0) f1 = exp(f2*log(f1)); else calc_error = true;
    rem_blanks(p, ip);
  }
  return f1;
}

double mul_div_pow(struct cell *p, short *ip) {
  double f1, f2;
  
  f1 = power(p, ip);
  while ((p->form[*ip] == '*') || (p->form[*ip] == '/')) {
    switch (p->form[*ip]) {
      case '*':
        ++(*ip);
        f1 *= power(p, ip);
        break;
        
      case '/':
        ++(*ip);
        f2 = power(p, ip);
        if (f2 != 0) f1 = f1/f2; else calc_error = true;
        break;
    }
  }
  return f1;
}

double expression(struct cell *p, short *ip) {
  double f1;
  
  f1 = mul_div_pow(p, ip);
  while ((p->form[*ip] == '+') || (p->form[*ip] == '-')) {
    switch (p->form[*ip]) {
      case '+':
        ++(*ip);
        f1 += mul_div_pow(p, ip);
        break;
        
      case '-':
        ++(*ip);
        f1 -= mul_div_pow(p, ip);
        break;
    }
  }
  return f1;
}

bool logical(struct cell *p, short *ip);

bool what(struct cell *p, short *ip) { /* local to logical() */
  bool llog;
  double f1, f2;
  short comp;

  rem_blanks(p, ip);
  if (p->form[*ip] == '[') {
    ++(*ip);
    llog = logical(p, ip);  /* recursive call! */
    eat_char(p, ip, ']');
  } else {
    f1 = formula(p, ip);
    comp = get_log(p, ip);
    f2 = formula(p, ip);
    llog = compare(f1, f2, comp);
  }
  return llog;
}

bool logical(struct cell *p, short *ip) {
  bool logic;

  logic = what(p, ip);
  rem_blanks(p, ip);
  while ((p->form[*ip] == '|') || (p->form[*ip] == '&')) {
    if (p->form[*ip] == '&') {
      ++(*ip);
      logic = logic && what(p, ip);
    } else {
      ++(*ip);
      logic = logic || what(p, ip);
    }
    rem_blanks(p, ip);
  }
  return logic;
}

double formula(struct cell *p, short *ip) {  /* recursive! */
  bool logic;
  double f1, f2;
  
  rem_blanks(p, ip);
  if (strncmp(&p->form[*ip], "@IF", 3) == 0) {
    *ip += 3;
    logic = logical(p, ip);
    eat_then(p, ip);
    f1 = formula(p, ip);
    eat_else(p, ip);
    f2 = formula(p, ip);
    if (logic) return f1; else return f2;
  } else {
    return expression(p, ip);
  }
}

void vertical(short i) {
  struct cell *p;

  p = get_point(cur_r, cur_c);
  posit(cur_r, cur_c);
  dis_val(p, cur_c);
  put_string("\033[4;23r\033[?6h");
  if (i < 0) put_string("\033M"); else put_string("\033[20;1H\033E");
  put_string("\033[?6l\033[r");
  cur_r += i;
  home_r += i;
  if (i > 0) direction = down; else direction = up;
  if (i < 0) gotoxy(3, 0); else gotoxy(22, 0);
  dis_row(cur_r, home_c);
  prompt(cur_r, cur_c);
}

void horizontal(short i) {
  cur_c += i;
  home_c += i;
  while ((cur_c > home_c) && !on_screen(cur_r, cur_c)) ++home_c;
  if (i > 0) direction = right; else direction = left;
  display(home_r, home_c);
}

void process_cursor() {
  char s_buf[2];
  struct cell *p;

  state = wait;
  s_buf[0] = get_char();
  s_buf[1] = get_char();
  if (s_buf[0] == '[') {
    switch (s_buf[1]) {
      case 'A':
        if (cur_r > 0) {
          if (on_screen(cur_r-1, cur_c)) {
            direction = up;
            p = get_point(cur_r, cur_c);
            posit(cur_r, cur_c);
            dis_val(p, cur_c);
            --cur_r;
            prompt(cur_r, cur_c);
          } else {
            vertical(-1);
          }
        } else {
          bell();
        }
        break;
        
      case 'B':
        if (cur_r < 255) {
          if (on_screen(cur_r+1, cur_c)) {
            direction = down;
            p = get_point(cur_r, cur_c);
            posit(cur_r, cur_c);
            dis_val(p, cur_c);
            ++cur_r;
            prompt(cur_r, cur_c);
          } else {
            vertical(1);
          }
        } else {
          bell();
        }
        break;
        
      case 'C':
        if (cur_c < 255) {
          if (on_screen(cur_r, cur_c+1)) {
            direction = right;
            p = get_point(cur_r, cur_c);
            posit(cur_r, cur_c);
            dis_val(p, cur_c);
            ++cur_c;
            prompt(cur_r, cur_c);
          } else {
            horizontal(1);
          }
        } else {
          bell();
        }
        break;
        
      case 'D':
        if (cur_c > 0) {
          if (on_screen(cur_r, cur_c-1)) {
            direction = left;
            p = get_point(cur_r, cur_c);
            posit(cur_r, cur_c);
            dis_val(p, cur_c);
            --cur_c;
            prompt(cur_r, cur_c);
          } else {
            horizontal(-1);
          }
        } else {
          bell();
        }
        break;
      
      default:
        bell();
        break;
    }
  } else {
    bell();
  }
}

void re_calc() {
  short i;
  struct cell *head;
  double f;

  head = chead;
  while (head) {
    if ((head->ready == 0) && (head->flag == 'F')) {
      if (on_screen(head->row, head->col)) {
        i = 0;
        error = false;
        calc_error = false;
        head->ready = 2;
        f = formula(head, &i);
        if (calc_error) head->ready = 2; else head->ready = 1;
        if (error) head->ready = 3;
        if ((head->value != f) || (head->ready != 1)) {
          head->value = f;
          posit(head->row, head->col);
          dis_val(head, head->col);
        }
      }
    }
    head = head->next;
  }
}

void move(struct cell *p_from, struct cell *p_to) {
  short i;
  
  p_to->row = p_from->row;
  p_to->col = p_from->col;
  p_to->flag = p_from->flag;
  p_to->ready = p_from->ready;
  p_to->prec = p_from->prec;
  p_to->align = p_from->align;
  p_to->value = p_from->value;
  p_to->alen = p_from->alen;
  for (i = 0; i < p_to->alen; ++i) p_to->form[i] = p_from->form[i];
}

void help() {
  short ch;
  bool ok;
  FILE *hf;
  char buf[80];
  
  hf = fopen("calc.hlp", "r");
  clear_scr();
  gotoxy(0, 0);
  ok = true;
  if (hf) {
    while (!feof(hf) && ok) {
      fgets(buf, 79, hf);
      if (buf[0] != '\014') {
        printf(buf);
      } else {
        gotoxy(23, 0);
        printf("Hit return to continue, anything else to exit ");
        fflush(stdout);
        ch = '\012';
        while (ch == '\012') ch = get_char();
        if (ch != '\015') ok = false;
        clear_scr();
        gotoxy(0, 0);
      }
    }
    fclose(hf);
  }
  display(home_r, home_c);
}

void out_head(FILE *f) {  /* local to out_page() */
  fprintf(f, "\014%45s\n", heading);
}

void out_top(FILE *f, short jstart, short jend) {  /* local to out_page() */
  short i, v1, v2;

  fprintf(f, "     ");
  for (i = jstart; i <= jend; ++i) {
    if (width[i] > 0) {
      v1 = (width[i]-2)/2;
      if (v1 > 0) fprintf(f, "%*s", v1, " ");
      v2 = i/26;
      if (v2 == 0)
        fprintf(f, " ");
      else
        fprintf(f, "%c", 'A'+v2-1);
      v2 = i%26;
      fprintf(f, "%c", 'A'+v2);
      v2 = width[i]-v1-2;
      if (v2 > 0) fprintf(f, "%*s", v2, " ");
    }
  }
  fprintf(f, "\n");
}

void print_alfa(FILE *f, struct cell *p, byte i) {  /* local to out_page() */
  short w1, w2;
  
  w1 = p->alen;
  if (w1 > width[i]) w1 = width[i];
  if (w1 < 0) w1 = 0;
  w2 = width[i]-w1;
  if (p->align == 1) {
    if (w2 > 0) fprintf(f, "%*s", w2, " ");
    if (w1 > 0) fprintf(f, "%*s", w1, p->form);
  } else if (p->align == 2) {
    w2 = w2/2;
    if (w2 > 0) fprintf(f, "%*s", w2, " ");
    if (w1 > 0) fprintf(f, "%*s", w1, p->form);
    w2 = width[i]-w1-w2;
    if (w2 > 0) fprintf(f, "%*s", w2, " ");
  } else {
    if (w1 > 0) fprintf(f, "%*s", w1, p->form);
    if (w2 > 0) fprintf(f, "%*s", w2, " ");
  }
}

void out_line(FILE *f, short jrow, short jstart, short jend) {  /* local to out_page() */
  short i, j;
  struct cell *p;
  char *fmt;

  fprintf(f, "%3d ", jrow+1);
  for (i = jstart; i <= jend; ++i) {
    if (width[i] > 0) {
      p = get_point(jrow, i);
      if (!p) {
        fprintf(f, "%*s", width[i], " ");
      } else {
        gotoxy(1, 0);
        clr_rest();
        dis_val(p, i);
        if (p->flag == 'A') {
          print_alfa(f, p, i);
        } else {
          if (p->ready > 2) {
            fprintf(f, "%*s", width[i], "ERROR");
          } else if (p->ready == 2) {
            fprintf(f, "%*s", width[i], "<VAL/ERR>");
          } else {
            j = (p->prec >> 5) & 0x3;
            if (j == 1) {
              fmt = "%*.*f";
            } else if (j == 2) {
              fmt = "%*.*E";
            } else {
#if 0
              fmt = "%*.*g";
#else
              fmt = ((fabs(p->value) < .01)) || ((fabs(p->value) > 1e10)) ? "%*.*E" : "%*.*f";
#endif
            }
            printf(fmt, width[i], p->prec & 0x1f, p->value);
          }
        }
      }
    }
  }
  fprintf(f, "\n");
}

void out_page(FILE *f, short *jrow, short jstart, short jend,
              short max_r) { /* local to print_out() */
  short irow;

  out_head(f);
  out_top(f, jstart, jend);
  irow = *jrow;
  while (*jrow <= max_r) {
    if (*jrow-irow == 50) {
      out_page(f, jrow, jstart, jend, max_r);  /* recursive call! */
      irow = *jrow;
    } else {
      out_line(f, *jrow, jstart, jend);
      ++(*jrow);
    }
  }
}

void print_out() {
  FILE *outf;
  short i, max_r, max_c, jstart, jend, len;
  struct cell *head;

  i = 0;
  strcpy(&buffer[ip], "sheet.prn");
  outf = fopen(buffer, "w");
  if (outf) {
    max_r = 0;
    max_c = 0;
    head = chead;
    while (head) {
      if (head->row > max_r) max_r = head->row;
      if (head->col > max_c) max_c = head->col;
      head = head->next;
    }
    for (jstart = 0; jstart <= max_c; ) {
      len = 5;
      jend = jstart;
      for (i = jstart; (i <= max_c) && (len+width[i] < 132); ++i) {
        len += width[i];
        jend = i;
      }
      i = 0;
      out_page(outf, &i, jstart, jend, max_r);
      jstart = jend+1;
    }
    fclose(outf);
  }
}

void delete(struct cell *work) {  /* local to do_command() */
  short i;
  struct cell *head, *curr;

  head = chead;
  while (head) {
    curr = head->next;
    if (((del_row != 0) && (head->row == cur_r)) ||
        ((del_col != 0) && (head->col == cur_c))) {
      do_dispose(head);
    }
    head = curr;
  }
  clr_ready();
  head = chead;
  while (head) {
    curr = head->next;
    if ((head->ready == 0) && (head->flag == 'F')) {
      move(head, work);
      i = 0;
      work->value = formula(work, &i);
      if (work->alen > head->alen) {
        do_dispose(head);
        head = get_new(work->alen);
        if (!chead) chead = head;
        if (ctail) ctail->next = head;
        ctail = head;
        head->next = NULL;
      }
      move(work, head);
      head->ready = 1;
    }
    head = curr;
  }
  head = chead;
  while (head) {
    if ((head->row >= test_r) && (head->col >= test_c)) {
      head->row += del_row;
      head->col += del_col;
    }
    head->ready = 0;
    head = head->next;
  }
}

void insert(struct cell *work) {  /* local to do_command() */
  short i;
  struct cell *head, *curr;

  clr_ready();
  head = chead;
  while (head) {
    curr = head->next;
    if ((head->ready == 0) && (head->flag == 'F')) {
      move(head, work);
      i = 0;
      work->value = formula(work, &i);
      if (work->alen > head->alen) {
        do_dispose(head);
        head = get_new(work->alen);
        if (!chead) chead = head;
        if (ctail) ctail->next = head;
        ctail = head;
        head->next = NULL;
      }
      move(work, head);
      head->ready = 1;
    }
    head = curr;
  }
  head = chead;
  while (head) {
    if ((head->row >= test_r) && (head->col >= test_c)) {
      head->row += del_row;
      head->col += del_col;
    }
    head->ready = 0;
    head = head->next;
  }
}

void common() {  /* local to do_command() */
  state = parameter;
  gotoxy(23, 0); /* 23, 9); */
  clr_rest();
}

void do_command() {
  short i, r, c;
  struct cell *head;

  error = false;
  calc_error = false;
  state = wait;
  global = false;
  switch (buffer[1]) {
    case 'W':
      common();
      command_type = 1;
      put_string("Column width: ");
      break;
      
    case 'L':
      common();
      command_type = 2;
      put_string("Load spreadsheet: ");
      break;
      
    case 'S':
      common();
      command_type = 3;
      put_string("Save spreadsheet as: ");
      break;

    case 'F':
      common();
      command_type = 4;
      if (buffer[2] == 'G') global = true;
      put_string("N)umber, S)cientific, G)eneral: ");
      break;

    case 'P':
      common();
      command_type = 5;
      if (buffer[2] == 'G') global = true;
      put_string("Digits of precision: ");
      break;

    case 'J':
      common();
      command_type = 6;
      if (buffer[2] == 'G') global = true;
      put_string("Justify text L)eft, R)ight or C)enter: ");
      break;

    case 'T':
#if 0
      command_type = 7;
      state = execute_parameter;
#else
      if (buffer[2] == 'C') {
        color = !color;
        resrev(0);
        display(home_r, home_c);
      } else if (buffer[2] == 'W') {
        if (screen == 80) {
          screen = 132;
          put_string("\033[?3h");
          display(home_r, home_c);
        } else if (screen == 132) {
          screen = 80;
          put_string("\033[?3l");
          if (!on_screen(cur_r, cur_c)) home_c = cur_c;
          display(home_r, home_c);
        }
      }
#endif
      break;

    case 'H':
      common();
      command_type = 8;
      put_string("New heading: ");
      break;

    case 'I':
      if ((buffer[2] == 'R') || (buffer[2] == 'C')) {
        if (buffer[2] == 'R') {
          del_row = 1;
          del_col = 0;
          test_r = cur_r;
          test_c = 0;
        } else {
          del_row = 0;
          del_col = 1;
          test_r = 0;
          test_c = cur_c;
          for (i = 254; i >= cur_c; --i) width[i+1] = width[i];
        }
        local_edit = true;
        insert((struct cell *) &work_cell);
        local_edit = false;
        del_row = 0;
        del_col = 0;
        test_r = 0;
        test_c = 0;
        re_calc();
        display(home_r, home_c);
      }
      break;

    case 'D':
      if ((buffer[2] == 'R') || (buffer[2] == 'C')) {
        if (buffer[2] == 'R') {
          del_row = -1;
          del_col = 0;
          test_r = cur_r;
          test_c = 0;
        } else {
          del_row = 0;
          del_col = -1;
          test_r = 0;
          test_c = cur_c;
          for (i = cur_c; i <= 254; ++i) width[i] = width[i+1];
        }
        local_edit = true;
        delete((struct cell *) &work_cell);
        local_edit = false;
        del_row = 0;
        del_col = 0;
        test_r = 0;
        test_c = 0;
        re_calc();
        display(home_r, home_c);
      }
      break;

    case '=':
      state = wait;
      if (ip >= 2) {
        for (i = 2; i < ip; ++i) work_cell.form[i-2] = buffer[i];
        ip -= 2;
        work_cell.form[ip+1] = '\0';
        head = (struct cell *) &work_cell;
        ip = 0;
        get_xy(head, &ip, &r, &c);
        if ((r >= 0) && (r <= 255) && (c >= 0) && (c <= 255)) {
          if (on_screen(r, c)) {
            head = get_point(cur_r, cur_c);
            posit(cur_r, cur_c);
            dis_val(head, cur_c);
            cur_r = r;
            cur_c = c;
          } else {
            cur_r = r;
            home_r = r;
            cur_c = c;
            home_c = c;
            display(home_r, home_c);
          }
        }
      }
      break;

    case 'R':
      common();
      command_type = 9;
      put_string("Replicate range: ");
      break;
  
    case 'O':
      common();
      command_type = 10;
      put_string("Print spreadsheet to: ");
      break;
      
    case 'C':
      common();
      command_type = 11;
      put_string("Copy from: ");
      break;

    case 'E':
      common();
      command_type = 12;
      put_string("Erase range: ");
      break;

    case '?':
      help();
      break;

    case '!':
      state = wait;
      display(home_r, home_c);
      break;

    case 'X':
      common();
      command_type = 13;
      put_string("Delete all? (Y/N): ");
      break;

    case 'Q':
      common();
      command_type = 14;
      put_string("Quit? (Y/N): ");
      break;
  }
  if (state == wait) prompt(cur_r, cur_c);
  clr_buf();
  ip = 0;
}

short get_con(short ipp) {  /* local to do_parameter() */
  return (short) atoi(buffer);
}

char *signature = "FreeCalc";

void load(char *filename) {
  /*short i;*/
  struct cell *head;
  struct fcell file_cell;
  byte buf[256];
  FILE *f;
  unsigned short count;

  f = fopen(filename, "r");
  if (!f) return; /* error: file not found */

  if (fread(buf, 1, 16, f) != 16) goto error1;
  if (fread(heading, 1, 40, f) != 40) goto error1;
  if (fread(width, 1, sizeof(width), f) != sizeof(width)) goto error1;
  fread(&home_r, 1, 1, f);
  fread(&home_c, 1, 1, f);
  fread(&cur_r, 1, 1, f);
  fread(&cur_c, 1, 1, f);
  fread(&count, 2, 1, f);

  dispose_all();  /* delete old spreadsheet */

  while (!feof(f) && count) {
    if (fread(&file_cell, 1, sizeof(file_cell), f) != sizeof(file_cell)) goto error1;
    if (file_cell.alen > 0) {
      fread(buf, 1, file_cell.alen, f);
      if (file_cell.alen > 69) file_cell.alen = 69;
      if (buf[file_cell.alen-1]) {
        buf[++file_cell.alen] = 0;
      }
    }
    head = get_new(file_cell.alen);
    if (!chead) chead = head;
    if (ctail) ctail->next = head;
    ctail = head;
    head->row = file_cell.row;
    head->col = file_cell.col;
    head->flag = file_cell.flag;
    head->ready = 0;
    head->next = NULL;
    head->align = file_cell.align;
    head->prec = file_cell.prec;
    head->value = bcd_to_f(file_cell.bcd_value);
    head->alen = file_cell.alen;
    if (head->alen > 0) memcpy(head->form, buf, head->alen);
    --count;
  }
  goto done1;
error1:
  cur_r = 0;
  cur_c = 0;
  home_r = 0;
  home_c = 0;
done1:
  fclose(f);
}

void save(char *filename) {
  struct cell *head;
  FILE *f;
  struct fcell file_cell;
  byte *bcd_val, buf[16];
  unsigned short count;
  
  memset(buf, 0, 16);
  memcpy(buf, signature, 8);
  buf[8] = 1; buf[9] = 2; /* version */
  f = fopen(filename, "w");
  if (f) {
    fwrite(buf, 16, 1, f);
    fwrite(heading, 40, 1, f);
    fwrite(width, sizeof(width), 1, f);
    fwrite(&home_r, 1, 1, f);
    fwrite(&home_c, 1, 1, f);
    fwrite(&cur_r, 1, 1, f);
    fwrite(&cur_c, 1, 1, f);
    count = 0;
    head = chead;
    while (head) { ++count; head = head->next; } /* count cells */
    fwrite(&count, 2, 1, f);
    head = chead;
    while (head) {
      file_cell.row = head->row;
      file_cell.col = head->col;
      file_cell.flag = head->flag;
      bcd_val = f_to_bcd(head->value);
      memcpy(file_cell.bcd_value, bcd_val, 6);
      file_cell.align = head->align;
      file_cell.prec = head->prec;
      file_cell.alen = head->alen;
      fwrite(&file_cell, sizeof(file_cell), 1, f);
      if (head->alen > 0) fwrite(head->form, head->alen, 1, f);
      head = head->next;
    }
    fclose(f);
  }
}

void do_parameter() {
  short i, j, r, c, r1, c1, r2, c2;
  struct cell *head, *curr, *work;

  state = wait;
  switch (command_type) {
    case 1:  /* column width */
      j = get_con(ip);
      if (ip > 0) {
        /*if (j == 1) j = 2;*/
        width[cur_c] = j;
        if (!on_screen(cur_r, cur_c)) home_c = cur_c;
        display(home_r, home_c);
      }
      break;

    case 2:  /* load spreadsheet */
      if (ip > 0) {
        strcpy(&buffer[ip], ".cal");  /* strcat(buffer, ...) */
        load(buffer);
        display(home_r, home_c);
      }
      break;

    case 3:  /* save spreadsheet */
      if (ip > 0) {
        strcpy(&buffer[ip], ".cal");  /* strcat(buffer, ...) */
        save(buffer);
      }
      break;

    case 4:  /* numeric format */
      upper();
      i = -1;
           if (buffer[0] == 'G') i = 0;
      else if (buffer[0] == 'N') i = 1;
      else if (buffer[0] == 'S') i = 2;
      if (i >= 0) {
        if (global) precision = (precision & 0x1f) | (i << 5);
        head = get_point(cur_r, cur_c);
        if (head) head->prec = (head->prec & 0x1f) | (i << 5);
      }
      break;

    case 5:  /* precision */
      j = get_con(ip);
      if (j <= 15) {
        if (global) precision = (precision & ~0x1f) | (j & 0x1f);
        head = get_point(cur_r, cur_c);
        if (head) head->prec = (head->prec & ~0x1f) | (j & 0x1f);
      }
      break;

    case 6:  /* justify */
      upper();
      i = -1;
           if (buffer[0] == 'R') i = 1;
      else if (buffer[0] == 'C') i = 2;
      else if (buffer[0] == 'L') i = 0;
      if (i >= 0) {
        head = get_point(cur_r, cur_c);
        if (head) head->align = i;
        if (global) justify = i;
      }
      break;

#if 0
    case 7:  /* toggle screen width */
      if (screen == 80) {
        screen = 132;
        put_string("\033[?3h");
        display(home_r, home_c);
      } else if (screen == 132) {
        screen = 80;
        put_string("\033[?3l");
        if (!on_screen(cur_r, cur_c)) home_c = cur_c;
        display(home_r, home_c);
      }
      break;
#endif

    case 8:  /* set heading */
      if (ip > 0) {
        set_heading(buffer, ip);
        if (color) resrev(1);
        gotoxy(0, 0);
        clr_rest();
        i = (screen-40)/2;
        gotoxy(0, i);
        printf("%40s", heading);
        if (color) resrev(0);
        fflush(stdout);
      }
      break;

    case 9:  /* replicate cell */
      upper();
      work = (struct cell *) &work_cell;
      for (i = 0; i < 70; ++i) work_cell.form[i] = buffer[i];
      work_cell.form[ip] = '\0';
      i = 0;
      if (!get_range(work, &i, &r1, &c1, &r2, &c2)) {
        r2 = r1;
        c2 = c1;
      }
      test_r = 0;
      test_c = 0;
      curr = get_point(cur_r, cur_c);
      if (curr && (r1 >= 0) && (r1 <= 255) && (c1 >= 0) && (c1 <= 255) &&
          (r2 >= r1) && (c2 >= c1)) {
        for (r = r1; r <= r2; ++r) {
          for (c = c1; c <= c2; ++c) {
            head = get_point(r, c);
            if (head != curr) {
              if (head) do_dispose(head);
              move(curr, work);
              work_cell.row = r;
              work_cell.col = c;
              local_edit = true;
              del_row = r-cur_r;
              del_col = c-cur_c;
              error = false;
              calc_error = false;
              i = 0;
              work->value = formula(work, &i);
              work->value = -0.000095623781;
              head = get_new(work->alen);
              ctail->next = head;
              ctail = head;
              move(work, head);
              head->next = NULL;
              head->ready = 0;
              local_edit = false;
            }
          }
        }
        clr_ready();
        re_calc();
      }
      break;

    case 10:  /* print spreadsheet */
      upper();
      print_out();
      break;

    case 11:  /* copy cells */
      upper();
      work = (struct cell *) &work_cell;
      for (i = 0; i < 70; ++i) work_cell.form[i] = buffer[i];
      work_cell.form[ip+1] = '\0';
      i = 0;
      if (!get_range(work, &i, &r1, &c1, &r2, &c2)) {
        r2 = r1;
        c2 = c1;
      }
      test_r = 0;
      test_c = 0;
      if ((r1 >= 0) && (r1 <= 255) && (c1 >= 0) && (c1 <= 255) &&
          (r2 >= r1) && (c2 >= c1)) {
        for (r = r1; r <= r2; ++r) {
          for (c = c1; c <= c2; ++c) {
            head = get_point(r, c);
            curr = get_point(cur_r+r-r1, cur_c+c-c1);
            if (curr != head) {
              if (curr) do_dispose(curr);
              if (head) {
                move(head, work);
                work_cell.row = cur_r+r-r1;
                work_cell.col = cur_c+c-c1;
                local_edit = true;
                del_row = cur_r-r1;
                del_col = cur_c-c1;
                error = false;
                calc_error = false;
                i = 0;
                work->value = formula(work, &i);
                work->value = -0.000095623781;
                curr = get_new(work->alen);
                ctail->next = curr;
                ctail = curr;
                move(work, curr);
                curr->next = NULL;
                curr->ready = 0;
                local_edit = false;
              }
            }
          }
        }
        clr_ready();
        re_calc();
      }
      break;

    case 12:  /* erase range */
      upper();
      work = (struct cell *) &work_cell;
      for (i = 0; i < 70; ++i) work_cell.form[i] = buffer[i];
      work_cell.form[ip+1] = '\0';
      i = 0;
      if (!get_range(work, &i, &r1, &c1, &r2, &c2)) {
        r2 = r1;
        c2 = c1;
      }
      if ((r1 >= 0) && (r1 <= 255) && (c1 >= 0) && (c1 <= 255) &&
          (r2 >= r1) && (c2 >= c1)) {
        for (r = r1; r <= r2; ++r) {
          for (c = c1; c <= c2; ++c) {
            head = get_point(r, c);
            if (head) do_dispose(head);
          }
        }
        clr_ready();
        re_calc();
        display(home_r, home_c);
      }
      break;

    case 13: /* delete spreadsheet */
      upper();
      if (buffer[0] == 'Y') {
        dispose_all();
        reset_calc();
        display(home_r, home_c);
      }
      break;

    case 14: /* quit */
      upper();
      if (buffer[0] == 'Y') done = true;
      break;
  }
  ip = 0;
  clr_buf();
  prompt(cur_r, cur_c);
}

void calculate_formula() {
  struct cell *p, *head;
  short i, j, k;
  byte save_justify, save_prec;
  char save_flag;

  error = false;
  calc_error = false;
  if (ip < 70-1) buffer[ip++] = '\0';
  j = 0;
  k = ip;
  if (buffer[0] == '"') {
    ++j;
    --k;
  }
  save_justify = justify;
  save_prec = precision;
  save_flag = '\0';
  p = get_point(cur_r, cur_c);
  if (!p) {
    p = get_new(k);
  } else {
    save_justify = p->align;
    save_prec = p->prec;
    save_flag = p->flag;
    do_dispose(p);
    p = get_new(k);
  }
  for (i = 0; i < k; ++i) p->form[i] = buffer[i+j];
  if (!chead) chead = p;
  if (ctail) ctail->next = p;
  ctail = p;
  p->next = NULL;
  p->row = cur_r;
  p->col = cur_c;
  p->flag = 'A';  /* assume alpha */
  p->ready = 2;
  p->value = 0;
  p->align = save_justify;
  p->prec = save_prec;
  p->alen = k;
  if (buffer[0] && strchr("+-.@0123456789", buffer[0])) p->flag = 'F'; /* formula */
  if (p->flag == 'F') {
    for (i = 0; i < ip; ++i) p->form[i] = toupper(p->form[i]);
    head = chead;
    while (head) {
      if (head->ready > 1) head->value = -0.0000980162302;
      head->ready = 0;
      head = head->next;
    }
    p->ready = 2;
    i = 0;
    p->value = formula(p, &i);
    if (calc_error) p->ready = 2; else p->ready = 1;
    if (error) p->ready = 3;
    posit(cur_r, cur_c);
    dis_val(p, cur_c);
    re_calc();
  } else {
    posit(cur_r, cur_c);
    dis_val(p, cur_c);
    if (save_flag == 'F') {
      clr_ready();
      p->ready = 2;
      re_calc();
    }
  }
  if (p->ready != 3) {
    switch (direction) {
      case up:
        if (cur_r > 0) {
          if (cur_r <= home_r) vertical(-1); else --cur_r;
        }
        break;

      case down:
        if (cur_r < 255) {
          if (cur_r >= home_r+19) vertical(1); else ++cur_r;
        }
        break;
        
      case left:
        if (cur_c > 0) {
          if (cur_c <= home_c) horizontal(-1); else --cur_c;
        }
        break;
        
      case right:
        if (cur_c < 255) {
          if (on_screen(cur_r, cur_c+1)) ++cur_c; else horizontal(1);
        }
        break;
        
      default:
        break;
    }
  }
  prompt(cur_r, cur_c);
  state = wait;
  ip = 0;
  clr_buf();
}

void p_process_char(char ch) {
  if (ch == '\033') {
    if (state == wait) process_cursor();
  } else if (ch == '\r') {
    buffer[ip] = '\0';
    if (state == command)
      state = execute_command;
    else if (state == formulas)
      state = execute_formula;
    else if (state == parameter)
      state = execute_parameter;
  } else if (ch == '\177' || ch == '\010') {
    if (state == command || state == formulas || state == parameter)
      if (ip > 0) {
        --ip;
        printf("\010 \010");
        fflush(stdout);
        if ((ip == 0) && (state != parameter)) state = wait;
      }
  } else {
    if (state == wait || state == command ||
        state == formulas || state == parameter) {
      if (ip < 70-1) {
        if ((ip == 0) && (state != parameter))
          state = (ch == '/') ? command : formulas;
        if (state == command) ch = toupper(ch);
        buffer[ip++] = ch;
        put_char(ch);
        fflush(stdout);
      } else {
        bell();
      }
    }
  }
}

void spread() {
  char ch;

  direction = down;
  state = wait;
  done = false;
  ip = 0;
  clr_buf();
  while (!done) {
    ch = get_char();
    p_process_char(ch);
    if (state == execute_formula) calculate_formula();
    if (state == execute_command) do_command();
    if (state == execute_parameter) do_parameter();
  }
}

int main(int argc, char *argv[]) {
  init_term();
  reset_calc();
  resrev(0);
  clear_scr();
  screen = 80;
  color = false;
  if (argv[1]) load(argv[1]);
  display(0, 0);
  spread();
  put_char('\r');
  resrev(0);
  attrib_off();
  clr_rest();
  reset_term();
  return 0;
}
