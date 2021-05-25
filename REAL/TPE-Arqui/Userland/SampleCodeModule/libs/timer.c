// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscallsASM.h>
#include <timer.h>
#include <stdlib.h>

int readSeconds(){
  uint8_t ans;
  getTimeSyscall(0,&ans);
  return ans;
}

int readMinutes(){
  uint8_t ans;
  getTimeSyscall(1,&ans);
  return ans;
}

int readHours(){
  int ans = 0;
  getTimeSyscall(2,(uint8_t*)&ans);
  ans = (ans-3)%24;
  return (ans);
}

int readDays(){
  uint8_t ans;
  getTimeSyscall(3,&ans);
  return ans;
}

int readMonths(){
  uint8_t ans;
  getTimeSyscall(4,&ans);
  return ans;
}

int readYear(){
  uint8_t ans;
  getTimeSyscall(5,&ans);
  return ans;
}


int totalTics(){
  uint64_t sec;
  getTicksSyscall(&sec);
  return sec;
}
