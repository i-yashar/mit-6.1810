#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int);

int main(int argc, char *argv[]) {
  int p[2];

  int isForked = 0;

  for(int i = 2; i < 35; i++) {
    if(!isForked) {
        pipe(p);

        if(fork() == 0) {
            close(p[1]);
            sieve(p[0]);
            close(p[0]);
        } else {
            close(p[0]);
            isForked = 1;
        }
    }

    write(p[1], &i, sizeof(int));
  }

  close(p[1]);
  wait(0);

  exit(0);
}

void sieve(int readEnd) {
    int prime;

    read(readEnd, &prime, sizeof(int));
    fprintf(1, "prime %d\n", prime);

    int p[2];
    int nextNumber;
    int isForked = 0;

    while(read(readEnd, &nextNumber, sizeof(int)) != 0) {
        if(nextNumber % prime != 0) {
            if(!isForked) {
                pipe(p);

                if(fork() == 0) {
                    close(p[1]);
                    sieve(p[0]);
                    close(p[0]);
                }

                close(p[0]);
                isForked = 1;
            }

            write(p[1], &nextNumber, sizeof prime);
        }
    }

    if(isForked) {
        close(p[1]);
        wait(0);
    }

    exit(0);
}