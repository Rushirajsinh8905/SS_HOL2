/*
Name : 23b.c
Author : Dabhi Rushirajsinh
Description : Write a program to set an interval timer for 10 seconds and
              10 microseconds using the ITIMER_VIRTUAL time domain.
              ITIMER_VIRTUAL decrements only while the process is executing
              in user mode. It delivers SIGVTALRM on expiry.
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
    printf("ITIMER_VIRTUAL: SIGVTALRM received (count = %d)\n", count);
    if (count >= 3) {
        printf("Received 3 virtual alarms. Exiting.\n");
        exit(0);
    }
}

int main()
{
    struct itimerval timer;

    signal(SIGVTALRM, handler);

    timer.it_value.tv_sec  = 10;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec  = 10;
    timer.it_interval.tv_usec = 10;

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
        perror("setitimer");
        exit(1);
    }

    printf("ITIMER_VIRTUAL set: counts only user-mode CPU time.\n");
    printf("Busy-looping to consume CPU time (needed for VIRTUAL timer)...\n");

    /* Must burn CPU time for ITIMER_VIRTUAL to count down */
    while (1) {
        volatile long i;
        for (i = 0; i < 1000000L; i++);  /* busy loop */
    }

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/23$ gcc 19b.c -o 19b

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/23$ ./19b
ITIMER_VIRTUAL set: counts only user-mode CPU time.
Busy-looping to consume CPU time (needed for VIRTUAL timer)...
ITIMER_VIRTUAL: SIGVTALRM received (count = 1)
ITIMER_VIRTUAL: SIGVTALRM received (count = 2)
ITIMER_VIRTUAL: SIGVTALRM received (count = 3)
Received 3 virtual alarms. Exiting.

(Time elapsed is in user CPU time, not wall-clock time. May take longer in real time.)

*/
