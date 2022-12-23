#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"



int
main(int argc, char *argv[])
{
    
    char buf[100];
    char *p;
    while(1) {
        p = gets(buf, 100);
        if (p[0] == '\0') {
            break;
        }
        // 读到的是带换行的数据
        p[strlen(p)-1] = '\0';
        if (fork() == 0) {
            // child
            char *new_argv[MAXARG];
            for (int i = 0; i < argc; i++) {
                new_argv[i] = argv[i];
            }
            new_argv[argc] = p;
            exec(new_argv[1], new_argv+1);
        } else {
            // parent
            wait(0);
        }
    }

    exit(0);
}
