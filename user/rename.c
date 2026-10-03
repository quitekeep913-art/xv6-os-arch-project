#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int sleep(int);
  if(argc < 2){
    printf("Usage: rename <new_name>\n");
    exit(1);
  }

  setname(argv[1]);
  printf("Process name successfully changed to: %s\n", argv[1]);

  // pause for background
  while(1) {
      // loop
  } 
  exit(0);
}