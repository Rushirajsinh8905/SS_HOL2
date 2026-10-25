/*
Name : 29c.c
Author : Dabhi Rushirajsinh
Description : Write a separate program using the signal system call to catch
              the SIGFPE signal (floating-point exception, e.g., divide by zero).
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void fpe_handler(int sig)
{
    printf("Caught SIGFPE (signal %d): Divide by zero or arithmetic error!\n", sig);
    exit(1);
}

int main()
{
    signal(SIGFPE, fpe_handler);

    printf("SIGFPE handler registered. Triggering divide-by-zero...\n");

    volatile int x = 0;
    volatile int y = 1 / x;   /* Integer divide by zero -> SIGFPE */
    (void)y;

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ gcc 25c.c -o 25c

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ ./25c
SIGFPE handler registered. Triggering divide-by-zero...
Caught SIGFPE (signal 8): Divide by zero or arithmetic error!

*/
