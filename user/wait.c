#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char* argv[]) {
    int status;
    if(argc < 2) {
        printf("Usage: wait [pid]\n");
        exit(0);
    }

    int wait_pid = atoi(argv[1]);

    while(1) {
        int pid = wait(&status);
        if(pid < 0) {
            printf("wait: pid %d is not a child of this shell\n", wait_pid);
            exit(1);
        } else if (pid == wait_pid) {
            printf("wait: pid %d exit %d\n", pid, status);
        }

        printf("wait: pid %d exit, not target process\n", pid);
    }
    exit(0);
}