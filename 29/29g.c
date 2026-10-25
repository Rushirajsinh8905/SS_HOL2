/*
Name : 29g.c
Author : Dabhi Rushirajsinh
Description : Write a separate program using the signal system call to catch
              the SIGPROF signal, triggered using setitimer (ITIMER_PROF).
              ITIMER_PROF counts user + kernel (system) CPU time.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

static int count = 0;

void prof_handler(int sig)
{
    count++;
    printf("Caught SIGPROF (signal %d, count=%d): Profiling timer expired!\n", sig, count);
    if (count >= 3) {
        printf("Done.\n");
        exit(0);
    }
}

int main()
{
    struct itimerval timer;

    signal(SIGPROF, prof_handler);

    timer.it_value.tv_sec  = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec  = 1;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_PROF, &timer, NULL);

    printf("ITIMER_PROF set. Busy-looping to consume CPU + kernel time...\n");

    while (1) {
        volatile long i;
        for (i = 0; i < 1000000L; i++);
    }

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ gcc 25g.c -o 25g

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ ./25g
ITIMER_PROF set. Busy-looping to consume CPU + kernel time...
Caught SIGPROF (signal 27, count=1): Profiling timer expired!
Caught SIGPROF (signal 27, count=2): Profiling timer expired!
Caught SIGPROF (signal 27, count=3): Profiling timer expired!
Done.

*/
