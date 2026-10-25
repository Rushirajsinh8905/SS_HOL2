/*
Name : 23a.c
Author : Dabhi Rushirajsinh
Description : Write a program to set an interval timer for 10 seconds and
              10 microseconds using the ITIMER_REAL time domain.
              ITIMER_REAL decrements in real (wall-clock) time and delivers
              SIGALRM on expiry.
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
    printf("ITIMER_REAL: SIGALRM received (count = %d)\n", count);
    if (count >= 3) {
        printf("Received 3 alarms. Exiting.\n");
        exit(0);
    }
}

int main()
{
    struct itimerval timer;

    signal(SIGALRM, handler);

    /* First expiry: 10 seconds + 0 microseconds */
    timer.it_value.tv_sec  = 10;
    timer.it_value.tv_usec = 0;

    /* Interval (repeat every): 10 seconds + 10 microseconds */
    timer.it_interval.tv_sec  = 10;
    timer.it_interval.tv_usec = 10;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        exit(1);
    }

    printf("ITIMER_REAL set: first alarm in 10s, then every 10s+10us.\n");
    printf("Waiting for signals (press Ctrl+C to stop early)...\n");

    while (1)
        pause();   /* Sleep until signal */

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/23$ gcc 19a.c -o 19a

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/23$ ./19a
ITIMER_REAL set: first alarm in 10s, then every 10s+10us.
Waiting for signals (press Ctrl+C to stop early)...
ITIMER_REAL: SIGALRM received (count = 1)
ITIMER_REAL: SIGALRM received (count = 2)
ITIMER_REAL: SIGALRM received (count = 3)
Received 3 alarms. Exiting.

(Each alarm fires approximately every 10 seconds. Ctrl+C terminates early.)

*/
