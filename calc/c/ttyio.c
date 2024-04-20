#include <stdio.h>

#ifdef __linux__
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>

struct termios sterm, sraw;

fd_set readset;
int fd_width;
#endif

#ifdef CPM
#define SIGINT  1
#define SIG_IGN ((void (*)(int))1)
extern void (* signal(int, void(*)(int)))(int);
extern int  write(int, void *, int);
extern int  read(int, void *, int);
extern int  bios(int, int, int);
#endif

void init_term() {
#ifdef __linux__
  tcgetattr(fileno(stdin), &sterm);
  sraw = sterm;
  cfmakeraw(&sraw);
  tcsetattr(fileno(stdin), TCSANOW, &sraw);
#endif
#ifdef CPM
  signal(SIGINT, SIG_IGN);  /* disable ^C trap */
  fflush(stdout);  /* include _close (?) */
#endif
}

void reset_term() {
#ifdef __linux__
  tcsetattr(fileno(stdin), TCSANOW, &sterm);
#endif
}

int kb_hit() {
#ifdef __linux__
  int nf;
  struct timeval tmout;
  
  fd_width = fileno(stdin) + 1;

  FD_ZERO(&readset);
  FD_SET(fileno(stdin), &readset);

  tmout.tv_sec = tmout.tv_usec = 0;

  nf = select(fd_width, &readset, NULL, NULL, &tmout);
  if (nf > 0 && FD_ISSET(fileno(stdin), &readset)) return 1;
  return 0;
#endif
#ifdef CPM
  int c;

  /*c = bios(2, 0, 0) ? bios(3, 0, 0) : 0;*/
  c = bios(2, 0, 0) ? 1 : 0;
  return c;
#endif
}

int time_delay(int msec) {
#ifdef __linux__
  struct timeval tmout;

  if (msec <= 0) return -1;
  
  tmout.tv_sec = msec / 1000;
  tmout.tv_usec = (msec % 1000) * 1000;

  return select(1, NULL, NULL, NULL, &tmout);
#endif
#ifdef CPM
  int i, j, k;
  
  for (i = 0; i < msec; ++i) {
    for (j = 0; j < 20; ++j) {
      k = i + j;
    }
  }
#endif
}

int get_char() {
  int c;

#ifdef __linux__
  fflush(stdout);
  while (!kb_hit());
  c = 0;
  read(fileno(stdin), &c, 1);
#endif
#ifdef CPM
  /*while ((c = kb_hit()) == 0) {}*/
  while (!kb_hit());
  c = bios(3, 0, 0);
#endif

  return c;
}

void put_char(int c) {
#ifdef __linux
  write(fileno(stdout), &c, 1);
#endif
#ifdef CPM
  bios(4, c, 0);
#endif
}

void put_string(char *str) {
  while (*str) put_char(*str++);
}

void get_line(char *str, int maxlen) {
    int c, length;

    length = 0;
    while ((c = get_char()) != 0x0d) {

        if (c == 3) {
            length = 0;
            break;
        }

        if (((c == 0x08) || (c == 0x7F)) && (length > 0)) {
            put_char(0x08); put_char(' '); put_char(0x08);
            length--;
        } else if (c == 0x18) {
            while (length > 0) {
                put_char(0x08); put_char(' '); put_char(0x08);
            }
        } else if (c < 0x20 || c == 0x7F) {
            /* Ignore control characters. */
            continue;
        } else {
            if (length < maxlen) {
                str[length++] = c;
                put_char(c);
            }
        }
    }

    str[length] = 0;
}
