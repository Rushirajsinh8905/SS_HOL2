/*
Name : 29a.c
Author : Dabhi Rushirajsinh
Description : Write a separate program using the signal system call to catch
              the SIGSEGV signal (segmentation fault - invalid memory access).
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void segv_handler(int signum)
{
    printf("Caught SIGSEGV (signal %d): Invalid memory access!\n", signum);
    exit(1);
}

int main()
{
    signal(SIGSEGV, segv_handler);

    printf("Registered SIGSEGV handler. Triggering segfault...\n");

    int *p = NULL;
    *p = 100;   /* Dereference NULL pointer -> SIGSEGV */

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ gcc 25a.c -o 25a

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ ./25a
Registered SIGSEGV handler. Triggering segfault...
Caught SIGSEGV (signal 11): Invalid memory access!

*/
