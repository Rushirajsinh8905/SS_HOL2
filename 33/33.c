/*
Name : 33.c
Author : Dabhi Rushirajsinh
Description : Write a program to create an orphan process. Utilize the kill
              system call to send a SIGKILL signal from the child process to
              the parent process.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) {
        /* Parent process */
        printf("Parent: PID = %d, Child PID = %d\n", getpid(), pid);
        printf("Parent: Sleeping for 2 seconds before being killed by child...\n");
        sleep(2);

        /* Parent will be killed by child before reaching here */
        printf("Parent: This line should not print if child kills parent.\n");

    } else {
        /* Child process */
        printf("Child: PID = %d, Parent PID = %d\n", getpid(), getppid());
        sleep(1);   /* Let parent settle */

        printf("Child: Sending SIGKILL to parent (PID=%d)...\n", getppid());
        kill(getppid(), SIGKILL);

        /* Child continues running; parent is now dead -> child becomes orphan */
        sleep(1);
        printf("Child: Parent is dead. My new PPID (adopted by init) = %d\n", getppid());
    }

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/33$ gcc 29.c -o 29

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/33$ ./29
Parent: PID = 1236, Child PID = 1237
Parent: Sleeping for 2 seconds before being killed by child...
Child: PID = 1237, Parent PID = 1236
Child: Sending SIGKILL to parent (PID=1236)...
Killed
Child: Parent is dead. My new PPID (adopted by init) = 1

(The orphan child is re-parented to PID 1 (init/systemd) after the parent dies.)

*/
