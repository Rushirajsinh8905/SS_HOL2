/*
Name : 1.c
Author : Dabhi Rushirajsinh
Description : Write a simple program to create a pipe, write to the pipe,
              read from the pipe, and display the content on the monitor.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int fd[2];
    char write_msg[] = "Hello from the pipe!";
    char read_msg[100];
    int n;

    /* Create the pipe */
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    /* Write to the write-end of the pipe */
    write(fd[1], write_msg, strlen(write_msg));
    close(fd[1]);   /* Close write-end after writing */

    /* Read from the read-end of the pipe */
    n = read(fd[0], read_msg, sizeof(read_msg) - 1);
    read_msg[n] = '\0';
    close(fd[0]);   /* Close read-end after reading */

    printf("Message read from pipe: %s\n", read_msg);

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/1$ gcc 1.c -o 1

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/1$ ./1
Message read from pipe: Hello from the pipe!

*/
