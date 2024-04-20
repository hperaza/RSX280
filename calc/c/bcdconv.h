#ifndef __bcdconv_h
#define __bcdconv_h

#ifndef byte
typedef unsigned char byte;
#endif

byte *f_to_bcd(double val);
double bcd_to_f(byte *bcd);

#endif
