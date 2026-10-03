#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
  printf("Process is running on CPU Core: %d\n", getcpuid());
  exit(0);
}