/*
Name : 10.c
Author : Dabhi Rushirajsinh
Description : Write a program to print the maximum number of files that can
              be opened within a process and the size of a pipe (circular buffer).
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <fcntl.h>

int main()
{
    struct rlimit rl;

    /* Maximum number of open files */
    if (getrlimit(RLIMIT_NOFILE, &rl) == 0) {
        printf("Max open files (soft limit): %ld\n", (long)rl.rlim_cur);
        printf("Max open files (hard limit): %ld\n", (long)rl.rlim_max);
    } else {
        perror("getrlimit");
    }

    /* Size of a pipe (circular buffer) */
    int pipefd[2];
    if (pipe(pipefd) == 0) {
#ifdef F_GETPIPE_SZ
        int size = fcntl(pipefd[0], F_GETPIPE_SZ);
        printf("Default pipe size (circular buffer): %d bytes\n", size);
#else
        /* On Linux the default is 65536 bytes (64 KB) since kernel 2.6.11 */
        printf("Default pipe size (circular buffer): 65536 bytes (Linux default, F_GETPIPE_SZ not available)\n");
#endif
        close(pipefd[0]);
        close(pipefd[1]);
    } else {
        perror("pipe");
    }

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/10$ gcc 10.c -o 10

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/10$ ./10
Max open files (soft limit): 1024
Max open files (hard limit): 4096
Default pipe size (circular buffer): 65536 bytes

(Values depend on the Linux kernel configuration and ulimit settings.)

*/
