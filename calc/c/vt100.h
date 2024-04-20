#ifndef __VT100_H
#define __VT100_H

void clear_scr(void);
void clear_eos(void);
void clear_eol(void);
void cursor_home(void);
void cursor_off(void);  /* ANSI */
void cursor_on(void);   /* ANSI */
void graph_mode(void);
void alpha_mode(void);
void bold_mode();
void underline_mode();
void reverse_mode();
void set_color(int fg, int bg);
void attrib_off();
void goto_xy(int x, int y);
int  printf_at(int x, int y, char *str, ...);

#endif /* __VT100_H */
