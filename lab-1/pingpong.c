#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    int parentToChild[2], childToParent[2];

    pipe(parentToChild);
    pipe(childToParent);

    if(fork() == 0) {
        close(childToParent[0]);
        close(parentToChild[1]);
        
        char buf[1];
        read(parentToChild[0], buf, 1);
        fprintf(1, "%d: received ping\n", getpid());

        write(childToParent[1], buf, sizeof buf);

        close(childToParent[1]);
        close(parentToChild[0]);

        exit(0);
    } else {
        close(parentToChild[0]);
        close(childToParent[1]);

        write(parentToChild[1], "", 1);
        wait(0);

        fprintf(1, "%d: received pong\n", getpid());

        close(parentToChild[1]);
        close(childToParent[0]);
    }
  
  exit(0);
}