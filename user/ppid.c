#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
  int pid = getpid();
  int ppid = getppid();
  printf("My Process ID (PID) is: %d\n", pid);
  printf("My Parent's ID (PPID) is: %d\n", ppid);
  exit(0);
}