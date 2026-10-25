/*
Name : 29f.c
Author : Dabhi Rushirajsinh
Description : Write a separate program using the signal system call to catch
              the SIGVTALRM signal, triggered using setitimer (ITIMER_VIRTUAL).
              ITIMER_VIRTUAL counts only user-mode CPU time.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

static int count = 0;

void vtalrm_handler(int sig)
{
    count++;
    printf("Caught SIGVTALRM (signal %d, count=%d): Virtual timer expired!\n", sig, count);
    if (count >= 3) {
        printf("Done.\n");
        exit(0);
    }
}

int main()
{
    struct itimerval timer;

    signal(SIGVTALRM, vtalrm_handler);

    timer.it_value.tv_sec  = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec  = 1;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    printf("ITIMER_VIRTUAL set. Busy-looping to consume CPU time...\n");

    /* Must consume CPU to trigger ITIMER_VIRTUAL */
    while (1) {
        volatile long i;
        for (i = 0; i < 1000000L; i++);
    }

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ gcc 25f.c -o 25f

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ ./25f
ITIMER_VIRTUAL set. Busy-looping to consume CPU time...
Caught SIGVTALRM (signal 26, count=1): Virtual timer expired!
Caught SIGVTALRM (signal 26, count=2): Virtual timer expired!
Caught SIGVTALRM (signal 26, count=3): Virtual timer expired!
Done.

*/
