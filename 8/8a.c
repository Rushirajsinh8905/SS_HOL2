/*
Name : 8a.c
Author : Dabhi Rushirajsinh
Description : Write a program (Process A) for two-way communication through
              FIFO. Process A sends a message via pipe1 and reads the reply
              from Process B via pipe2. Run with 8b.c in another terminal.
              Two FIFOs (pipe1, pipe2) must exist before running:
              $ mkfifo pipe1 pipe2
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

    /* Create FIFOs (ignore errors if they exist already) */
    mkfifo("pipe1", 0666);
    mkfifo("pipe2", 0666);

    while (1) {
        printf("Process A: Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);

        /* Write to pipe1 (Process B reads from pipe1) */
        fd1 = open("pipe1", O_WRONLY);
        write(fd1, buffer, strlen(buffer) + 1);
        close(fd1);

        if (strncmp(buffer, "exit", 4) == 0)
            break;

        /* Read reply from pipe2 (Process B writes to pipe2) */
        fd2 = open("pipe2", O_RDONLY);
        read(fd2, buffer, sizeof(buffer));
        close(fd2);

        printf("Process B replied: %s\n", buffer);
    }

    unlink("pipe1");
    unlink("pipe2");

    return 0;
}

/*
Sample Output :

# Compile both programs
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/8$ gcc 8a.c -o 8a
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/8$ gcc 8b.c -o 8b

# Terminal 1 - Process A:
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/8$ ./8a
Process A: Enter message: Hello from Process A!
Process B replied: Hello from Process B!
Process A: Enter message: exit

# Terminal 2 - Process B:
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/8$ ./8b
Process A: Hello from Process A!
Process B: Enter reply: Hello from Process B!
Process A: exit

*/
