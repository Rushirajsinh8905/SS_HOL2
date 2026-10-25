/*
Name : 18b.c
Author : Dabhi Rushirajsinh
Description : Write a program to create a counting semaphore and initialize
              its value. The count represents the number of available resources.
              Pass the resource count as a command-line argument.
              Usage: ./18b <count>
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#ifndef __APPLE__
union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
};
#endif

int main(int argc, char *argv[])
{
    int N;

    if (argc < 2) {
        printf("Usage: %s <resource_count>\n", argv[0]);
        printf("Example: %s 5\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    if (N <= 0) {
        printf("Resource count must be a positive integer.\n");
        return 1;
    }

    key_t key   = ftok("semfile", 66);
    int   semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) { perror("semget"); exit(1); }

    union semun arg;
    arg.val = N;
    if (semctl(semid, 0, SETVAL, arg) == -1) { perror("semctl SETVAL"); exit(1); }

    printf("Counting semaphore created.\n");
    printf("  semid         = %d\n", semid);
    printf("  initial value = %d  (number of available resources)\n", semctl(semid, 0, GETVAL));

    semctl(semid, 0, IPC_RMID);

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/18$ gcc 18b.c -o 18b

# Run with 5 resources
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/18$ ./18b 5
Counting semaphore created.
  semid         = 65537
  initial value = 5  (number of available resources)

# Run with 10 resources
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/18$ ./18b 10
Counting semaphore created.
  semid         = 65538
  initial value = 10  (number of available resources)

*/
