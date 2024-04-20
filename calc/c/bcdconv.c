#include <stdio.h>
#include <string.h>

#include "bcdconv.h"

/* #define _DEBUG_ */

#ifndef bool
#define bool byte
#define false 0
#define true  (!false)
#endif

byte *f_to_bcd(double val) {
  /* convert C double to 6-byte BCDLIB-compatible format */
  byte mantissa[10], exp, sign, esign;
  static byte result[6];
  char *p, buf[20];
  int i;
  
  memset(mantissa, 0, sizeof(mantissa));
  memset(result, 0, sizeof(result));
  if (val == 0.0) return result;
  
  snprintf(buf, 20, "%.10E", val);

  p = buf;
  sign = 0;
  /* first, mantissa */
  i = 0;  /* BCD digit counter */
  while (*p) {
    if ((*p == ' ') || (*p == '.')) { ++p; continue; }
    if (*p == 'E') break;
    if (*p == '-') {
      sign = 1; /* negative value */
    } else {
      if (i < 10) mantissa[i++] = *p - '0';
    }
    ++p;
  }
  if (*p) ++p; /* skip 'E' */
  /* exponent */
  exp = 0;
  esign = 0;
  while (*p) {
    if (*p == '+') { ++p; continue; }
    if (*p == '-') {
      esign = 1; /* negative exponent */
    } else {
      exp = exp * 10 + *p - '0';
    }
    ++p;
  }
  
  /* now pack everything */
  exp = (esign) ? 0x80 - exp : 0x80 + exp;
  if ((exp & 1) == 0) {
    for (i = 9; i >= 1; --i) mantissa[i] = mantissa[i-1];
    mantissa[0] = 0;
  }
  result[0] = (exp >> 1);
  if (sign) result[0] |= 0x80;
  
  for (i = 0; i < 5; ++i) {
    result[i+1] = (mantissa[i*2] << 4) | mantissa[i*2+1];
  }
  
#ifdef _DEBUG_
//  printf("\r\n");
  for (i = 0; i < 6; ++i) printf("%02X ", result[i]);
  printf(" <= %s\r\n", buf);
#endif

  return result;
}

double bcd_to_f(byte *bcd) {
  /* convert 6-byte BCD floating point value to C double */
  double f;
  int  i;
  char *p, buf[30];
  byte exp, digit;
  bool first;

  f = 0.0;
  exp = bcd[0];
  if (exp != 0) {
    p = buf;
    if (exp & 0x80) *p++ = '-';
    /* convert mantissa */
    first = true;
    for (i = 1; i < 6; ++i) {
      digit = (bcd[i] >> 4) & 0x0F;
      *p++ = digit + '0';
      if (first) { *p++ = '.'; first = false; }
      digit = bcd[i] & 0x0F;
      *p++ = digit + '0';
    }
    /* convert exponent */
    *p++ = 'E';
    exp = (exp << 1);
    if (exp < 0x80) {
      *p++ = '-';
      exp = 0x7F - exp;
    } else {
      *p++ = '+';
      exp = exp - 0x7F;
    }
    snprintf(p, 10, "%02u", exp);
    sscanf(buf, "%lg", &f);
#ifdef _DEBUG_
//    printf("\r\n");
    for (i = 0; i < 6; ++i) printf("%02X ", bcd[i]);
    printf(" => %s => %18.9E\r\n", buf, f);
#endif
  }
  
  return f;
}
