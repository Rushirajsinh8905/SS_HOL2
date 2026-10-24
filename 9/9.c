/*
Name : 9.c
Author : Dabhi Rushirajsinh
Description : Write a program to wait for data to be written into a FIFO
              within 10 seconds, utilizing the select system call with the FIFO.
              If no data arrives within 10 seconds, a timeout is reported.
              Run 9_writer.c in another terminal to send data.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>
#include <string.h>

#define FIFO_NAME "select_fifo"

int main()
{
    char buffer[100];
    fd_set readfds;
    struct timeval timeout;
    int fd, ret, n;

    mkfifo(FIFO_NAME, 0666);

    printf("Waiting for data in FIFO '%s' for 10 seconds...\n", FIFO_NAME);

    /* Open FIFO non-blocking so we don't block waiting for writer */
    fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    timeout.tv_sec  = 10;
    timeout.tv_usec = 0;

    ret = select(fd + 1, &readfds, NULL, NULL, &timeout);

    if (ret == 0) {
        printf("Timeout: No data written to FIFO within 10 seconds.\n");
    } else if (ret > 0) {
        if (FD_ISSET(fd, &readfds)) {
            n = read(fd, buffer, sizeof(buffer) - 1);
            if (n > 0) {
                buffer[n] = '\0';
                printf("Data received: %s\n", buffer);
            }
        }
    } else {
        perror("select");
    }

    close(fd);
    unlink(FIFO_NAME);

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/9$ gcc 9.c -o 9
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/9$ gcc 9_writer.c -o 9_writer

# Terminal 1 (start reader):
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/9$ ./9
Waiting for data in FIFO 'select_fifo' for 10 seconds...
Data received: Hello from writer process!

# Terminal 2 (run writer within 10 seconds):
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/9$ ./9_writer

# If writer is NOT run within 10 seconds:
Timeout: No data written to FIFO within 10 seconds.

*/
