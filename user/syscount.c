#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
  int count = getsyscount();
  printf("Total system calls made by this program: %d\n", count);
  exit(0);
}