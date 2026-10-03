#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(void)
{
  printf("Open File in start: %d\n", getopenfiles());

  // one  file (README) opens in read mode
  int fd = open("README", O_RDONLY);

  printf("Count after one file open: %d\n", getopenfiles());

  // File close 
  close(fd);

  exit(0);
}