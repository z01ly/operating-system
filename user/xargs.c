#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char *argv[]) {
    if(argc < 2) {
         fprintf(2, "Usage: xargs...\n");
         exit(1);
     }

    char* command = argv[1];
    int readline = 1;
    char p;
    char* new_argv[MAXARG];
    char buf[MAXARG - argc + 1][32];

    while (readline) {
        memset(buf, 0, sizeof buf);
        int pos = 0;
        int line = 0;
        int result;
        // It would be better to consider more than one space.
        int space_num = 0;

        for(int i = 1; i < argc; i++) {
            // First malloc then strcpy. Ohterwise new_argv[i - 1] would be null.
            new_argv[i-1] = (char*)malloc(strlen(argv[i]) + 1);
            strcpy(new_argv[i-1], argv[i]);
        }

        while ((result = read(0, &p, 1)) != 0) {
            if (p == '\n')
                 break;

            if (p != ' ') {
                buf[line][pos] = p;
                pos++;
                space_num = 0;
            } else if (p == ' ') {
                space_num++;
                if (space_num == 1) {
                    line++;
                    pos = 0;
                }
            }

        }

        if (result == 0) {
            readline = 0;
            break;
        }

        for (int i = 0; i < MAXARG - argc + 1 - 1; i++) {
            new_argv[argc - 1 + i] = buf[i];
        }
        // The last one should be set to 0. See xv6 book 1.3 code.
        new_argv[MAXARG - 1] = 0;

        if (fork() == 0) {
            exec(command, new_argv);
            exit(0);
        } else {
            wait(0);
        }
    }
    
    exit(0);
}
