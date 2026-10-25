/*
Name : 30.c
Author : Dabhi Rushirajsinh
Description : Write a program to ignore a SIGINT signal and then reset it
              to the default action using the signal system call.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main()
{
    printf("PID = %d\n", getpid());

    /* Step 1: Ignore SIGINT */
    signal(SIGINT, SIG_IGN);
    printf("SIGINT is now IGNORED. Press Ctrl+C (nothing will happen for 5 seconds)...\n");
    sleep(5);

    /* Step 2: Reset SIGINT to default action (terminate) */
    signal(SIGINT, SIG_DFL);
    printf("SIGINT reset to DEFAULT. Press Ctrl+C again to terminate the process.\n");

    while (1) {
        sleep(1);
    }

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/30$ gcc 26.c -o 26

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/30$ ./26
PID = 1234
SIGINT is now IGNORED. Press Ctrl+C (nothing will happen for 5 seconds)...
^C                         (Ctrl+C pressed - no effect while ignored)
^C                         (Ctrl+C again  - still no effect)
SIGINT reset to DEFAULT. Press Ctrl+C again to terminate the process.
^C
(Process terminates with SIGINT default action.)

*/
