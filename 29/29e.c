/*
Name : 29e.c
Author : Dabhi Rushirajsinh
Description : Write a separate program using the signal system call to catch
              the SIGALRM signal, triggered using the setitimer system call
              (ITIMER_REAL domain).
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

time_t start_time;

void alarm_handler(int sig)
{
    time_t now = time(NULL);
    printf("Caught SIGALRM via setitimer (signal %d). Elapsed: %ld seconds\n",
           sig, (long)(now - start_time));
}

int main()
{
    struct itimerval timer;

    signal(SIGALRM, alarm_handler);

    start_time = time(NULL);

    timer.it_value.tv_sec  = 3;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec  = 3;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);

    printf("ITIMER_REAL set via setitimer. Fires every 3 seconds.\n");

    int count = 0;
    while (count < 3) {
        pause();
        count++;
    }

    printf("Received 3 SIGALRM signals. Done.\n");

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ gcc 25e.c -o 25e

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ ./25e
ITIMER_REAL set via setitimer. Fires every 3 seconds.
Caught SIGALRM via setitimer (signal 14). Elapsed: 3 seconds
Caught SIGALRM via setitimer (signal 14). Elapsed: 6 seconds
Caught SIGALRM via setitimer (signal 14). Elapsed: 9 seconds
Received 3 SIGALRM signals. Done.

*/
