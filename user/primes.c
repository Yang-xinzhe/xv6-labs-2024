#include "kernel/types.h"
#include "user/user.h"
void primes(int[2]) __attribute__((noreturn));
void primes(int pfd[2]) {
    close(pfd[1]); // close pipe-write

    int num;
    if(read(pfd[0], &num, sizeof(int)) == 0) { // EOF
        exit(0);
    }

    printf("prime %d\n", num);

    int p[2]; // new pipe
    pipe(p);

    if(fork() == 0) { // child process
        close(pfd[0]); // close pipe-read
        primes(p);
    } else {    // parent process
        int n;
        while(read(pfd[0], &n, sizeof(int)) != 0){
            if (n % num != 0) {
                write(p[1], &n, sizeof(int));
            }
        }
        close(p[1]);
        close(pfd[0]);
        wait(0);
        exit(0);
    }
    exit(1);
}



int main(void) {
    int p[2];
    pipe(p); // p[0]-read p[1]-write

    int idx;

    if(fork() == 0) { // child process
        primes(p);
    } else {    // parent process
        close(p[0]); 
        for(idx = 2 ; idx <= 280 ; idx++) {
            write(p[1], &idx, sizeof(int));
        }
        close(p[1]);
        wait(0);
    }
}