/*
Name : 23c.c
Author : Dabhi Rushirajsinh
Description : Write a program to set an interval timer for 10 seconds and
              10 microseconds using the ITIMER_PROF time domain.
              ITIMER_PROF decrements in user + kernel (system call) time
              while the process is running. It delivers SIGPROF on expiry.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

static int count = 0;

void handler(int signum)
{
    count++;
    printf("ITIMER_PROF: SIGPROF received (count = %d)\n", count);
    if (count >= 3) {
        printf("Received 3 prof alarms. Exiting.\n");
        exit(0);
    }
}

int main()
{
    struct itimerval timer;

    signal(SIGPROF, handler);

    timer.it_value.tv_sec  = 10;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec  = 10;
    timer.it_interval.tv_usec = 10;

    if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
        perror("setitimer");
        exit(1);
    }

    printf("ITIMER_PROF set: counts user + kernel CPU time.\n");
    printf("Busy-looping to consume CPU time...\n");

    while (1) {
        volatile long i;
        for (i = 0; i < 1000000L; i++);
    }

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/23$ gcc 19c.c -o 19c

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/23$ ./19c
ITIMER_PROF set: counts user + kernel CPU time.
Busy-looping to consume CPU time...
ITIMER_PROF: SIGPROF received (count = 1)
ITIMER_PROF: SIGPROF received (count = 2)
ITIMER_PROF: SIGPROF received (count = 3)
Received 3 prof alarms. Exiting.

*/
