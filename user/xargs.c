#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {

    char *param[MAXARG];
    char buf[512];
    int n;

    for(int i = 1 ; i < argc ; ++i) {
        param[i-1] = argv[i];
    }

    while((n = read(0, buf, sizeof(buf))) > 0) {
        int i = 0;
        while(i < n) {
            int j = 0;
            char line[256];
            while(i < n && buf[i] != '\n' && j < sizeof(line) - 1) {
                line[j++] = buf[i++];
            }
            if(buf[i] == '\n') {
                i++;
            }
            line[j] = 0;

            int arg_index = argc - 1;
            char *p = line;
            while(*p) {
                while(*p == ' ') p++; // remove ' '
                if(*p == 0) break; // EOF

                param[arg_index++] = p;

                while(*p && *p != ' ') p++;
                if(*p) {
                    *p = 0;
                    p++;
                }

                if(arg_index >= MAXARG - 1){
                    break;
                }
            }
            param[arg_index] = 0;

            if(fork() == 0) {
                exec(param[0], param);
                fprintf(2, "exec failed\n");
                exit(1);
            } else {
                wait(0);
            }
        }
    }

    exit(0);
}