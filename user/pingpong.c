#include "kernel/types.h"
#include "user/user.h"

int main(void) {
    int p[2]; // user pipe
    char buf[10];

    pipe(p); // p[0]-read p[1]-write

    if(fork() == 0) { // child process
        read(p[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
        write(p[1], "pong", 4);
        exit(0);
    } else {        // parent process
        write(p[1], "ping", 4);
        wait(0);
        read(p[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
        exit(0);
    }
    exit(0);
}