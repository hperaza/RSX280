#ifndef __ttyio_h
#define __ttyio_h

void init_term();
void reset_term();
int  kb_hit();
int  time_delay(int msec);
int  get_char();
void put_char(int c);
void put_string(char *str);
void get_line(char *str, int maxlen);

#endif
