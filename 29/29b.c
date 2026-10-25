/*
Name : 29b.c
Author : Dabhi Rushirajsinh
Description : Write a separate program using the signal system call to catch
              the SIGINT signal (interrupt from keyboard, i.e., Ctrl+C).
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void int_handler(int sig)
{
    printf("\nCaught SIGINT (signal %d): Ctrl+C pressed!\n", sig);
    exit(0);
}

int main()
{
    signal(SIGINT, int_handler);

    printf("SIGINT handler registered. Press Ctrl+C to trigger...\n");

    while (1) {
        printf("Working...\n");
        sleep(1);
    }

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ gcc 25b.c -o 25b

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/29$ ./25b
SIGINT handler registered. Press Ctrl+C to trigger...
Working...
Working...
^C
Caught SIGINT (signal 2): Ctrl+C pressed!

*/
