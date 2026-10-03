#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
  uint64 satp_val = getsatp();

  // Using %p to print the 64-bit hardware register value in Hexadecimal
  printf("RISC-V satp hardware register value: %p\n", (void*)satp_val);

  exit(0);
}