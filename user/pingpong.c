#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p[2];
    char buf[1];

    pipe(p);
    if (fork() == 0) {
        read(p[0], buf, 1);
        close(p[0]);
        printf("%d: received ping\n", getpid());
        write(p[1], buf, 1);
        close(p[1]);
        exit(0);
    }

    write(p[1], "b", 1);
    close(p[1]);
    wait(0);
    read(p[0], buf, 1);
    close(p[0]);
    printf("%d: received pong\n", getpid());
    exit(0);
}

