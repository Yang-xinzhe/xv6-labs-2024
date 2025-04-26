#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char *fmtname(char *path) {
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for(p = path + strlen(path); p >= path && *p != '/' ; p--)
        ;
    p++; // first character

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find(char *path, char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return ;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        return ;
    }

    if(st.type != T_DIR) {
        close(fd);
        fprintf(2, "%s is not a directory\n", path);
        return ;        
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
        fprintf(2, "%s is too long\n", path);
        return;
    }


    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0)
            continue;
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) 
            continue;
        strcpy(buf, path);
        p = buf + strlen(buf); // point to the end of path
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if(stat(buf, &st) < 0) {
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }
        if(st.type == T_DIR) {
            find(buf, filename);
        } else if (st.type == T_FILE) {
            if(strcmp(de.name, filename) == 0) {
                printf("%s\n", buf);
            }
        }
    }
    close(fd);
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        fprintf(2, "Usage: find <path> <filename> \n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}