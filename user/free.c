#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
  printf("Total Free Memory: %d bytes\n", freemem());
  exit(0);
}