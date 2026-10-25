/*
Name : 32.c
Author : Dabhi Rushirajsinh
Description : Write a program to ignore a SIGINT signal and then reset it
              to the default action using the sigaction system call.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main()
{
    struct sigaction sa_ignore, sa_default;

    printf("PID = %d\n", getpid());

    /* Step 1: Ignore SIGINT using sigaction with SIG_IGN */
    sa_ignore.sa_handler = SIG_IGN;
    sigemptyset(&sa_ignore.sa_mask);
    sa_ignore.sa_flags = 0;

    sigaction(SIGINT, &sa_ignore, NULL);
    printf("SIGINT IGNORED via sigaction. Press Ctrl+C (5 seconds)...\n");
    sleep(5);

    /* Step 2: Reset to default using SIG_DFL */
    sa_default.sa_handler = SIG_DFL;
    sigemptyset(&sa_default.sa_mask);
    sa_default.sa_flags = 0;

    sigaction(SIGINT, &sa_default, NULL);
    printf("SIGINT reset to DEFAULT via sigaction. Press Ctrl+C to terminate.\n");

    while (1) {
        sleep(1);
    }

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/32$ gcc 28.c -o 28

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/32$ ./28
PID = 1235
SIGINT IGNORED via sigaction. Press Ctrl+C (5 seconds)...
^C                             (no effect — ignored)
SIGINT reset to DEFAULT via sigaction. Press Ctrl+C to terminate.
^C
(Process terminates.)

*/
