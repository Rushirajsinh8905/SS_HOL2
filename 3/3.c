/*
Name : 3.c
Author : Dabhi Rushirajsinh
Description : Write a program to send and receive data between the parent and
              child processes using two-way communication (two pipes).
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int fd1[2];   /* parent -> child */
    int fd2[2];   /* child -> parent */
    char parent_msg[] = "Hello Child, how are you?";
    char child_msg[]  = "I am fine, Parent!";
    char buffer[100];
    pid_t pid;

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) {
        /* Parent: write on fd1, read from fd2 */
        close(fd1[0]);
        close(fd2[1]);

        write(fd1[1], parent_msg, strlen(parent_msg) + 1);
        close(fd1[1]);

        read(fd2[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);
        close(fd2[0]);
        wait(NULL);
    } else {
        /* Child: read from fd1, write on fd2 */
        close(fd1[1]);
        close(fd2[0]);

        read(fd1[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(fd1[0]);

        write(fd2[1], child_msg, strlen(child_msg) + 1);
        close(fd2[1]);
    }

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/3$ gcc 3.c -o 3

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/3$ ./3
Child received: Hello Child, how are you?
Parent received: I am fine, Parent!

*/
