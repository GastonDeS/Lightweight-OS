#ifndef DATE_DRIVER
#define DATE_DRIVER
#include <date_asm.h>
#include <date_driver.h>
#include <stdint.h>

uint8_t getSeconds() {
  return getTimeInfo(0);
}

uint8_t getMinutes() {
  return getTimeInfo(2);
}

uint8_t getHours() {
  return getTimeInfo(4);
}

uint8_t getDay() {
  return getTimeInfo(7);
}

uint8_t getMonth() {
  return getTimeInfo(8);
}

uint8_t getYear() {
  return getTimeInfo(9);
}



#endif
