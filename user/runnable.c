#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
  int count = runnable();
  printf("Total runnable processes waiting for CPU: %d\n", count);
  exit(0);
}