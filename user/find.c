#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find_func(char *path, char *key) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
            for(p = path + strlen(path); p >= path && *p != '/'; p--)
                ;
            p++;
            if (strcmp(p, key) == 0)
                printf("%s\n", path);
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                if ((strcmp(de.name, ".") != 0) && (strcmp(de.name, "..") != 0))
                    find_func(buf, key);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if(argc < 3){
        fprintf(2, "Usage: find...\n");
        exit(1);
    }

    char *path = argv[1];
    char *key = argv[2];
    find_func(path, key);

    exit(0);
}
