#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char buf[1];

    int fd_for_parent_write[2];
    int fd_for_child_write[2];
    pipe(fd_for_parent_write);
    pipe(fd_for_child_write);
    int pid = fork();
    if (pid == 0) {
        // 子进程
        close(fd_for_child_write[0]);
        close(fd_for_parent_write[1]);
        read(fd_for_parent_write[0], buf, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(fd_for_child_write[1], buf, 1);
        exit(0);
    } else {
        // 父进程
        close(fd_for_parent_write[0]);
        close(fd_for_child_write[1]);
        write(fd_for_parent_write[1], 'a', 1);
        read(fd_for_child_write[0], buf, 1);
        fprintf(1, "%d: received pong\n", getpid());
        exit(0);
    }
}
