/*
Name : 29d.c
Author : Dabhi Rushirajsinh
Description : Write a separate program using the signal system call to catch
              the SIGALRM signal, triggered using the alarm() system call.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void alarm_handler(int sig)
{
    printf("Caught SIGALRM (signal %d): alarm() fired!\n", sig);
}

int main()
{
    signal(SIGALRM, alarm_handler);

    printf("Setting alarm for 3 seconds...\n");
    alarm(3);   /* Deliver SIGALRM after 3 seconds */

    while (1) {
        printf("Waiting...\n");
        sleep(1);
    }

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ gcc 25d.c -o 25d

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ ./25d
Setting alarm for 3 seconds...
Waiting...
Waiting...
Waiting...
Caught SIGALRM (signal 14): alarm() fired!
Waiting...
Waiting...
...
(Continues indefinitely after alarm. Press Ctrl+C to stop.)

*/
