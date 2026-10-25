/*
Name : 31a.c
Author : Dabhi Rushirajsinh
Description : Write a separate program using the sigaction system call to
              catch the SIGSEGV signal (segmentation fault).
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int signum)
{
    printf("Caught SIGSEGV (signal %d) via sigaction: Invalid memory access!\n", signum);
    exit(1);
}

int main()
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("SIGSEGV handler (sigaction) registered. Triggering segfault...\n");

    int *p = NULL;
    *p = 42;   /* NULL dereference -> SIGSEGV */

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/31$ gcc 27a.c -o 27a

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/31$ ./27a
SIGSEGV handler (sigaction) registered. Triggering segfault...
Caught SIGSEGV (signal 11) via sigaction: Invalid memory access!

*/
