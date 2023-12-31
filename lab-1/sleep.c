#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if(argc < 2) {
    fprintf(2, "sleep expects one argument, e.g 'sleep <time>'\n");
    exit(1);
  }

  int duration = atoi(argv[1]);

  sleep(duration);
  
  exit(0);
}
