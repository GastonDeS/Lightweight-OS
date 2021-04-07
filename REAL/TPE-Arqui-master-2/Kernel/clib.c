#ifndef CHAR_LIB
#define CHAR_LIB
#include <clib.h>

uint8_t isPrintable(char ch) {
  if (ch >= '0' && ch <= '9') //numeric
    return 1;

  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) //alpha
    return 1;

  if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\b') //space
    return 1;

  if ((ch >= '!' && ch <= "/") || (ch >= ':' && ch <= '@') || (ch >= "[" && ch <= 0x60) || (ch >= "{" && ch <= "~")) {
    return 1;
  }

  return 0;
}

#endif
