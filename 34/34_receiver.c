/*
Name : 34_receiver.c
Author : Dabhi Rushirajsinh
Description : Write a program (receiver) that attempts to catch the SIGSTOP
              signal using the signal system call. Determines whether SIGSTOP
              can be caught. (Answer: No — SIGSTOP cannot be caught, blocked,
              or ignored by any process; it is handled directly by the kernel.)
              Run this in Terminal 1. Then use 30_sender.c from Terminal 2.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void stop_handler(int sig)
{
    /* This handler will NEVER execute for SIGSTOP */
    printf("Caught SIGSTOP! (this should never print)\n");
}

int main()
{
    printf("Receiver PID = %d\n", getpid());
    printf("Attempting to register SIGSTOP handler (signal returns SIG_ERR)...\n");

    if (signal(SIGSTOP, stop_handler) == SIG_ERR) {
        perror("signal(SIGSTOP)");
        printf("CONCLUSION: SIGSTOP CANNOT be caught, blocked, or ignored.\n");
    } else {
        printf("Handler registered (unexpected on Linux).\n");
    }

    printf("Waiting... send SIGSTOP from Terminal 2 using 30_sender.\n");
    printf("(Process will pause; resume with SIGCONT: kill -CONT %d)\n", getpid());

    while (1) {
        sleep(1);
    }

    return 0;
}

/*
Sample Output :

# Compile both programs
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/34$ gcc 30_receiver.c -o 30_receiver
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/34$ gcc 30_sender.c -o 30_sender

# Terminal 1 (receiver):
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/34$ ./30_receiver
Receiver PID = 1240
Attempting to register SIGSTOP handler (signal returns SIG_ERR)...
signal(SIGSTOP): Invalid argument
CONCLUSION: SIGSTOP CANNOT be caught, blocked, or ignored.
Waiting... send SIGSTOP from Terminal 2 using 30_sender.
(Process will pause; resume with SIGCONT: kill -CONT 1240)

# Terminal 2 (sender):
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/34$ ./30_sender 1240
Sent SIGSTOP to PID 1240. Process is now stopped.
Resume it with: kill -CONT 1240

# Back in Terminal 1: process output is frozen (stopped by kernel, not caught)
# Restore with: kill -CONT 1240

*/
