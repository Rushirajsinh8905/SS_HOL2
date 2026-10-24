/*
Name : 8b.c
Author : Dabhi Rushirajsinh
Description : Write a program (Process B) for two-way communication through
              FIFO. Process B reads from pipe1 (sent by Process A) and writes
              a reply to pipe2. Run with 8a.c in another terminal.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    char buffer[100];
    int fd1, fd2;

    while (1) {
        /* Read message from pipe1 (sent by Process A) */
        fd1 = open("pipe1", O_RDONLY);
        read(fd1, buffer, sizeof(buffer));
        close(fd1);

        printf("Process A: %s\n", buffer);

        if (strncmp(buffer, "exit", 4) == 0)
            break;

        printf("Process B: Enter reply: ");
        fgets(buffer, sizeof(buffer), stdin);

        /* Write reply to pipe2 (Process A reads from pipe2) */
        fd2 = open("pipe2", O_WRONLY);
        write(fd2, buffer, strlen(buffer) + 1);
        close(fd2);

        if (strncmp(buffer, "exit", 4) == 0)
            break;
    }

    return 0;
}

/*
Sample Output :

# Terminal 2 - Process B (after starting 8a in Terminal 1):
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/8$ ./8b
Process A: Hello from Process A!
Process B: Enter reply: Hello from Process B!
Process A: exit

*/
