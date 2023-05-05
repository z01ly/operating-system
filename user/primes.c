#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int recur_process(int *p) {
    int key, num, base_case;
    close(p[1]);
    read(p[0], &key, 4);
    printf("prime %d\n", key);
    
    base_case = read(p[0], &num, 4);
    if (base_case != 0) {
        int sub_p[2];
        pipe(sub_p);

        if (fork() == 0) {
            recur_process(sub_p);
        }

        close(sub_p[0]);
        if (num % key != 0)
            write(sub_p[1], &num, 4);
        while(read(p[0], &num, 4) != 0) {
            if (num % key != 0)
                write(sub_p[1], &num, 4);
        }
        close(p[0]);
        close(sub_p[1]);
    } else {
        close(p[0]);
    }

    wait(0);
    exit(0);
}

int main() {
    int p[2];
    pipe(p);

    if (fork() == 0)
        recur_process(p);

    close(p[0]);
    for(int i = 2; i < 36; i++) {
        write(p[1], &i, 4);
    }
    close(p[1]);
    wait(0);
    exit(0);
}
