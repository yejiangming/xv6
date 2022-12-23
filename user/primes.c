#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int sieve(int fd);

int
main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        // 子进程
        close(fd[1]);
        sieve(fd[0]);
    } else {
        // 父进程
        close(fd[0]);
        for (int i = 2; i <= 35; i++) {
            write(fd[1], (char*)&i, 4);
        }
        close(fd[1]);
        wait(0);
    }
    exit(0);
}

// 从 fd 读取数据
int sieve(int input) {
    int p = 0;
    int n;
    int hasChild = 0;
    int fd[2];
    while(1) {
        int num = read(input, &n, 4);
        if(num == 0) {
            break;
        }
        if (p == 0) {
            p = n;
            fprintf(1, "prime %d\n", p);
        } else {
            if (n % p != 0) {
                if (hasChild == 0) {
                    pipe(fd);
                    if (fork() == 0) {
                        // 子进程
                        close(fd[1]);
                        sieve(fd[0]);
                    } else {
                        close(fd[0]);
                        hasChild = 1;
                    }
                }
                write(fd[1], (char*)&n, 4);
            }
        }
    }
    if (hasChild == 1) {
        close(fd[1]);
        wait(0);
    }
    exit(0);
}
