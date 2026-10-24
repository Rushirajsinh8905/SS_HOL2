/*
Name : 5.c
Author : Dabhi Rushirajsinh
Description : Write a program to find the total number of directories in the
              present working directory using ls -l | grep ^d | wc.
              Implement the solution using only dup2.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int fd1[2];   /* pipe between ls -l and grep ^d */
    int fd2[2];   /* pipe between grep ^d and wc    */

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("pipe");
        exit(1);
    }

    /* Child 1: run ls -l, redirect stdout to fd1 write-end */
    if (fork() == 0) {
        close(fd1[0]);
        close(fd2[0]); close(fd2[1]);
        dup2(fd1[1], 1);
        close(fd1[1]);
        execlp("ls", "ls", "-l", NULL);
        exit(1);
    }

    /* Child 2: run grep ^d, stdin from fd1 read-end, stdout to fd2 write-end */
    if (fork() == 0) {
        close(fd1[1]);
        close(fd2[0]);
        dup2(fd1[0], 0);
        close(fd1[0]);
        dup2(fd2[1], 1);
        close(fd2[1]);
        execlp("grep", "grep", "^d", NULL);
        exit(1);
    }

    /* Child 3: run wc, stdin from fd2 read-end */
    if (fork() == 0) {
        close(fd1[0]); close(fd1[1]);
        close(fd2[1]);
        dup2(fd2[0], 0);
        close(fd2[0]);
        execlp("wc", "wc", "-l", NULL);
        exit(1);
    }

    /* Parent: close all pipe ends and wait for all children */
    close(fd1[0]); close(fd1[1]);
    close(fd2[0]); close(fd2[1]);
    wait(NULL); wait(NULL); wait(NULL);

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/5$ gcc 5.c -o 5

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/5$ ./5
3

(3 means there are 3 subdirectories in the current directory. Actual number depends on pwd.)

*/
